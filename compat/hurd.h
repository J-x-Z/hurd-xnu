/*
 * hurd.h - Minimal XNU compatibility shim for Hurd
 * 
 * Provides basic Hurd type definitions that other Hurd code expects.
 */
#ifndef _COMPAT_HURD_H
#define _COMPAT_HURD_H

#include <mach/mach.h>
#include <sys/types.h>

/* error_t is just int */
typedef int error_t;

/* Hurd uses thread_t as a Mach thread port */
/* XNU already defines thread_t in <mach/mach_types.h> */

/* Basic success/failure */
#ifndef KERN_SUCCESS
#define KERN_SUCCESS 0
#endif

/* Common Hurd error placeholder */
#ifndef EDIED
#define EDIED 1000
#endif

/* Placeholder for Hurd's hurd_thread_cancel - we'll implement later */
static inline error_t hurd_thread_cancel(thread_t thread) {
    return thread_abort(thread);
}

#endif /* _COMPAT_HURD_H */
