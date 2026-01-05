/*
 * bits/utsname.h - XNU compatibility shim for Hurd
 * 
 * This provides the _UTSNAME_LENGTH definition that Hurd's hurd_types.defs expects.
 * On glibc, this comes from bits/utsname.h; on macOS, we define it ourselves.
 */
#ifndef _BITS_UTSNAME_H
#define _BITS_UTSNAME_H

/* Match the size used in macOS's sys/utsname.h */
#define _UTSNAME_LENGTH 256

#endif /* _BITS_UTSNAME_H */
