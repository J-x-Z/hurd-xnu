#include <mach/mach.h>
#include <mach/message.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a simple structure for the message
// Note: On 64-bit systems, natural alignment is important.
typedef struct {
    mach_msg_header_t header;
    int data; // Body of the message
    mach_msg_trailer_t trailer; // Trailer for receive
} msg_t;

int main() {
    mach_port_t port;
    kern_return_t kr;
    msg_t send_msg, rcv_msg;

    printf("=== XNU Mach IPC Verification ===\n");

    // 1. Allocate a Receive Right
    // mach_task_self() is a built-in port to the current task
    kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &port);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] Failed to allocate port: %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] Port allocated: %u\n", port);

    // 2. Insert a Send Right (so we can send to ourselves)
    kr = mach_port_insert_right(mach_task_self(), port, port, MACH_MSG_TYPE_MAKE_SEND);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] Failed to insert send right: %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] Send right inserted.\n");

    // 3. Prepare message to send
    memset(&send_msg, 0, sizeof(send_msg));
    // Header setup
    // Remote port = destination (our new port)
    // Local port = reply port (none for this test)
    send_msg.header.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, 0);
    send_msg.header.msgh_remote_port = port;
    send_msg.header.msgh_local_port = MACH_PORT_NULL;
    send_msg.header.msgh_id = 999;
    
    // Size = Header + Body (User data)
    // IMPORTANT: mach_msg_header_t size is included.
    // We send header + int data.
    send_msg.header.msgh_size = sizeof(mach_msg_header_t) + sizeof(int);
    send_msg.data = 0xDEADBEEF;

    printf("[INFO] Sending message (ID: %d, Data: 0x%X)...\n", send_msg.header.msgh_id, send_msg.data);

    // 4. Send Message
    kr = mach_msg(&send_msg.header, 
                  MACH_SEND_MSG, 
                  send_msg.header.msgh_size, 
                  0, // rcv size (0 for send)
                  MACH_PORT_NULL, // rcv name
                  MACH_MSG_TIMEOUT_NONE, 
                  MACH_PORT_NULL);

    if (kr != KERN_SUCCESS) {
        printf("[ERROR] Failed to send message: %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] Message sent.\n");

    // 5. Receive Message
    memset(&rcv_msg, 0, sizeof(rcv_msg));
    printf("[INFO] Receiving message...\n");

    kr = mach_msg(&rcv_msg.header,
                  MACH_RCV_MSG,
                  0, // send size
                  sizeof(rcv_msg), // rcv size (buffer size)
                  port, // receive from this port
                  MACH_MSG_TIMEOUT_NONE,
                  MACH_PORT_NULL);

    if (kr != KERN_SUCCESS) {
        printf("[ERROR] Failed to receive message: %s\n", mach_error_string(kr));
        return 1;
    }

    printf("[OK] Message received.\n");
    printf("      ID: %d\n", rcv_msg.header.msgh_id);
    printf("      Data: 0x%X\n", rcv_msg.data);

    if (rcv_msg.data == 0xDEADBEEF) {
        printf("SUCCESS: Data verified!\n");
    } else {
        printf("FAILURE: Data mismatch.\n");
        return 1;
    }

    return 0;
}
