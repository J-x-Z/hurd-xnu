/*
 * Test compilation of libports/init.c with XNU compat layer
 */
#include <stdio.h>

/* Pull in our compatibility headers */
#include "mach.h"
#include "hurd.h"
#include "refcount.h"
#include "hurd/ihash.h"

/* Now test that we can define the basic libports types */
#include <pthread.h>

/* From libports/ports.h - simplified for testing */
struct port_info {
    void *port_class;
    refcounts_t refcounts;
    mach_port_mscount_t mscount;
    int flags;
    mach_port_t port_right;
    void *current_rpcs;
    void *bucket;
    hurd_ihash_locp_t hentry;
    hurd_ihash_locp_t ports_htable_entry;
};

struct ports_threadpool {
    int dummy;
};

struct port_bucket {
    mach_port_t portset;
    struct hurd_ihash htable;
    int rpcs;
    int flags;
    int count;
    struct ports_threadpool threadpool;
    struct port_info *notify_port;
};

/* Test globals from init.c */
pthread_mutex_t _ports_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t _ports_block = PTHREAD_COND_INITIALIZER;

struct hurd_ihash _ports_htable =
  HURD_IHASH_INITIALIZER (offsetof (struct port_info, ports_htable_entry));
pthread_rwlock_t _ports_htable_lock = PTHREAD_RWLOCK_INITIALIZER;

int _ports_total_rpcs;
int _ports_flags;

int main() {
    printf("=== libports Compilation Test ===\n");
    printf("[OK] mach.h included\n");
    printf("[OK] hurd.h included\n");
    printf("[OK] refcount.h included\n");
    printf("[OK] hurd/ihash.h included\n");
    printf("[OK] port_info struct defined (%zu bytes)\n", sizeof(struct port_info));
    printf("[OK] port_bucket struct defined (%zu bytes)\n", sizeof(struct port_bucket));
    printf("[OK] _ports_htable initialized\n");
    printf("\nSUCCESS: Basic libports structures compile on XNU!\n");
    return 0;
}
