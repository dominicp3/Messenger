#ifndef SERVER_H
#define SERVER_H

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
#include "request.h"
#include "list/list.h"
#include "sendrecv/sendrecv.h"

void run_server(int sockfd, char *ip, uint16_t port);
struct client *accept_client(int sockfd);
void initial_message(int fd, int id);

#endif /* SERVER_H */
