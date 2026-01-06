/* machdev_xnu.c - Mach device layer compatibility for XNU
   Bridges Hurd's libmachdev to XNU's IOKit/device APIs */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mach/mach.h>

#include "mach.h"
#include "ports.h"

/*
 * Mach device structure - represents a device in Hurd's mach_device layer
 */
struct machdev {
    char *name;
    mach_port_t port;
    int fd;                     /* Backend file descriptor */
    size_t size;                /* Device size */
    size_t block_size;          /* Block size (usually 512) */
    void *priv;                 /* Driver private data */
    
    /* Device operations */
    int (*dev_open)(struct machdev *, int flags);
    int (*dev_close)(struct machdev *);
    int (*dev_read)(struct machdev *, void *buf, size_t len, off_t off, size_t *resid);
    int (*dev_write)(struct machdev *, const void *buf, size_t len, off_t off, size_t *resid);
    int (*dev_ioctl)(struct machdev *, unsigned long cmd, void *arg);
};

/* Device table */
#define MAX_DEVICES 64
static struct machdev *devices[MAX_DEVICES];
static int device_count = 0;
static pthread_mutex_t dev_lock = PTHREAD_MUTEX_INITIALIZER;

static struct port_bucket *machdev_bucket;
static struct port_class *machdev_class;

/*
 * Initialize machdev layer
 */
int machdev_init(void)
{
    machdev_bucket = ports_create_bucket();
    if (!machdev_bucket) return -1;
    
    machdev_class = ports_create_class(NULL, NULL);
    if (!machdev_class) return -1;
    
    printf("[MACHDEV/XNU] Initialized\n");
    return 0;
}

/*
 * Register a new device
 */
int machdev_register(const char *name, struct machdev *dev)
{
    pthread_mutex_lock(&dev_lock);
    
    if (device_count >= MAX_DEVICES) {
        pthread_mutex_unlock(&dev_lock);
        return ENOMEM;
    }
    
    dev->name = strdup(name);
    devices[device_count++] = dev;
    
    printf("[MACHDEV/XNU] Registered device: %s\n", name);
    
    pthread_mutex_unlock(&dev_lock);
    return 0;
}

/*
 * Find device by name
 */
struct machdev *machdev_lookup(const char *name)
{
    pthread_mutex_lock(&dev_lock);
    
    for (int i = 0; i < device_count; i++) {
        if (strcmp(devices[i]->name, name) == 0) {
            pthread_mutex_unlock(&dev_lock);
            return devices[i];
        }
    }
    
    pthread_mutex_unlock(&dev_lock);
    return NULL;
}

/*
 * Simple file-backed device implementation
 * Used as a backend for testing and disk images
 */
struct file_device {
    int fd;
    size_t size;
};

static int file_dev_open(struct machdev *dev, int flags)
{
    /* Already opened via create */
    return 0;
}

static int file_dev_close(struct machdev *dev)
{
    struct file_device *fd_dev = dev->priv;
    if (fd_dev && fd_dev->fd >= 0) {
        close(fd_dev->fd);
        fd_dev->fd = -1;
    }
    return 0;
}

static int file_dev_read(struct machdev *dev, void *buf, size_t len, 
                         off_t off, size_t *resid)
{
    struct file_device *fd_dev = dev->priv;
    ssize_t n = pread(fd_dev->fd, buf, len, off);
    if (n < 0) return errno;
    *resid = len - n;
    return 0;
}

static int file_dev_write(struct machdev *dev, const void *buf, size_t len,
                          off_t off, size_t *resid)
{
    struct file_device *fd_dev = dev->priv;
    ssize_t n = pwrite(fd_dev->fd, buf, len, off);
    if (n < 0) return errno;
    *resid = len - n;
    return 0;
}

static int file_dev_ioctl(struct machdev *dev, unsigned long cmd, void *arg)
{
    struct file_device *fd_dev = dev->priv;
    
    /* DIOCGMEDIASIZE - get media size (NetBSD/Rump ioctl) */
    if (cmd == 0x40086418) {  /* DIOCGMEDIASIZE */
        *(uint64_t *)arg = fd_dev->size;
        return 0;
    }
    
    return ENOTTY;
}

/*
 * Create a file-backed block device
 */
int machdev_create_file_device(const char *name, const char *path)
{
    struct machdev *dev;
    struct file_device *fd_dev;
    struct stat sb;
    int fd;
    
    fd = open(path, O_RDWR);
    if (fd < 0) {
        fd = open(path, O_RDONLY);
        if (fd < 0) return errno;
    }
    
    if (fstat(fd, &sb) < 0) {
        close(fd);
        return errno;
    }
    
    dev = calloc(1, sizeof(struct machdev));
    if (!dev) { close(fd); return ENOMEM; }
    
    fd_dev = calloc(1, sizeof(struct file_device));
    if (!fd_dev) { free(dev); close(fd); return ENOMEM; }
    
    fd_dev->fd = fd;
    fd_dev->size = sb.st_size;
    
    dev->priv = fd_dev;
    dev->size = sb.st_size;
    dev->block_size = 512;
    dev->dev_open = file_dev_open;
    dev->dev_close = file_dev_close;
    dev->dev_read = file_dev_read;
    dev->dev_write = file_dev_write;
    dev->dev_ioctl = file_dev_ioctl;
    
    return machdev_register(name, dev);
}

/*
 * MIG RPC handlers for device operations
 * These would be called from the device MIG stubs
 */
kern_return_t
device_open(mach_port_t master, const char *name, int flags, mach_port_t *device)
{
    struct machdev *dev = machdev_lookup(name);
    if (!dev) return ENODEV;
    
    if (dev->dev_open) {
        int err = dev->dev_open(dev, flags);
        if (err) return err;
    }
    
    *device = dev->port;
    return 0;
}

kern_return_t
device_read(mach_port_t device, int mode, off_t offset,
            char **data, mach_msg_type_number_t *dataCnt, size_t bytes_wanted)
{
    /* Find device by port - simplified lookup */
    struct machdev *dev = NULL;
    for (int i = 0; i < device_count; i++) {
        if (devices[i]->port == device) {
            dev = devices[i];
            break;
        }
    }
    if (!dev || !dev->dev_read) return ENODEV;
    
    *data = malloc(bytes_wanted);
    if (!*data) return ENOMEM;
    
    size_t resid = 0;
    int err = dev->dev_read(dev, *data, bytes_wanted, offset, &resid);
    if (err) {
        free(*data);
        return err;
    }
    
    *dataCnt = bytes_wanted - resid;
    return 0;
}

kern_return_t
device_write(mach_port_t device, int mode, off_t offset,
             char *data, mach_msg_type_number_t dataCnt, int *bytes_written)
{
    struct machdev *dev = NULL;
    for (int i = 0; i < device_count; i++) {
        if (devices[i]->port == device) {
            dev = devices[i];
            break;
        }
    }
    if (!dev || !dev->dev_write) return ENODEV;
    
    size_t resid = 0;
    int err = dev->dev_write(dev, data, dataCnt, offset, &resid);
    if (err) return err;
    
    *bytes_written = dataCnt - resid;
    return 0;
}

#ifdef MACHDEV_TEST
int main(int argc, char *argv[])
{
    printf("=== Machdev Test ===\n");
    
    if (machdev_init() < 0) {
        printf("FAILED: machdev_init\n");
        return 1;
    }
    printf("PASSED: machdev_init\n");
    
    /* Create a test file device */
    system("dd if=/dev/zero of=/tmp/test_device.img bs=1M count=1 2>/dev/null");
    
    if (machdev_create_file_device("hd0", "/tmp/test_device.img") == 0) {
        printf("PASSED: machdev_create_file_device\n");
        
        struct machdev *dev = machdev_lookup("hd0");
        if (dev) {
            printf("PASSED: machdev_lookup (size=%zu)\n", dev->size);
            
            /* Test read/write */
            char buf[512] = "Hurd on XNU!";
            size_t resid;
            
            if (dev->dev_write(dev, buf, 512, 0, &resid) == 0) {
                printf("PASSED: dev_write\n");
            }
            
            memset(buf, 0, 512);
            if (dev->dev_read(dev, buf, 512, 0, &resid) == 0) {
                printf("PASSED: dev_read (data: %.12s)\n", buf);
            }
            
            dev->dev_close(dev);
        }
    } else {
        printf("FAILED: machdev_create_file_device\n");
    }
    
    unlink("/tmp/test_device.img");
    printf("=== Machdev Test Complete ===\n");
    return 0;
}
#endif
