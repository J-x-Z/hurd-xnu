/* ihash_stub.c - Minimal ihash implementation for auth server */

#include <stdlib.h>
#include <errno.h>
#include "mach.h"
#include "hurd/ihash.h"

/* Initialize an ihash table */
void
hurd_ihash_init(hurd_ihash_t ht, intptr_t locp_offs)
{
    ht->nr_items = 0;
    ht->size = 0;
    ht->locp_offset = locp_offs;
    ht->max_load = HURD_IHASH_MAX_LOAD_DEFAULT;
    ht->cleanup = NULL;
    ht->fct_hash = NULL;
    ht->fct_cmp = NULL;
    ht->nr_free = 0;
    ht->items = NULL;
}

/* Destroy ihash */
void
hurd_ihash_destroy(hurd_ihash_t ht)
{
    if (ht->items)
        free(ht->items);
    ht->items = NULL;
    ht->size = 0;
}

/* Stubs for libports */
void _ports_complete_deallocate(void *p) { }
void _ports_threadpool_init(void) { }
int ports_default_uninhibitable_rpcs = 0;
