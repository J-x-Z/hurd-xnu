/* pfinet_xnu.c - XNU-compatible Hurd pfinet (TCP/IP) server
   Simplified implementation using BSD sockets */

#include <stdlib.h>
#include <string.h>
#include <mach/mach.h>
#include <pthread.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "mach.h"
#include "ports.h"

/* MIG-generated header */
#include "socketServer.h"

/* Socket table */
#define MAX_SOCKETS 256
static int socket_fds[MAX_SOCKETS];
static int socket_count = 0;
static pthread_mutex_t socket_lock = PTHREAD_MUTEX_INITIALIZER;

static struct port_bucket *pfinet_bucket;
static struct port_class *pfinet_portclass;

/* Map Hurd socket to fd */
static int port_to_fd(mach_port_t port) {
    /* Simplified: use port value as index */
    unsigned int idx = (unsigned int)port & 0xFF;
    if (idx < MAX_SOCKETS && socket_fds[idx] >= 0)
        return socket_fds[idx];
    return -1;
}

/* Initialize pfinet server */
int pfinet_server_init(void)
{
    pfinet_bucket = ports_create_bucket();
    if (!pfinet_bucket) return -1;
    
    pfinet_portclass = ports_create_class(NULL, NULL);
    if (!pfinet_portclass) return -1;
    
    memset(socket_fds, -1, sizeof(socket_fds));
    
    printf("Pfinet server initialized\n");
    return 0;
}

/* 26100: Create socket */
kern_return_t
socket_create(mach_port_t server,
              int sock_type, int protocol,
              mach_port_t *sock)
{
    int fd = socket(AF_INET, sock_type, protocol);
    if (fd < 0) {
        *sock = MACH_PORT_NULL;
        return errno;
    }
    
    pthread_mutex_lock(&socket_lock);
    int idx = socket_count++ % MAX_SOCKETS;
    socket_fds[idx] = fd;
    *sock = (mach_port_t)(idx | 0x100);  /* Simple mapping */
    pthread_mutex_unlock(&socket_lock);
    
    return 0;
}

/* 26101: Bind */
kern_return_t
socket_bind(mach_port_t sock, char addr[16])
{
    int fd = port_to_fd(sock);
    if (fd < 0) return EBADF;
    
    struct sockaddr_in sin;
    memcpy(&sin, addr, sizeof(sin));
    
    if (bind(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        return errno;
    return 0;
}

/* 26102: Listen */
kern_return_t
socket_listen(mach_port_t sock, int backlog)
{
    int fd = port_to_fd(sock);
    if (fd < 0) return EBADF;
    
    if (listen(fd, backlog) < 0)
        return errno;
    return 0;
}

/* 26103: Accept */
kern_return_t
socket_accept(mach_port_t sock,
              mach_port_t *conn,
              char peer_addr[16])
{
    int fd = port_to_fd(sock);
    if (fd < 0) return EBADF;
    
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    int newfd = accept(fd, (struct sockaddr *)&sin, &len);
    if (newfd < 0) {
        *conn = MACH_PORT_NULL;
        return errno;
    }
    
    pthread_mutex_lock(&socket_lock);
    int idx = socket_count++ % MAX_SOCKETS;
    socket_fds[idx] = newfd;
    *conn = (mach_port_t)(idx | 0x100);
    pthread_mutex_unlock(&socket_lock);
    
    memcpy(peer_addr, &sin, 16);
    return 0;
}

/* 26104: Connect */
kern_return_t
socket_connect(mach_port_t sock, char addr[16])
{
    int fd = port_to_fd(sock);
    if (fd < 0) return EBADF;
    
    struct sockaddr_in sin;
    memcpy(&sin, addr, sizeof(sin));
    
    if (connect(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        return errno;
    return 0;
}

/* 26105: Send */
kern_return_t
socket_send(mach_port_t sock,
            char *data, mach_msg_type_number_t dataCnt,
            int flags, int *amount)
{
    int fd = port_to_fd(sock);
    if (fd < 0) return EBADF;
    
    ssize_t n = send(fd, data, dataCnt, flags);
    if (n < 0) {
        *amount = 0;
        return errno;
    }
    *amount = (int)n;
    return 0;
}

/* 26106: Receive */
kern_return_t
socket_recv(mach_port_t sock,
            char **data, mach_msg_type_number_t *dataCnt,
            int flags, int amount)
{
    int fd = port_to_fd(sock);
    if (fd < 0) return EBADF;
    
    *data = malloc(amount);
    if (!*data) return ENOMEM;
    
    ssize_t n = recv(fd, *data, amount, flags);
    if (n < 0) {
        free(*data);
        *data = NULL;
        *dataCnt = 0;
        return errno;
    }
    *dataCnt = (mach_msg_type_number_t)n;
    return 0;
}

/* 26107: Get socket name */
kern_return_t
socket_name(mach_port_t sock, char addr[16])
{
    int fd = port_to_fd(sock);
    if (fd < 0) return EBADF;
    
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    if (getsockname(fd, (struct sockaddr *)&sin, &len) < 0)
        return errno;
    
    memcpy(addr, &sin, 16);
    return 0;
}

/* 26108: Get peer name */
kern_return_t
socket_peername(mach_port_t sock, char addr[16])
{
    int fd = port_to_fd(sock);
    if (fd < 0) return EBADF;
    
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    if (getpeername(fd, (struct sockaddr *)&sin, &len) < 0)
        return errno;
    
    memcpy(addr, &sin, 16);
    return 0;
}

/* 26109: Shutdown */
kern_return_t
socket_shutdown(mach_port_t sock, int how)
{
    int fd = port_to_fd(sock);
    if (fd < 0) return EBADF;
    
    if (shutdown(fd, how) < 0)
        return errno;
    return 0;
}

/* 26110: Get socket option */
kern_return_t
socket_getopt(mach_port_t sock,
              int level, int option,
              char **optval, mach_msg_type_number_t *optvalCnt)
{
    int fd = port_to_fd(sock);
    if (fd < 0) return EBADF;
    
    *optval = malloc(256);
    if (!*optval) return ENOMEM;
    
    socklen_t len = 256;
    if (getsockopt(fd, level, option, *optval, &len) < 0) {
        free(*optval);
        *optval = NULL;
        *optvalCnt = 0;
        return errno;
    }
    *optvalCnt = len;
    return 0;
}

/* 26111: Set socket option */
kern_return_t
socket_setopt(mach_port_t sock,
              int level, int option,
              char *optval, mach_msg_type_number_t optvalCnt)
{
    int fd = port_to_fd(sock);
    if (fd < 0) return EBADF;
    
    if (setsockopt(fd, level, option, optval, optvalCnt) < 0)
        return errno;
    return 0;
}

#ifdef PFINET_TEST
int main(int argc, char *argv[])
{
    printf("=== Pfinet Server Test ===\n");
    
    if (pfinet_server_init() < 0) {
        printf("FAILED: pfinet_server_init\n");
        return 1;
    }
    printf("PASSED: pfinet_server_init\n");
    
    /* Test socket creation */
    mach_port_t sock;
    if (socket_create(MACH_PORT_NULL, SOCK_STREAM, 0, &sock) == 0) {
        printf("PASSED: socket_create (port=0x%x)\n", sock);
        socket_shutdown(sock, 2);
    } else {
        printf("FAILED: socket_create\n");
    }
    
    printf("=== Pfinet Server Test Complete ===\n");
    return 0;
}
#endif
