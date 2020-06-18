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
void *client_server_talk(void *data_ptr);

#endif /* REQUEST_H */