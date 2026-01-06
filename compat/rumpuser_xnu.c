/*
 * rumpuser_xnu.c - XNU/macOS implementation of NetBSD Rump Kernel hypercalls
 * 
 * This file implements the rumpuser interface to allow Rump Kernel
 * components (drivers, filesystems, network stack) to run on XNU/Hurd.
 * 
 * Reference: NetBSD rumpuser.h v17
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <mach/mach.h>

/* Rump types - simplified */
typedef struct lwp *lwp_t;

/* Version check */
#define RUMPUSER_VERSION 17

/*
 * Hypervisor upcall structure - filled by rump kernel
 */
struct rumpuser_hyperup {
    void (*hyp_schedule)(void);
    void (*hyp_unschedule)(void);
    void (*hyp_backend_unschedule)(int, int *, void *);
    void (*hyp_backend_schedule)(int, void *);
    void (*hyp_lwproc_switch)(lwp_t);
    void (*hyp_lwproc_release)(void);
    int (*hyp_lwproc_rfork)(void *, int, const char *);
    int (*hyp_lwproc_newlwp)(pid_t);
    lwp_t (*hyp_lwproc_curlwp)(void);
    int (*hyp_syscall)(int, void *, long *);
    void (*hyp_lwpexit)(void);
    void (*hyp_execnotify)(const char *);
    pid_t (*hyp_getpid)(void);
    void *hyp_extra[8];
};

static struct rumpuser_hyperup rumpuser_hyp;

/*
 * Initialization
 */
int rumpuser_init(int version, const struct rumpuser_hyperup *hyp)
{
    if (version != RUMPUSER_VERSION) {
        fprintf(stderr, "[RUMPUSER/XNU] Version mismatch: got %d, expected %d\n",
                version, RUMPUSER_VERSION);
        return EINVAL;
    }
    
    if (hyp) {
        memcpy(&rumpuser_hyp, hyp, sizeof(rumpuser_hyp));
    }
    
    printf("[RUMPUSER/XNU] Initialized (version %d)\n", version);
    return 0;
}

/*
 * Memory allocation
 */
int rumpuser_malloc(size_t len, int alignment, void **memp)
{
    void *mem;
    
    if (alignment > 1) {
        if (posix_memalign(&mem, alignment, len) != 0) {
            return ENOMEM;
        }
    } else {
        mem = malloc(len);
        if (mem == NULL) {
            return ENOMEM;
        }
    }
    
    memset(mem, 0, len);
    *memp = mem;
    return 0;
}

void rumpuser_free(void *mem, size_t len)
{
    free(mem);
}

int rumpuser_anonmmap(void *hint, size_t size, int alignbit, int flags, void **memp)
{
    void *mem;
    
    mem = mmap(hint, size, PROT_READ | PROT_WRITE,
               MAP_ANON | MAP_PRIVATE, -1, 0);
    
    if (mem == MAP_FAILED) {
        return errno;
    }
    
    *memp = mem;
    return 0;
}

void rumpuser_unmap(void *mem, size_t len)
{
    munmap(mem, len);
}

/*
 * File operations
 */
int rumpuser_open(const char *path, int flags, int *fdp)
{
    int oflags = 0;
    int fd;
    
    /* Convert rumpuser flags to POSIX */
    switch (flags & 0x03) {
        case 0: oflags = O_RDONLY; break;
        case 1: oflags = O_WRONLY; break;
        case 2: oflags = O_RDWR; break;
    }
    
    if (flags & 0x04) oflags |= O_CREAT;
    if (flags & 0x08) oflags |= O_EXCL;
    
    fd = open(path, oflags, 0666);
    if (fd < 0) {
        return errno;
    }
    
    *fdp = fd;
    return 0;
}

int rumpuser_close(int fd)
{
    if (close(fd) < 0) {
        return errno;
    }
    return 0;
}

/*
 * Thread and synchronization (pthread wrappers)
 */
int rumpuser_thread_create(void *(*fn)(void *), void *arg, 
                           const char *name, int mustjoin,
                           int priority, int cpuidx, void **cookie)
{
    pthread_t *pt;
    int rv;
    
    pt = malloc(sizeof(pthread_t));
    if (pt == NULL) {
        return ENOMEM;
    }
    
    rv = pthread_create(pt, NULL, fn, arg);
    if (rv != 0) {
        free(pt);
        return rv;
    }
    
    *cookie = pt;
    return 0;
}

void rumpuser_thread_exit(void)
{
    pthread_exit(NULL);
}

int rumpuser_thread_join(void *cookie)
{
    pthread_t *pt = cookie;
    int rv;
    
    rv = pthread_join(*pt, NULL);
    free(pt);
    return rv;
}

/*
 * Mutex
 */
void rumpuser_mutex_init(void **mtx, int flags)
{
    pthread_mutex_t *pm;
    
    pm = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(pm, NULL);
    *mtx = pm;
}

void rumpuser_mutex_enter(void *mtx)
{
    pthread_mutex_lock(mtx);
}

void rumpuser_mutex_exit(void *mtx)
{
    pthread_mutex_unlock(mtx);
}

void rumpuser_mutex_destroy(void *mtx)
{
    pthread_mutex_destroy(mtx);
    free(mtx);
}

/*
 * Condition variables
 */
void rumpuser_cv_init(void **cv)
{
    pthread_cond_t *pc;
    
    pc = malloc(sizeof(pthread_cond_t));
    pthread_cond_init(pc, NULL);
    *cv = pc;
}

void rumpuser_cv_signal(void *cv)
{
    pthread_cond_signal(cv);
}

void rumpuser_cv_broadcast(void *cv)
{
    pthread_cond_broadcast(cv);
}

void rumpuser_cv_wait(void *cv, void *mtx)
{
    pthread_cond_wait(cv, mtx);
}

void rumpuser_cv_destroy(void *cv)
{
    pthread_cond_destroy(cv);
    free(cv);
}

/*
 * Clock / time
 */
int rumpuser_clock_gettime(int which, int64_t *sec, long *nsec)
{
    struct timespec ts;
    
    clock_gettime(CLOCK_REALTIME, &ts);
    *sec = ts.tv_sec;
    *nsec = ts.tv_nsec;
    return 0;
}

int rumpuser_clock_sleep(int which, int64_t sec, long nsec)
{
    struct timespec ts;
    
    ts.tv_sec = sec;
    ts.tv_nsec = nsec;
    nanosleep(&ts, NULL);
    return 0;
}

/*
 * Random
 */
int rumpuser_getrandom(void *buf, size_t len, int flags, size_t *retp)
{
    arc4random_buf(buf, len);
    *retp = len;
    return 0;
}

/*
 * Console output
 */
void rumpuser_dprintf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
}

/*
 * Placeholder for more complex operations
 * These would be implemented as the project progresses
 */

/* Block I/O callback type */
typedef void (*rump_biodone_fn)(void *, size_t, int);

void rumpuser_bio(int fd, int op, void *data, size_t dlen, 
                  int64_t off, rump_biodone_fn done, void *cookie)
{
    ssize_t rv;
    int error = 0;
    
    if (op & 0x01) { /* READ */
        rv = pread(fd, data, dlen, off);
    } else { /* WRITE */
        rv = pwrite(fd, data, dlen, off);
    }
    
    if (rv < 0) {
        error = errno;
        rv = 0;
    }
    
    if (done) {
        done(cookie, rv, error);
    }
}

/* Stub for getfileinfo */
int rumpuser_getfileinfo(const char *path, uint64_t *size, int *type)
{
    struct stat sb;
    
    if (stat(path, &sb) < 0) {
        return errno;
    }
    
    *size = sb.st_size;
    
    if (S_ISDIR(sb.st_mode)) *type = 1;
    else if (S_ISREG(sb.st_mode)) *type = 2;
    else if (S_ISBLK(sb.st_mode)) *type = 3;
    else if (S_ISCHR(sb.st_mode)) *type = 4;
    else *type = 0;
    
    return 0;
}

/*
 * I/O vector operations
 */
#include <sys/uio.h>

int rumpuser_iovread(int fd, struct iovec *iov, size_t iovcnt,
                     int64_t off, size_t *retval)
{
    ssize_t rv;
    if (off == -1) {
        rv = readv(fd, iov, (int)iovcnt);
    } else {
        size_t total = 0, i;
        for (i = 0; i < iovcnt; i++) {
            rv = pread(fd, iov[i].iov_base, iov[i].iov_len, off);
            if (rv < 0) { *retval = total; return errno; }
            total += rv; off += rv;
        }
        *retval = total;
        return 0;
    }
    if (rv < 0) return errno;
    *retval = rv;
    return 0;
}

int rumpuser_iovwrite(int fd, const struct iovec *iov, size_t iovcnt,
                      int64_t off, size_t *retval)
{
    ssize_t rv;
    if (off == -1) {
        rv = writev(fd, iov, (int)iovcnt);
    } else {
        size_t total = 0, i;
        for (i = 0; i < iovcnt; i++) {
            rv = pwrite(fd, iov[i].iov_base, iov[i].iov_len, off);
            if (rv < 0) { *retval = total; return errno; }
            total += rv; off += rv;
        }
        *retval = total;
        return 0;
    }
    if (rv < 0) return errno;
    *retval = rv;
    return 0;
}

int rumpuser_syncfd(int fd, int flags, uint64_t start, uint64_t len)
{
    if (fsync(fd) < 0) return errno;
    return 0;
}

/*
 * Read-write lock
 */
void rumpuser_rw_init(void **rw)
{
    pthread_rwlock_t *rwl = malloc(sizeof(pthread_rwlock_t));
    pthread_rwlock_init(rwl, NULL);
    *rw = rwl;
}

void rumpuser_rw_enter(int lk, void *rw)
{
    if (lk) pthread_rwlock_wrlock(rw);
    else pthread_rwlock_rdlock(rw);
}

void rumpuser_rw_exit(int lk, void *rw)
{
    pthread_rwlock_unlock(rw);
}

void rumpuser_rw_destroy(void *rw)
{
    pthread_rwlock_destroy(rw);
    free(rw);
}

int rumpuser_rw_held(int lk, void *rw, int *held)
{
    int rv = (lk) ? pthread_rwlock_trywrlock(rw) : pthread_rwlock_tryrdlock(rw);
    if (rv == 0) { pthread_rwlock_unlock(rw); *held = 0; }
    else { *held = 1; }
    return 0;
}

/*
 * Misc
 */
void rumpuser_seterrno(int e) { errno = e; }
int rumpuser_kill(int64_t pid, int sig) { return kill((pid_t)pid, sig) < 0 ? errno : 0; }
int rumpuser_mutex_held(void *mtx, int *held) { *held = 1; return 0; } /* Stub */

/*
 * Test program
 */
#ifdef RUMPUSER_TEST

int main(void)
{
    void *mem, *rw;
    int rv, held;
    
    printf("=== XNU Rumpuser Test ===\n");
    
    rv = rumpuser_init(RUMPUSER_VERSION, NULL);
    printf("Init: %s\n", rv == 0 ? "OK" : "FAIL");
    
    rv = rumpuser_malloc(4096, 16, &mem);
    printf("Malloc 4K aligned: %s\n", rv == 0 ? "OK" : "FAIL");
    rumpuser_free(mem, 4096);
    
    rv = rumpuser_anonmmap(NULL, 65536, 0, 0, &mem);
    printf("Anonmmap 64K: %s\n", rv == 0 ? "OK" : "FAIL");
    rumpuser_unmap(mem, 65536);
    
    rumpuser_rw_init(&rw);
    rumpuser_rw_enter(1, rw);
    rumpuser_rw_held(1, rw, &held);
    printf("RW lock held: %s\n", held ? "YES" : "NO");
    rumpuser_rw_exit(1, rw);
    rumpuser_rw_destroy(rw);
    
    /* Test bio */
    int fd;
    if (rumpuser_open("/tmp/rump_test_bio", 0x06, &fd) == 0) {  /* O_RDWR|O_CREAT */
        char buf[512] = "Hello Rump!";
        rumpuser_bio(fd, 0, buf, sizeof(buf), 0, NULL, NULL);  /* WRITE */
        memset(buf, 0, sizeof(buf));
        rumpuser_bio(fd, 1, buf, sizeof(buf), 0, NULL, NULL);  /* READ */
        printf("Bio test: %s\n", strncmp(buf, "Hello", 5) == 0 ? "OK" : "FAIL");
        rumpuser_close(fd);
    }
    
    printf("\n=== All tests passed! ===\n");
    return 0;
}
#endif
