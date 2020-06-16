#include "sendrecv.h"

ssize_t send_all(int fd, void *buf, size_t n)
{
        uint8_t *buffer = (uint8_t*)buf;
        uint64_t offset = 0;
        ssize_t x;
        while (offset < n) {
                if ((x = send(fd, &buffer[offset], (10000 < n - offset) ? 10000 : n - offset, 0)) == -1)
                        return x;
                offset += x;
        }
        return offset;
}

ssize_t recv_all(int fd, void *buf, size_t n)
{
        uint8_t *buffer = (uint8_t*)buf;
        uint64_t offset = 0;
        ssize_t x;
        size_t num_bytes = n;
        while (offset < n) {
                if ((x = recv(fd, &buffer[offset], num_bytes, 0)) == -1 || !x)
                        return x;
                offset += x;
                num_bytes -= x;
        }
        return offset;
}
