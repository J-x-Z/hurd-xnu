/*
 * ports.h - XNU compatibility version of libports header
 * 
 * This is a simplified version for testing compilation on macOS.
 */
#ifndef _HURD_PORTS_
#define _HURD_PORTS_

#include "mach.h"
#include <stdlib.h>
#include "hurd.h"
#include "hurd/ihash.h"
#include <mach/notify.h>
#include <pthread.h>
#include "refcount.h"

/* Forward declarations */
struct port_info;
struct port_bucket;
struct port_class;
struct rpc_info;

/* Threadpool (simplified) */
struct ports_threadpool {
    pthread_t thread;
    int active;
};

static inline void _ports_threadpool_init(struct ports_threadpool *pool) {
    pool->thread = 0;
    pool->active = 0;
}

/* Port info structure */
struct port_info
{
    struct port_class *class;
    refcounts_t refcounts;
    mach_port_mscount_t mscount;
    mach_msg_seqno_t cancel_threshold;
    int flags;
    mach_port_t port_right;
    struct rpc_info *current_rpcs;
    struct port_bucket *bucket;
    hurd_ihash_locp_t hentry;
    hurd_ihash_locp_t ports_htable_entry;
};
typedef struct port_info *port_info_t;

/* Port bucket structure */
struct port_bucket
{
    mach_port_t portset;
    struct hurd_ihash htable;
    int rpcs;
    int flags;
    int count;
    struct ports_threadpool threadpool;
    struct port_info *notify_port;
};

/* Port class structure */
struct port_class
{
    int flags;
    int rpcs;
    int count;
    void (*clean_routine) (void *);
    void (*dropweak_routine) (void *);
    void *uninhibitable_rpcs;
};

/* RPC info structure */
struct rpc_info
{
    thread_t thread;
    struct rpc_info *next, **prevp;
    void *notifies;
    struct rpc_info *interrupted_next;
};

/* Demuxer type */
typedef int (*ports_demuxer_type)(mach_msg_header_t *inp, mach_msg_header_t *outp);

/* Function prototypes */
struct port_bucket *ports_create_bucket (void);
struct port_class *ports_create_class (void (*clean_routine)(void *),
                                       void (*dropweak_routine)(void *));
error_t ports_create_port (struct port_class *port_class,
                           struct port_bucket *bucket,
                           size_t size, void *result);
void *ports_lookup_port (struct port_bucket *bucket,
                         mach_port_t port, struct port_class *port_class);
void ports_port_ref (void *port);
void ports_port_deref (void *port);
mach_port_t ports_get_right (void *port);

/* Globals */
extern pthread_mutex_t _ports_lock;
extern pthread_cond_t _ports_block;
extern struct hurd_ihash _ports_htable;
extern pthread_rwlock_t _ports_htable_lock;
extern int _ports_total_rpcs;
extern int _ports_flags;

#endif /* _HURD_PORTS_ */
