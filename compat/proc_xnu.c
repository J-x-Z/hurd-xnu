/* proc_xnu.c - XNU-compatible Hurd process server
   Simplified implementation for XNU Mach */

#include <stdlib.h>
#include <string.h>
#include <mach/mach.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

#include "mach.h"
#include "ports.h"
#include "idvec.h"

/* MIG-generated header */
#include "processServer.h"

/* Process table entry */
struct proc_entry {
    pid_t pid;
    pid_t ppid;
    task_t task;
    int orphaned;
    int exited;
    int exit_status;
};

/* Simple process table */
#define MAX_PROCS 256
static struct proc_entry proc_table[MAX_PROCS];
static int proc_count = 0;
static pthread_mutex_t proc_lock = PTHREAD_MUTEX_INITIALIZER;

static struct port_bucket *proc_bucket;
static struct port_class *proc_portclass;

/* Initialize proc server */
int proc_server_init(void)
{
    proc_bucket = ports_create_bucket();
    if (!proc_bucket) return -1;
    
    proc_portclass = ports_create_class(NULL, NULL);
    if (!proc_portclass) return -1;
    
    /* Add init process (pid 1) */
    proc_table[0].pid = 1;
    proc_table[0].ppid = 0;
    proc_table[0].task = mach_task_self();
    proc_count = 1;
    
    printf("Proc server initialized\n");
    return 0;
}

/* 24004: Get privileged ports */
kern_return_t
proc_getprivports(mach_port_t process,
                  mach_port_t *host_priv,
                  mach_port_t *device_master)
{
    /* On XNU, can't actually get these without root */
    *host_priv = MACH_PORT_NULL;
    *device_master = MACH_PORT_NULL;
    return 0;
}

/* 24005: Get all PIDs */
kern_return_t
proc_getallpids(mach_port_t process,
                pid_t **pidarray,
                mach_msg_type_number_t *pidarrayCnt)
{
    pthread_mutex_lock(&proc_lock);
    
    *pidarrayCnt = proc_count;
    *pidarray = malloc(proc_count * sizeof(pid_t));
    if (!*pidarray) {
        pthread_mutex_unlock(&proc_lock);
        return ENOMEM;
    }
    
    for (int i = 0; i < proc_count; i++) {
        (*pidarray)[i] = proc_table[i].pid;
    }
    
    pthread_mutex_unlock(&proc_lock);
    return 0;
}

/* 24015: Get PIDs */
kern_return_t
proc_getpids(mach_port_t process,
             pid_t *pid, pid_t *ppid, int *orphaned)
{
    /* Return current process info */
    *pid = getpid();
    *ppid = getppid();
    *orphaned = 0;
    return 0;
}

/* 24019: Wait for child */
kern_return_t
proc_wait(mach_port_t process,
          pid_t pid, int options,
          int *status, int *sigcode,
          pid_t *pid_status)
{
    int stat;
    pid_t result = waitpid(pid, &stat, options);
    
    if (result < 0) {
        *status = 0;
        *sigcode = 0;
        *pid_status = 0;
        return errno;
    }
    
    *status = stat;
    *sigcode = WTERMSIG(stat);
    *pid_status = result;
    return 0;
}

/* 24028: PID to task */
kern_return_t
proc_pid2task(mach_port_t process,
              pid_t pid,
              mach_port_t *task)
{
    pthread_mutex_lock(&proc_lock);
    
    for (int i = 0; i < proc_count; i++) {
        if (proc_table[i].pid == pid) {
            *task = proc_table[i].task;
            pthread_mutex_unlock(&proc_lock);
            return 0;
        }
    }
    
    pthread_mutex_unlock(&proc_lock);
    *task = MACH_PORT_NULL;
    return ESRCH;
}

/* 24029: Task to PID */
kern_return_t
proc_task2pid(mach_port_t process,
              mach_port_t task,
              pid_t *pid)
{
    pthread_mutex_lock(&proc_lock);
    
    for (int i = 0; i < proc_count; i++) {
        if (proc_table[i].task == task) {
            *pid = proc_table[i].pid;
            pthread_mutex_unlock(&proc_lock);
            return 0;
        }
    }
    
    pthread_mutex_unlock(&proc_lock);
    *pid = 0;
    return ESRCH;
}

#ifdef PROC_TEST
int main(int argc, char *argv[])
{
    printf("=== Proc Server Test ===\n");
    
    if (proc_server_init() < 0) {
        printf("FAILED: proc_server_init\n");
        return 1;
    }
    printf("PASSED: proc_server_init\n");
    
    /* Test proc_getpids */
    pid_t pid, ppid;
    int orphaned;
    if (proc_getpids(MACH_PORT_NULL, &pid, &ppid, &orphaned) == 0) {
        printf("PASSED: proc_getpids (pid=%d, ppid=%d)\n", pid, ppid);
    } else {
        printf("FAILED: proc_getpids\n");
    }
    
    /* Test proc_getallpids */
    pid_t *pids;
    mach_msg_type_number_t count;
    if (proc_getallpids(MACH_PORT_NULL, &pids, &count) == 0) {
        printf("PASSED: proc_getallpids (count=%d)\n", count);
        free(pids);
    } else {
        printf("FAILED: proc_getallpids\n");
    }
    
    printf("=== Proc Server Test Complete ===\n");
    return 0;
}
#endif
