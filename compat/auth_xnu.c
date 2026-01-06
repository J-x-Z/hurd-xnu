/* auth_xnu.c - XNU-compatible Hurd auth server
   Based on GNU Hurd auth/auth.c
   Simplified for XNU Mach compatibility */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <mach/mach.h>
#include <pthread.h>
#include <errno.h>

#include "mach.h"
#include "ports.h"
#include "idvec.h"
#include "hurd/ihash.h"
#include "assert-backtrace.h"

/* MIG-generated headers */
#include "authServer.h"

/* Auth handles are server ports with sets of ids */
struct authhandle {
    struct port_info pi;
    struct idvec euids, egids, auids, agids;
};

static struct port_bucket *auth_bucket;
static struct port_class *authhandle_portclass;

/* Create a new auth port */
static error_t
create_authhandle(struct authhandle **new)
{
    error_t err = ports_create_port(authhandle_portclass, auth_bucket,
                                    sizeof(**new), new);
    if (!err) {
        memset(&(*new)->euids, 0, sizeof(struct idvec) * 4);
    }
    return err;
}

/* Clean up a dead auth port */
static void
destroy_authhandle(void *p)
{
    struct authhandle *h = p;
    idvec_free_contents(&h->euids);
    idvec_free_contents(&h->egids);
    idvec_free_contents(&h->auids);
    idvec_free_contents(&h->agids);
}

/* Helper to copy out idvec */
static inline void
idvec_copyout(struct idvec *idvec, uid_t **ids, mach_msg_type_number_t *nids)
{
    if (idvec->num > *nids)
        *ids = idvec->ids;
    else if (idvec->num)
        memcpy(*ids, idvec->ids, idvec->num * sizeof(uid_t));
    *nids = idvec->num;
}

#define C(auth, ids) idvec_copyout(&auth->ids, ids, n##ids)
#define OUTIDS(auth) (C(auth, euids), C(auth, egids), C(auth, auids), C(auth, agids))

/* Convert port to authhandle - simplified version */
static struct authhandle *
auth_port_to_handle(mach_port_t port)
{
    /* In full implementation, this would look up the port in port_bucket */
    /* For now, return NULL (requires libports integration) */
    return NULL;
}

/* Implement auth_getids */
kern_return_t
auth_getids(mach_port_t handle,
            uid_t **euids, mach_msg_type_number_t *neuids,
            uid_t **auids, mach_msg_type_number_t *nauids,
            uid_t **egids, mach_msg_type_number_t *negids,
            uid_t **agids, mach_msg_type_number_t *nagids)
{
    /* TODO: Look up handle in port table */
    /* For now, return empty ids for demonstration */
    *neuids = 0;
    *nauids = 0;
    *negids = 0;
    *nagids = 0;
    return 0;
}

/* Implement auth_makeauth */
kern_return_t
auth_makeauth(mach_port_t handle,
              mach_port_t *other_handles, mach_msg_type_number_t nauths,
              uid_t *euids, mach_msg_type_number_t neuids,
              uid_t *auids, mach_msg_type_number_t nauids,
              uid_t *egids, mach_msg_type_number_t negids,
              uid_t *agids, mach_msg_type_number_t nagids,
              mach_port_t *newhandle)
{
    struct authhandle *newauth;
    error_t err;
    
    err = create_authhandle(&newauth);
    if (err)
        return err;
    
    /* Merge the requested ids into the new handle */
    idvec_merge_ids(&newauth->euids, euids, neuids);
    idvec_merge_ids(&newauth->auids, auids, nauids);
    idvec_merge_ids(&newauth->egids, egids, negids);
    idvec_merge_ids(&newauth->agids, agids, nagids);
    
    *newhandle = ports_get_right(newauth);
    ports_port_deref(newauth);
    
    return 0;
}

/* Implement auth_user_authenticate - simplified */
kern_return_t
auth_user_authenticate(mach_port_t handle,
                       mach_port_t rendezvous,
                       mach_port_t *newport)
{
    /* Full implementation requires pthread_hurd_cond_wait_np */
    /* For now, return not supported */
    if (!MACH_PORT_VALID(rendezvous))
        return EINVAL;
    
    *newport = MACH_PORT_NULL;
    return EOPNOTSUPP;
}

/* Implement auth_server_authenticate - simplified */
kern_return_t
auth_server_authenticate(mach_port_t handle,
                         mach_port_t rendezvous,
                         mach_port_t newport,
                         uid_t **euids, mach_msg_type_number_t *neuids,
                         uid_t **auids, mach_msg_type_number_t *nauids,
                         uid_t **egids, mach_msg_type_number_t *negids,
                         uid_t **agids, mach_msg_type_number_t *nagids)
{
    /* Full implementation requires pthread_hurd_cond_wait_np */
    /* For now, return not supported */
    if (!MACH_PORT_VALID(rendezvous))
        return EINVAL;
    
    *neuids = 0;
    *nauids = 0;
    *negids = 0;
    *nagids = 0;
    return EOPNOTSUPP;
}

/* Message demuxer */
static int
auth_demuxer(mach_msg_header_t *inp, mach_msg_header_t *outp)
{
    mig_routine_t routine;
    if ((routine = auth_server_routine(inp))) {
        (*routine)(inp, outp);
        return 1;
    }
    return 0;
}

/* Initialize auth server */
int
auth_server_init(void)
{
    error_t err;
    struct authhandle *firstauth;
    
    auth_bucket = ports_create_bucket();
    if (!auth_bucket)
        return -1;
    
    authhandle_portclass = ports_create_class(&destroy_authhandle, NULL);
    if (!authhandle_portclass)
        return -1;
    
    /* Create the initial root auth handle */
    err = create_authhandle(&firstauth);
    if (err)
        return -1;
    
    /* Add root uid/gid */
    idvec_add(&firstauth->euids, 0);
    idvec_add(&firstauth->auids, 0);
    idvec_add(&firstauth->egids, 0);
    idvec_add(&firstauth->agids, 0);
    
    printf("Auth server initialized with root handle\n");
    return 0;
}

/* Run auth server message loop - stub for testing */
int
auth_server_run(void)
{
    /* Full implementation requires ports_manage_port_operations_multithread */
    /* For testing, just return success */
    printf("auth_server_run: Ready for message loop (stub)\n");
    return 0;
}

#ifdef AUTH_TEST
/* Test program */
int main(int argc, char *argv[])
{
    printf("=== Auth Server Test ===\n");
    
    if (auth_server_init() < 0) {
        printf("FAILED: auth_server_init\n");
        return 1;
    }
    printf("PASSED: auth_server_init\n");
    
    /* Test idvec operations */
    struct idvec test_vec;
    idvec_init(&test_vec);
    idvec_add(&test_vec, 1000);
    idvec_add(&test_vec, 1001);
    
    if (idvec_contains(&test_vec, 1000) && idvec_contains(&test_vec, 1001)) {
        printf("PASSED: idvec operations\n");
    } else {
        printf("FAILED: idvec operations\n");
    }
    
    idvec_free_contents(&test_vec);
    
    printf("=== Auth Server Test Complete ===\n");
    return 0;
}
#endif
