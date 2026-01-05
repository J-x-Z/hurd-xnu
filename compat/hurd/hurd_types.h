/*
 * hurd/hurd_types.h - Minimal stub for XNU compatibility
 * 
 * This is a placeholder. The real hurd_types.h defines various C types.
 * For MIG processing, we just need the basic type names.
 */
#ifndef _HURD_TYPES_H
#define _HURD_TYPES_H

#include <sys/types.h>
#include <mach/mach_types.h>

/* Placeholder typedefs - will be fleshed out as needed */
typedef int retry_type;
typedef int file_changed_type_t;
typedef int dir_changed_type_t;

#endif /* _HURD_TYPES_H */
