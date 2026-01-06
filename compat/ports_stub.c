/* ports_stub.c - Minimal libports stubs for auth server testing
   These are the missing functions that weren't in libports_xnu.a */

#include <stdlib.h>
#include <mach/mach.h>
#include "mach.h"
#include "ports.h"

/* Stub implementation of ports_create_port */
error_t
ports_create_port(struct port_class *class,
                  struct port_bucket *bucket,
                  size_t size,
                  void *result)
{
    void **res = (void **)result;
    *res = calloc(1, size);
    if (!*res)
        return ENOMEM;
    
    /* Initialize port_info */
    struct port_info *pi = *res;
    pi->bucket = bucket;
    pi->class = class;
    
    kern_return_t kr = mach_port_allocate(mach_task_self(),
                                          MACH_PORT_RIGHT_RECEIVE,
                                          &pi->port_right);
    if (kr != KERN_SUCCESS) {
        free(*res);
        *res = NULL;
        return kr;
    }
    
    refcounts_init(&pi->refcounts, 1, 0);
    return 0;
}

/* Get the right from a port */
mach_port_t
ports_get_right(void *port)
{
    struct port_info *pi = port;
    return pi->port_right;
}

/* Deref a port */
void
ports_port_deref(void *port)
{
    struct port_info *pi = port;
    struct references refs;
    refcounts_deref(&pi->refcounts, &refs);
    if (refs.hard == 0) {
        mach_port_deallocate(mach_task_self(), pi->port_right);
        free(port);
    }
}

/* Create port bucket */
struct port_bucket *
ports_create_bucket(void)
{
    struct port_bucket *bucket = calloc(1, sizeof(struct port_bucket));
    if (!bucket)
        return NULL;
    
    kern_return_t kr = mach_port_allocate(mach_task_self(),
                                          MACH_PORT_RIGHT_PORT_SET,
                                          &bucket->portset);
    if (kr != KERN_SUCCESS) {
        free(bucket);
        return NULL;
    }
    
    return bucket;
}

/* Create port class */
struct port_class *
ports_create_class(void (*clean_routine)(void *),
                   void (*dropweak_routine)(void *))
{
    struct port_class *class = calloc(1, sizeof(struct port_class));
    if (!class)
        return NULL;
    
    class->clean_routine = clean_routine;
    class->dropweak_routine = dropweak_routine;
    return class;
}
