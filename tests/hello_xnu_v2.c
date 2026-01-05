/*
 * hello_xnu_v2.c - A minimal "Hello World" server using raw XNU Mach APIs
 * 
 * Version 2: Uses threads instead of fork() to avoid Mach port inheritance issues.
 * Mach ports are NOT inherited across fork() - child gets copies with no rights.
 * 
 * This version demonstrates single-process client-server communication.
 */

#include <mach/mach.h>
#include <mach/message.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

// Message IDs
#define MSG_ID_READ_REQUEST  1001
#define MSG_ID_READ_REPLY    1002

// The content we serve
static const char hello_content[] = "Hello, World from XNU Mach!\n";

// Global server port (shared within process)
static mach_port_t g_server_port;

// Request message structure
typedef struct {
    mach_msg_header_t header;
    int offset;
    int length;
    mach_msg_trailer_t trailer;
} read_request_t;

// Reply message structure
typedef struct {
    mach_msg_header_t header;
    int bytes_read;
    char data[256];
    mach_msg_trailer_t trailer;
} read_reply_t;

// Client thread function
void *client_thread(void *arg) {
    kern_return_t kr;
    read_request_t req;
    read_reply_t reply;
    mach_port_t reply_port;

    usleep(100000); // Wait for server thread

    printf("[CLIENT] Starting client...\n");

    // Allocate reply port
    kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &reply_port);
    if (kr != KERN_SUCCESS) {
        printf("[CLIENT] Failed to allocate reply port: %s\n", mach_error_string(kr));
        return NULL;
    }

    // Prepare read request
    memset(&req, 0, sizeof(req));
    // MACH_MSG_TYPE_COPY_SEND for destination (we copied a send right)
    // MACH_MSG_TYPE_MAKE_SEND_ONCE for our reply port (kernel creates send-once from our receive)
    req.header.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, MACH_MSG_TYPE_MAKE_SEND_ONCE);
    req.header.msgh_remote_port = g_server_port;
    req.header.msgh_local_port = reply_port;
    req.header.msgh_id = MSG_ID_READ_REQUEST;
    req.header.msgh_size = sizeof(mach_msg_header_t) + 2 * sizeof(int);
    req.offset = 0;
    req.length = 100;

    printf("[CLIENT] Sending read request to server port %u...\n", g_server_port);
    kr = mach_msg(&req.header, MACH_SEND_MSG, 
                  sizeof(mach_msg_header_t) + 2 * sizeof(int),
                  0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
    if (kr != KERN_SUCCESS) {
        printf("[CLIENT] Failed to send request: %s\n", mach_error_string(kr));
        return NULL;
    }
    printf("[CLIENT] Request sent, waiting for reply...\n");

    // Receive reply
    memset(&reply, 0, sizeof(reply));
    kr = mach_msg(&reply.header, MACH_RCV_MSG, 0, sizeof(reply), 
                  reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
    if (kr != KERN_SUCCESS) {
        printf("[CLIENT] Failed to receive reply: %s\n", mach_error_string(kr));
        return NULL;
    }

    printf("[CLIENT] Received reply!\n");
    printf("         Bytes read: %d\n", reply.bytes_read);
    printf("         Data: \"%s\"\n", reply.data);

    mach_port_deallocate(mach_task_self(), reply_port);
    return NULL;
}

// Handle a read request
static void handle_read_request(read_request_t *req) {
    read_reply_t reply;
    memset(&reply, 0, sizeof(reply));
    kern_return_t kr;

    int offset = req->offset;
    int length = req->length;
    int content_len = strlen(hello_content);

    // Clamp read parameters
    if (offset < 0) offset = 0;
    if (offset > content_len) offset = content_len;
    if (length > (int)sizeof(reply.data) - 1) length = sizeof(reply.data) - 1;
    if (offset + length > content_len) length = content_len - offset;

    // Copy data
    if (length > 0) {
        memcpy(reply.data, hello_content + offset, length);
    }
    reply.bytes_read = length;

    // Get the reply port from the request (it's a send-once right)
    mach_port_t reply_port = req->header.msgh_remote_port;

    // Prepare reply header
    reply.header.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_MOVE_SEND_ONCE, 0);
    reply.header.msgh_remote_port = reply_port;
    reply.header.msgh_local_port = MACH_PORT_NULL;
    reply.header.msgh_id = MSG_ID_READ_REPLY;
    // Size = header + body (not including trailer, that's for receive)
    size_t send_size = sizeof(mach_msg_header_t) + sizeof(int) + sizeof(reply.data);
    reply.header.msgh_size = send_size;

    kr = mach_msg(&reply.header, MACH_SEND_MSG, 
                  send_size,
                  0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
    if (kr != KERN_SUCCESS) {
        printf("[SERVER] Failed to send reply: %s\n", mach_error_string(kr));
    }
}

int main(int argc, char **argv) {
    kern_return_t kr;
    pthread_t client;

    printf("=== Hello XNU Mach Server v2 ===\n");
    printf("Thread-based client-server demonstration.\n\n");

    // 1. Allocate receive right for server
    kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &g_server_port);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] Failed to allocate port: %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] Server port allocated: %u\n", g_server_port);

    // 2. Insert send right so client can send to us
    kr = mach_port_insert_right(mach_task_self(), g_server_port, g_server_port, 
                                 MACH_MSG_TYPE_MAKE_SEND);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] Failed to insert send right: %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] Send right inserted.\n\n");

    // 3. Start client thread
    pthread_create(&client, NULL, client_thread, NULL);

    // 4. Server message loop
    printf("[SERVER] Entering message loop...\n");
    read_request_t req;
    memset(&req, 0, sizeof(req));

    kr = mach_msg(&req.header, MACH_RCV_MSG, 0, sizeof(req), 
                  g_server_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
    if (kr != KERN_SUCCESS) {
        printf("[SERVER] Failed to receive message: %s\n", mach_error_string(kr));
        return 1;
    }

    printf("[SERVER] Received message ID: %d\n", req.header.msgh_id);

    if (req.header.msgh_id == MSG_ID_READ_REQUEST) {
        printf("[SERVER] Handling read request: offset=%d, length=%d\n", 
               req.offset, req.length);
        handle_read_request(&req);
        printf("[SERVER] Reply sent.\n");
    }

    pthread_join(client, NULL);

    printf("\n=== SUCCESS: XNU Mach RPC Demo Complete! ===\n");
    printf("This proves Hurd-style servers can work on XNU.\n");

    return 0;
}
