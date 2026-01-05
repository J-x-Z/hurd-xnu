/*
 * mach_portset_test.c
 * 
 * Tests Mach Port Sets - a critical feature for Hurd servers.
 * Hurd servers typically create a "port set" and add multiple receive rights to it,
 * then use a single mach_msg() call to receive from any of them.
 * 
 * This pattern is the core of Hurd's event loop.
 */

#include <mach/mach.h>
#include <mach/message.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    mach_msg_header_t header;
    int data;
    mach_msg_trailer_t trailer;
} simple_msg_t;

// Sender thread: sends messages to two different ports
void *sender_thread(void *arg) {
    mach_port_t *ports = (mach_port_t *)arg;
    mach_port_t port1 = ports[0];
    mach_port_t port2 = ports[1];
    kern_return_t kr;
    simple_msg_t msg;

    usleep(100000); // 100ms delay to let receiver set up

    // Send to port1
    memset(&msg, 0, sizeof(msg));
    msg.header.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, 0);
    msg.header.msgh_remote_port = port1;
    msg.header.msgh_local_port = MACH_PORT_NULL;
    msg.header.msgh_id = 1001;
    msg.header.msgh_size = sizeof(mach_msg_header_t) + sizeof(int);
    msg.data = 0xAAAA;

    kr = mach_msg(&msg.header, MACH_SEND_MSG, msg.header.msgh_size, 0, 
                  MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
    if (kr != KERN_SUCCESS) {
        printf("[SENDER] Failed to send to port1: %s\n", mach_error_string(kr));
    } else {
        printf("[SENDER] Sent msg ID=1001, data=0xAAAA to port1\n");
    }

    usleep(50000); // 50ms

    // Send to port2
    memset(&msg, 0, sizeof(msg));
    msg.header.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, 0);
    msg.header.msgh_remote_port = port2;
    msg.header.msgh_local_port = MACH_PORT_NULL;
    msg.header.msgh_id = 1002;
    msg.header.msgh_size = sizeof(mach_msg_header_t) + sizeof(int);
    msg.data = 0xBBBB;

    kr = mach_msg(&msg.header, MACH_SEND_MSG, msg.header.msgh_size, 0, 
                  MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
    if (kr != KERN_SUCCESS) {
        printf("[SENDER] Failed to send to port2: %s\n", mach_error_string(kr));
    } else {
        printf("[SENDER] Sent msg ID=1002, data=0xBBBB to port2\n");
    }

    return NULL;
}

int main() {
    kern_return_t kr;
    mach_port_t port1, port2, port_set;

    printf("=== XNU Mach Port Set Verification ===\n");
    printf("This tests Hurd-style message receive loops.\n\n");

    // 1. Create two receive rights
    kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &port1);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] port1 allocate: %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] port1 allocated: %u\n", port1);

    kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &port2);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] port2 allocate: %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] port2 allocated: %u\n", port2);

    // 2. Create a Port Set
    kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_PORT_SET, &port_set);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] port_set allocate: %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] port_set allocated: %u\n", port_set);

    // 3. Add both ports to the Port Set using mach_port_move_member
    //    This is a CRITICAL Hurd pattern!
    kr = mach_port_move_member(mach_task_self(), port1, port_set);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] mach_port_move_member(port1): %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] port1 added to port_set\n");

    kr = mach_port_move_member(mach_task_self(), port2, port_set);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] mach_port_move_member(port2): %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] port2 added to port_set\n");

    // 4. Insert send rights so we can send to ourselves
    kr = mach_port_insert_right(mach_task_self(), port1, port1, MACH_MSG_TYPE_MAKE_SEND);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] insert send right port1: %s\n", mach_error_string(kr));
        return 1;
    }
    kr = mach_port_insert_right(mach_task_self(), port2, port2, MACH_MSG_TYPE_MAKE_SEND);
    if (kr != KERN_SUCCESS) {
        printf("[ERROR] insert send right port2: %s\n", mach_error_string(kr));
        return 1;
    }
    printf("[OK] Send rights inserted for both ports\n\n");

    // 5. Start sender thread
    mach_port_t ports[2] = {port1, port2};
    pthread_t sender;
    pthread_create(&sender, NULL, sender_thread, ports);

    // 6. Receive from Port Set (should get messages from either port)
    printf("[RECEIVER] Listening on port_set for 2 messages...\n");
    int received_count = 0;
    int received_1001 = 0, received_1002 = 0;

    while (received_count < 2) {
        simple_msg_t rcv_msg;
        memset(&rcv_msg, 0, sizeof(rcv_msg));

        kr = mach_msg(&rcv_msg.header, MACH_RCV_MSG, 0, sizeof(rcv_msg),
                      port_set, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
        if (kr != KERN_SUCCESS) {
            printf("[ERROR] Receive failed: %s\n", mach_error_string(kr));
            return 1;
        }

        printf("[RECEIVER] Got message: ID=%d, data=0x%X, from local_port=%u\n",
               rcv_msg.header.msgh_id, rcv_msg.data, rcv_msg.header.msgh_local_port);

        if (rcv_msg.header.msgh_id == 1001 && rcv_msg.data == 0xAAAA) received_1001 = 1;
        if (rcv_msg.header.msgh_id == 1002 && rcv_msg.data == 0xBBBB) received_1002 = 1;
        received_count++;
    }

    pthread_join(sender, NULL);

    printf("\n=== Results ===\n");
    if (received_1001 && received_1002) {
        printf("SUCCESS: Port Set multiplexing works!\n");
        printf("This confirms XNU supports Hurd-style message loops.\n");
        return 0;
    } else {
        printf("FAILURE: Did not receive expected messages.\n");
        return 1;
    }
}
