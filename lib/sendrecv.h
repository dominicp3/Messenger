#ifndef SENDRECV_H
#define SENDRECV_H

#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>

ssize_t send_all(int fd, void *buf, size_t n);
ssize_t recv_all(int fd, void *buf, size_t n);

#endif /* SENDRECV_H */
