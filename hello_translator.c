/*
 * hello_translator.c - A Hurd-style "Hello World" Translator for XNU
 * 
 * This is a standalone translator that responds to read requests with
 * "Hello, World from Hurd on XNU!\n"
 * 
 * Build: gcc -I./compat -include compat/mach.h -D__extern_inline='static inline' \
 *        -o hello_translator hello_translator.c -L./compat -lports_xnu -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  /* for usleep */
#include <pthread.h>
#include <mach/mach.h>
#include <mach/message.h>
#include <mach/ndr.h>  /* for NDR_record */

/* Hurd-style message IDs (from io.defs subsystem 21000) */
#define HURD_IO_READ     21000
#define HURD_IO_READABLE 21003

/* Our content */
static const char *hello_content = "Hello, World from Hurd on XNU!\n";
static size_t hello_content_len;

/* Server state */
static mach_port_t server_port;
static volatile int running = 1;

/* Generic request message */
typedef struct {
    mach_msg_header_t header;
    NDR_record_t NDR;
    union {
        struct {
            int64_t offset;
            uint32_t amount;
        } read;
    } args;
    mach_msg_trailer_t trailer;
} request_msg_t;

/* Read reply message */
typedef struct {
    mach_msg_header_t header;
    mach_msg_body_t body;
    mach_msg_ool_descriptor_t data_desc;
    NDR_record_t NDR;
    uint32_t data_len;
} read_reply_t;

/* Simple reply (for io_readable, etc.) */
typedef struct {
    mach_msg_header_t header;
    NDR_record_t NDR;
    kern_return_t retcode;
    uint32_t value;
} simple_reply_t;

/*
 * Handle io_read request
 */
static kern_return_t handle_io_read(request_msg_t *req, mach_port_t reply_port)
{
    int64_t offset = req->args.read.offset;
    uint32_t amount = req->args.read.amount;
    
    printf("[TRANSLATOR] io_read: offset=%lld, amount=%u\n", offset, amount);
    
    /* Calculate how much data to return */
    size_t available = 0;
    if (offset < (int64_t)hello_content_len) {
        available = hello_content_len - offset;
    }
    size_t to_send = (amount < available) ? amount : available;
    
    /* Build reply */
    read_reply_t reply;
    memset(&reply, 0, sizeof(reply));
    
    reply.header.msgh_bits = MACH_MSGH_BITS_COMPLEX | 
                             MACH_MSGH_BITS(MACH_MSG_TYPE_MOVE_SEND_ONCE, 0);
    reply.header.msgh_size = sizeof(reply);
    reply.header.msgh_remote_port = reply_port;
    reply.header.msgh_local_port = MACH_PORT_NULL;
    reply.header.msgh_id = HURD_IO_READ + 100;  /* Reply ID convention */
    
    reply.body.msgh_descriptor_count = 1;
    reply.data_desc.address = (void *)(hello_content + offset);
    reply.data_desc.size = (mach_msg_size_t)to_send;
    reply.data_desc.deallocate = FALSE;
    reply.data_desc.copy = MACH_MSG_VIRTUAL_COPY;
    reply.data_desc.type = MACH_MSG_OOL_DESCRIPTOR;
    
    reply.NDR = NDR_record;
    reply.data_len = (uint32_t)to_send;
    
    kern_return_t kr = mach_msg(&reply.header, MACH_SEND_MSG,
                                sizeof(reply), 0, MACH_PORT_NULL,
                                MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
    
    if (kr != KERN_SUCCESS) {
        printf("[TRANSLATOR] mach_msg send failed: %s\n", mach_error_string(kr));
    } else {
        printf("[TRANSLATOR] Sent %zu bytes\n", to_send);
    }
    
    return kr;
}

/*
 * Handle io_readable request - return how many bytes can be read
 */
static kern_return_t handle_io_readable(request_msg_t *req, mach_port_t reply_port)
{
    printf("[TRANSLATOR] io_readable\n");
    
    simple_reply_t reply;
    memset(&reply, 0, sizeof(reply));
    
    reply.header.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_MOVE_SEND_ONCE, 0);
    reply.header.msgh_size = sizeof(reply);
    reply.header.msgh_remote_port = reply_port;
    reply.header.msgh_local_port = MACH_PORT_NULL;
    reply.header.msgh_id = HURD_IO_READABLE + 100;
    
    reply.NDR = NDR_record;
    reply.retcode = KERN_SUCCESS;
    reply.value = (uint32_t)hello_content_len;
    
    return mach_msg(&reply.header, MACH_SEND_MSG,
                    sizeof(reply), 0, MACH_PORT_NULL,
                    MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
}

/*
 * Main server loop
 */
static void *server_thread(void *arg)
{
    request_msg_t req;
    kern_return_t kr;
    
    printf("[TRANSLATOR] Server thread started on port %d\n", server_port);
    
    while (running) {
        memset(&req, 0, sizeof(req));
        
        kr = mach_msg(&req.header, MACH_RCV_MSG, 0, sizeof(req),
                      server_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
        
        if (kr != KERN_SUCCESS) {
            printf("[TRANSLATOR] mach_msg receive failed: %s\n", mach_error_string(kr));
            continue;
        }
        
        mach_port_t reply_port = req.header.msgh_remote_port;
        
        printf("[TRANSLATOR] Received message ID: %d\n", req.header.msgh_id);
        
        switch (req.header.msgh_id) {
            case HURD_IO_READ:
                handle_io_read(&req, reply_port);
                break;
            case HURD_IO_READABLE:
                handle_io_readable(&req, reply_port);
                break;
            default:
                printf("[TRANSLATOR] Unknown message ID: %d\n", req.header.msgh_id);
                /* Send error reply */
                break;
        }
    }
    
    return NULL;
}

/*
 * Client test function
 */
static void test_client(void)
{
    /* Wait for server to start */
    usleep(100000);
    
    printf("\n[CLIENT] Testing io_readable...\n");
    
    /* Prepare io_readable request */
    struct {
        mach_msg_header_t header;
    } readable_req;
    
    struct {
        mach_msg_header_t header;
        NDR_record_t NDR;
        kern_return_t retcode;
        uint32_t amount;
        mach_msg_trailer_t trailer;
    } readable_reply;
    
    /* Get a reply port */
    mach_port_t reply_port;
    mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &reply_port);
    mach_port_insert_right(mach_task_self(), reply_port, reply_port, MACH_MSG_TYPE_MAKE_SEND_ONCE);
    
    readable_req.header.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, 
                                                   MACH_MSG_TYPE_MAKE_SEND_ONCE);
    readable_req.header.msgh_size = sizeof(readable_req);
    readable_req.header.msgh_remote_port = server_port;
    readable_req.header.msgh_local_port = reply_port;
    readable_req.header.msgh_id = HURD_IO_READABLE;
    
    kern_return_t kr = mach_msg(&readable_req.header, 
                                MACH_SEND_MSG | MACH_RCV_MSG,
                                sizeof(readable_req), sizeof(readable_reply),
                                reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
    
    if (kr == KERN_SUCCESS) {
        printf("[CLIENT] io_readable returned: %u bytes available\n", readable_reply.amount);
    } else {
        printf("[CLIENT] io_readable failed: %s\n", mach_error_string(kr));
    }
    
    /* Now test io_read */
    printf("\n[CLIENT] Testing io_read...\n");
    
    struct {
        mach_msg_header_t header;
        NDR_record_t NDR;
        int64_t offset;
        uint32_t amount;
    } read_req;
    
    struct {
        mach_msg_header_t header;
        mach_msg_body_t body;
        mach_msg_ool_descriptor_t data_desc;
        NDR_record_t NDR;
        uint32_t data_len;
        mach_msg_trailer_t trailer;
    } read_reply;
    
    mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &reply_port);
    
    read_req.header.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, 
                                               MACH_MSG_TYPE_MAKE_SEND_ONCE);
    read_req.header.msgh_size = sizeof(read_req);
    read_req.header.msgh_remote_port = server_port;
    read_req.header.msgh_local_port = reply_port;
    read_req.header.msgh_id = HURD_IO_READ;
    read_req.NDR = NDR_record;
    read_req.offset = 0;
    read_req.amount = 100;
    
    memset(&read_reply, 0, sizeof(read_reply));
    
    kr = mach_msg(&read_req.header,
                  MACH_SEND_MSG | MACH_RCV_MSG,
                  sizeof(read_req), sizeof(read_reply),
                  reply_port, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
    
    if (kr == KERN_SUCCESS) {
        printf("[CLIENT] io_read returned %u bytes: \"%.*s\"\n", 
               read_reply.data_len,
               read_reply.data_len, (char *)read_reply.data_desc.address);
        
        /* Deallocate OOL data */
        if (read_reply.data_desc.address) {
            vm_deallocate(mach_task_self(), 
                          (vm_address_t)read_reply.data_desc.address,
                          read_reply.data_desc.size);
        }
    } else {
        printf("[CLIENT] io_read failed: %s\n", mach_error_string(kr));
    }
    
    printf("\n=== Hello Translator Test Complete! ===\n");
    running = 0;
}

int main(int argc, char *argv[])
{
    kern_return_t kr;
    
    printf("=== Hurd-style Hello Translator on XNU ===\n\n");
    
    hello_content_len = strlen(hello_content);
    
    /* Allocate server port */
    kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &server_port);
    if (kr != KERN_SUCCESS) {
        fprintf(stderr, "Failed to allocate port: %s\n", mach_error_string(kr));
        return 1;
    }
    
    /* Insert send right for client */
    kr = mach_port_insert_right(mach_task_self(), server_port, server_port, 
                                MACH_MSG_TYPE_MAKE_SEND);
    if (kr != KERN_SUCCESS) {
        fprintf(stderr, "Failed to insert send right: %s\n", mach_error_string(kr));
        return 1;
    }
    
    printf("[MAIN] Translator port: %d\n", server_port);
    printf("[MAIN] Content: \"%s\" (%zu bytes)\n\n", hello_content, hello_content_len);
    
    /* Start server thread */
    pthread_t server_tid;
    pthread_create(&server_tid, NULL, server_thread, NULL);
    
    /* Run test client */
    test_client();
    
    /* Wait for server to finish */
    pthread_join(server_tid, NULL);
    
    printf("\n[MAIN] Translator shut down.\n");
    return 0;
}
