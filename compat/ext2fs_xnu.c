/* ext2fs_xnu.c - XNU-compatible Hurd ext2fs translator
   Simplified filesystem translator using POSIX filesystem calls */

#include <stdlib.h>
#include <string.h>
#include <mach/mach.h>
#include <pthread.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#include "mach.h"
#include "ports.h"

/* MIG-generated header */
#include "filesystemServer.h"

/* File handle table */
#define MAX_FILES 256
static int file_fds[MAX_FILES];
static char file_paths[MAX_FILES][256];
static int file_count = 0;
static pthread_mutex_t fs_lock = PTHREAD_MUTEX_INITIALIZER;

static struct port_bucket *fs_bucket;
static struct port_class *fs_portclass;

/* Helpers */
static int port_to_fd(mach_port_t port) {
    unsigned int idx = (unsigned int)port & 0xFF;
    if (idx < MAX_FILES && file_fds[idx] >= 0)
        return file_fds[idx];
    return -1;
}

static mach_port_t fd_to_port(int fd) {
    pthread_mutex_lock(&fs_lock);
    int idx = file_count++ % MAX_FILES;
    file_fds[idx] = fd;
    pthread_mutex_unlock(&fs_lock);
    return (mach_port_t)(idx | 0x200);
}

/* Initialize ext2fs server */
int ext2fs_server_init(void)
{
    fs_bucket = ports_create_bucket();
    if (!fs_bucket) return -1;
    
    fs_portclass = ports_create_class(NULL, NULL);
    if (!fs_portclass) return -1;
    
    memset(file_fds, -1, sizeof(file_fds));
    
    printf("Ext2fs server initialized\n");
    return 0;
}

/* 22000: Directory lookup */
kern_return_t
dir_lookup(mach_port_t dir,
           char *name, mach_msg_type_number_t nameCnt,
           int flags, int mode,
           int *do_retry,
           char **retry_name, mach_msg_type_number_t *retry_nameCnt,
           mach_port_t *file)
{
    int dir_fd = port_to_fd(dir);
    if (dir_fd < 0) {
        /* Assume root lookup */
        dir_fd = AT_FDCWD;
    }
    
    char path[256];
    snprintf(path, sizeof(path), "%.*s", (int)nameCnt, name);
    
    int fd = openat(dir_fd, path, flags | O_RDONLY);
    if (fd < 0) {
        *file = MACH_PORT_NULL;
        *do_retry = 0;
        *retry_nameCnt = 0;
        return errno;
    }
    
    *file = fd_to_port(fd);
    *do_retry = 0;
    *retry_nameCnt = 0;
    return 0;
}

/* 22001: Read directory */
kern_return_t
dir_readdir(mach_port_t dir,
            char **data, mach_msg_type_number_t *dataCnt,
            int entry, int nentries, int bufsiz,
            int *amount)
{
    int fd = port_to_fd(dir);
    if (fd < 0) return EBADF;
    
    DIR *dp = fdopendir(dup(fd));
    if (!dp) return errno;
    
    *data = malloc(bufsiz);
    if (!*data) {
        closedir(dp);
        return ENOMEM;
    }
    
    char *ptr = *data;
    int bytes_used = 0;
    int entries_read = 0;
    
    struct dirent *de;
    while ((de = readdir(dp)) && entries_read < nentries) {
        size_t len = strlen(de->d_name) + 1;
        if (bytes_used + len > (size_t)bufsiz) break;
        
        memcpy(ptr, de->d_name, len);
        ptr += len;
        bytes_used += len;
        entries_read++;
    }
    
    closedir(dp);
    *dataCnt = bytes_used;
    *amount = entries_read;
    return 0;
}

/* 22002: Make directory */
kern_return_t
dir_mkdir(mach_port_t dir,
          char *name, mach_msg_type_number_t nameCnt,
          int mode)
{
    int dir_fd = port_to_fd(dir);
    if (dir_fd < 0) dir_fd = AT_FDCWD;
    
    char path[256];
    snprintf(path, sizeof(path), "%.*s", (int)nameCnt, name);
    
    if (mkdirat(dir_fd, path, mode) < 0)
        return errno;
    return 0;
}

/* 22003: Remove directory */
kern_return_t
dir_rmdir(mach_port_t dir,
          char *name, mach_msg_type_number_t nameCnt)
{
    int dir_fd = port_to_fd(dir);
    if (dir_fd < 0) dir_fd = AT_FDCWD;
    
    char path[256];
    snprintf(path, sizeof(path), "%.*s", (int)nameCnt, name);
    
    if (unlinkat(dir_fd, path, AT_REMOVEDIR) < 0)
        return errno;
    return 0;
}

/* 22004: Unlink file */
kern_return_t
dir_unlink(mach_port_t dir,
           char *name, mach_msg_type_number_t nameCnt)
{
    int dir_fd = port_to_fd(dir);
    if (dir_fd < 0) dir_fd = AT_FDCWD;
    
    char path[256];
    snprintf(path, sizeof(path), "%.*s", (int)nameCnt, name);
    
    if (unlinkat(dir_fd, path, 0) < 0)
        return errno;
    return 0;
}

/* 22005: Link */
kern_return_t
dir_link(mach_port_t dir,
         mach_port_t source,
         char *name, mach_msg_type_number_t nameCnt,
         int excl)
{
    /* Hard links require source path - simplified stub */
    return EOPNOTSUPP;
}

/* 22006: Rename */
kern_return_t
dir_rename(mach_port_t olddir,
           char *oldname, mach_msg_type_number_t oldnameCnt,
           mach_port_t newdir,
           char *newname, mach_msg_type_number_t newnameCnt,
           int excl)
{
    int old_fd = port_to_fd(olddir);
    int new_fd = port_to_fd(newdir);
    if (old_fd < 0) old_fd = AT_FDCWD;
    if (new_fd < 0) new_fd = AT_FDCWD;
    
    char opath[256], npath[256];
    snprintf(opath, sizeof(opath), "%.*s", (int)oldnameCnt, oldname);
    snprintf(npath, sizeof(npath), "%.*s", (int)newnameCnt, newname);
    
    if (renameat(old_fd, opath, new_fd, npath) < 0)
        return errno;
    return 0;
}

/* 22007: Make file */
kern_return_t
dir_mkfile(mach_port_t dir, int flags, int mode, mach_port_t *file)
{
    int dir_fd = port_to_fd(dir);
    if (dir_fd < 0) dir_fd = AT_FDCWD;
    
    /* Create temp file */
    char temp[] = "/tmp/hurd_fileXXXXXX";
    int fd = mkstemp(temp);
    if (fd < 0) {
        *file = MACH_PORT_NULL;
        return errno;
    }
    
    fchmod(fd, mode);
    *file = fd_to_port(fd);
    return 0;
}

/* 22008: Get stat */
kern_return_t
file_getstat(mach_port_t file,
             char **stat_data, mach_msg_type_number_t *stat_dataCnt)
{
    int fd = port_to_fd(file);
    if (fd < 0) return EBADF;
    
    struct stat st;
    if (fstat(fd, &st) < 0)
        return errno;
    
    *stat_data = malloc(sizeof(st));
    if (!*stat_data) return ENOMEM;
    
    memcpy(*stat_data, &st, sizeof(st));
    *stat_dataCnt = sizeof(st);
    return 0;
}

/* 22009: Set stat */
kern_return_t
file_setstat(mach_port_t file,
             char *stat_data, mach_msg_type_number_t stat_dataCnt,
             int stat_flags)
{
    int fd = port_to_fd(file);
    if (fd < 0) return EBADF;
    
    /* Partial implementation - just chmod */
    struct stat *st = (struct stat *)stat_data;
    if (stat_flags & 0x01) /* mode */ {
        fchmod(fd, st->st_mode);
    }
    return 0;
}

/* 22010: Sync */
kern_return_t
file_sync(mach_port_t file, int wait, int omit_metadata)
{
    int fd = port_to_fd(file);
    if (fd < 0) return EBADF;
    
    if (omit_metadata)
        fdatasync(fd);
    else
        fsync(fd);
    return 0;
}

/* 22011: Set size */
kern_return_t
file_set_size(mach_port_t file, int64_t size)
{
    int fd = port_to_fd(file);
    if (fd < 0) return EBADF;
    
    if (ftruncate(fd, size) < 0)
        return errno;
    return 0;
}

/* 22015: Read */
kern_return_t
io_read(mach_port_t io,
        char **data, mach_msg_type_number_t *dataCnt,
        int64_t offset, int amount)
{
    int fd = port_to_fd(io);
    if (fd < 0) return EBADF;
    
    *data = malloc(amount);
    if (!*data) return ENOMEM;
    
    ssize_t n = pread(fd, *data, amount, offset);
    if (n < 0) {
        free(*data);
        *data = NULL;
        *dataCnt = 0;
        return errno;
    }
    *dataCnt = n;
    return 0;
}

/* 22016: Write */
kern_return_t
io_write(mach_port_t io,
         char *data, mach_msg_type_number_t dataCnt,
         int64_t offset, int *amount)
{
    int fd = port_to_fd(io);
    if (fd < 0) return EBADF;
    
    ssize_t n = pwrite(fd, data, dataCnt, offset);
    if (n < 0) {
        *amount = 0;
        return errno;
    }
    *amount = (int)n;
    return 0;
}

#ifdef EXT2FS_TEST
int main(int argc, char *argv[])
{
    printf("=== Ext2fs Server Test ===\n");
    
    if (ext2fs_server_init() < 0) {
        printf("FAILED: ext2fs_server_init\n");
        return 1;
    }
    printf("PASSED: ext2fs_server_init\n");
    
    /* Test dir_mkfile */
    mach_port_t file;
    if (dir_mkfile(MACH_PORT_NULL, 0, 0644, &file) == 0) {
        printf("PASSED: dir_mkfile\n");
        
        /* Test io_write */
        const char *testdata = "Hello, Hurd!";
        int amount;
        if (io_write(file, (char*)testdata, strlen(testdata), 0, &amount) == 0) {
            printf("PASSED: io_write (%d bytes)\n", amount);
        } else {
            printf("FAILED: io_write\n");
        }
        
        /* Test io_read */
        char *readdata;
        mach_msg_type_number_t readCnt;
        if (io_read(file, &readdata, &readCnt, 0, 256) == 0) {
            printf("PASSED: io_read (%d bytes: %.*s)\n", 
                   readCnt, readCnt, readdata);
            free(readdata);
        } else {
            printf("FAILED: io_read\n");
        }
    } else {
        printf("FAILED: dir_mkfile\n");
    }
    
    printf("=== Ext2fs Server Test Complete ===\n");
    return 0;
}
#endif
