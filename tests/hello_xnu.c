/*
 * hello_xnu.c - A minimal "Hello World" server using raw XNU Mach APIs
 * 
 * This is a proof-of-concept demonstrating that we can write a Hurd-style
 * server that receives Mach messages and handles RPC-like requests,
 * but using only XNU-native Mach interfaces.
 * 
 * This is a stepping stone toward running real Hurd translators on XNU.
 * 
 * The server:
 *   1. Creates a receive right (service port)
 *   2. Registers itself with launchd/bootstrap (or just prints the port name)
 *   3. Enters a message loop
 *   4. Responds to a simple "read" request with "Hello, World from XNU!"
 */

#include <mach/mach.h>
#include <mach/message.h>
#include <servers/bootstrap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Message IDs (simulating a simple protocol)
#define MSG_ID_READ_REQUEST  1001
#define MSG_ID_READ_REPLY    1002

// The content we serve
static const char hello_content[] = "Hello, World from XNU Mach!\n";

// Request message structure
typedef struct {
    mach_msg_header_t header;
    int offset;                  // Read offset
    int length;                  // Requested length
    mach_msg_trailer_t trailer;
} read_request_t;

// Reply message structure
typedef struct {
    mach_msg_header_t header;
    int bytes_read;
    char data[256];
} read_reply_t;

// Handle a read request
static void handle_read_request(read_request_t *req, mach_port_t reply_port) {
    read_reply_t reply;
    memset(&reply, 0, sizeof(reply));

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

    // Prepare reply header
    reply.header.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_MOVE_SEND_ONCE, 0);
    reply.header.msgh_remote_port = reply_port;
    reply.header.msgh_local_port = MACH_PORT_NULL;
    reply.header.msgh_id = MSG_ID_READ_REPLY;
    reply.header.msgh_size = sizeof(mach_msg_header_t) + sizeof(int) + length + 1;

    kern_return_t kr = mach_msg(&reply.header, MACH_SEND_MSG, 
                                sizeof(reply.header) + sizeof(int) + length + 1,
                                0, MACH_PORT_NULL, 
                                MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
    if (kr != KERN_SUCCESS) {
        printf("[SERVER] Failed to send reply: %s\n", mach_error_string(kr));
    }
}

int main(int argc, char **argv) {
    kern_return_t kr;
    mach_port_t server_port;

    printf("=== Hello XNU Mach Server ===\n");
    printf("A proof-of-concept Hurd-style server on XNU.\n\n");

    // 1. Allocate a receive right for our service
    kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &server_port);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] Failed to allocate port: %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] Server port allocated: %u\n", server_port);

    // 2. Insert a send right so clients can connect (for demo, we'll be our own client)
    kr = mach_port_insert_right(mach_task_self(), server_port, server_port, 
                                 MACH_MSG_TYPE_MAKE_SEND);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] Failed to insert send right: %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] Send right inserted.\n");

    // 3. Fork a child to act as a client
    pid_t pid = fork();
    if (pid == 0) {
        // === CLIENT PROCESS ===
        usleep(100000); // Wait for server to be ready

        read_request_t req;
        read_reply_t reply;
        mach_port_t reply_port;

        // Allocate reply port
        kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &reply_port);
        if (kr != KERN_SUCCESS) {
            printf("[CLIENT] Failed to allocate reply port\n");
            exit(1);
        }
        // Note: No need to insert send-once right explicitly - we use MACH_MSG_TYPE_MAKE_SEND_ONCE
        // in mach_msg which creates the send-once right inline from our receive right

        // Prepare read request
        memset(&req, 0, sizeof(req));
        req.header.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, MACH_MSG_TYPE_MAKE_SEND_ONCE);
        req.header.msgh_remote_port = server_port;
        req.header.msgh_local_port = reply_port;
        req.header.msgh_id = MSG_ID_READ_REQUEST;
        req.header.msgh_size = sizeof(mach_msg_header_t) + 2 * sizeof(int);
        req.offset = 0;
        req.length = 100;

        printf("[CLIENT] Sending read request...\n");
        kr = mach_msg(&req.header, MACH_SEND_MSG, req.header.msgh_size, 
                      0, MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
        if (kr != KERN_SUCCESS) {
            printf("[CLIENT] Failed to send request: %s\n", mach_error_string(kr));
            exit(1);
        }
        printf("[CLIENT] Request sent, waiting for reply...\n");

        // Receive reply
        memset(&reply, 0, sizeof(reply));
        kr = mach_msg(&reply.header, MACH_RCV_MSG, 0, sizeof(reply), 
                      reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
        if (kr != KERN_SUCCESS) {
            printf("[CLIENT] Failed to receive reply: %s\n", mach_error_string(kr));
            exit(1);
        }

        printf("[CLIENT] Received reply!\n");
        printf("         Bytes read: %d\n", reply.bytes_read);
        printf("         Data: \"%s\"\n", reply.data);

        printf("\n=== SUCCESS: XNU Mach Client-Server RPC works! ===\n");
        exit(0);

    } else {
        // === SERVER PROCESS ===
        printf("[SERVER] Entering message loop (waiting for 1 request)...\n");

        // Receive one message and handle it
        read_request_t req;
        memset(&req, 0, sizeof(req));

        kr = mach_msg(&req.header, MACH_RCV_MSG, 0, sizeof(req), 
                      server_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
        if (kr != KERN_SUCCESS) {
            printf("[SERVER] Failed to receive message: %s\n", mach_error_string(kr));
            return 1;
        }

        printf("[SERVER] Received message ID: %d\n", req.header.msgh_id);

        if (req.header.msgh_id == MSG_ID_READ_REQUEST) {
            printf("[SERVER] Handling read request: offset=%d, length=%d\n", 
                   req.offset, req.length);
            handle_read_request(&req, req.header.msgh_remote_port);
            printf("[SERVER] Reply sent.\n");
        }

        // Wait for child
        int status;
        waitpid(pid, &status, 0);

        printf("\n[SERVER] Demo complete.\n");
    }

    return 0;
}
