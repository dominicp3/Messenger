#ifndef TALK_H
#define TALK_H

#include <poll.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "sendrecv/sendrecv.h"

struct client_fds
{
        int fd1;
        int fd2;
};

void setup_client_comm(int fd1, int fd2);
void *cl2cl(void *cl_fds_ptr);

#endif /* TALK_H */
