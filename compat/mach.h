/*
 * mach.h - XNU compatibility shim for Hurd
 * 
 * Hurd expects <mach.h> to include all basic Mach definitions.
 * On macOS, we include the XNU Mach headers.
 */
#ifndef _COMPAT_MACH_H
#define _COMPAT_MACH_H

#include <mach/mach.h>
#include <mach/mach_error.h>
#include <mach/message.h>
#include <mach/port.h>
#include <mach/mach_port.h>
#include <mach/vm_map.h>
#include <mach/mach_vm.h>
#include <mach/thread_act.h>
#include <mach/task.h>
#include <pthread.h>

/* Types that Hurd expects but XNU lacks or names differently */
/* mach_msg_seqno_t exists in Hurd's Mach but not XNU's mach.h directly */
typedef unsigned int mach_msg_seqno_t;

/* macOS doesn't have pthread_spinlock_t - use rwlock as a workaround */
#ifndef PTHREAD_SPINLOCK_COMPAT
#define PTHREAD_SPINLOCK_COMPAT
typedef pthread_mutex_t pthread_spinlock_t;
#define pthread_spin_init(lock, shared) pthread_mutex_init(lock, NULL)
#define pthread_spin_destroy(lock) pthread_mutex_destroy(lock)
#define pthread_spin_lock(lock) pthread_mutex_lock(lock)
#define pthread_spin_unlock(lock) pthread_mutex_unlock(lock)
#define PTHREAD_SPINLOCK_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#endif

/* Hurd uses error_t which is int */
typedef int error_t;

/* Hurd-specific error codes that may not exist on XNU */
#ifndef EDIED
#define EDIED 1000  /* Placeholder for Hurd specific error */
#endif

/* Hurd port type definitions for MIG-generated code */
typedef mach_port_t io_t;
typedef mach_port_t file_t;
typedef mach_port_t fsys_t;
typedef mach_port_t process_t;
typedef mach_port_t auth_t;
typedef mach_port_t socket_t;
typedef mach_port_t pf_t;
typedef mach_port_t addr_port_t;
typedef mach_port_t term_t;
typedef mach_port_t startup_t;
typedef mach_port_t interrupt_t;
typedef mach_port_t fs_notify_t;
typedef mach_port_t exec_startup_t;

/* Additional types for MIG-generated code */
typedef char *data_t;
typedef unsigned int *intarray_t;
typedef unsigned int *idarray_t;
typedef int retry_type;
typedef char *string_t;

/* fsys_statfsbuf_t - simplified statfs buffer */
typedef struct {
    unsigned int f_type;
    unsigned long f_bsize;
    unsigned long long f_blocks;
    unsigned long long f_bfree;
    unsigned long long f_bavail;
    unsigned long long f_files;
    unsigned long long f_ffree;
    unsigned long long f_fsid;
    unsigned long f_namelen;
    unsigned long long f_favail;
    unsigned long f_frsize;
    unsigned long f_flag;
} fsys_statfsbuf_t;

#endif /* _COMPAT_MACH_H */
