#ifndef _DEVICE_DEVICE_H_
#define _DEVICE_DEVICE_H_

#include <mach/mach.h>

typedef mach_port_t device_t;
typedef int dev_mode_t;
typedef int dev_flavor_t;
typedef int dev_status_t;
typedef int recnum_t;
typedef int io_buf_ptr_t;

#define D_READ  0x1
#define D_WRITE 0x2
#define D_NODELAY 0x4
#define D_NOWAIT 0x8

#define D_SUCCESS 0
#define D_IO_ERROR 2500
#define D_WOULD_BLOCK 2501
#define D_NO_SUCH_DEVICE 2502
#define D_ALREADY_OPEN 2503
#define D_DEVICE_DOWN 2504
#define D_INVALID_OPERATION 2505
#define D_INVALID_RECNUM 2506
#define D_INVALID_SIZE 2507

/* Status flavors */
#define DEV_GET_SIZE 3
#define DEV_GET_RECORDS 4
#define DEV_GET_SIZE_RECORD_SIZE 0
#define DEV_GET_SIZE_DEVICE_SIZE 1
#define DEV_GET_RECORDS_RECORD_SIZE 0
#define DEV_GET_RECORDS_DEVICE_RECORDS 1

#endif
