#ifndef REQUEST_H
#define REQUEST_H

#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <byteswap.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "struct.h"
#include "command.h"

void user_request(struct meta *m);
void *client_request(void *node_void);

#endif /* REQUEST_H */