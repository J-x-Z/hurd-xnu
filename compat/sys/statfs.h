/*
 * sys/statfs.h - XNU compatibility shim for Hurd
 * 
 * macOS doesn't have sys/statfs.h (it uses sys/mount.h for statfs).
 * Provide minimal compatibility.
 */
#ifndef _SYS_STATFS_H
#define _SYS_STATFS_H

#include <sys/mount.h>

#endif /* _SYS_STATFS_H */
