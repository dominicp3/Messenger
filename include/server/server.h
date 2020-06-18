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

#include "request.h"
#include "list.h"
#include "sendrecv/sendrecv.h"
#include "struct.h"

void run_server(int sockfd, char *ip, uint16_t port);
struct client *incoming_client_connection(int sockfd, int cur_id, list_t *list);
struct client *accept_client_connection(int sockfd);

#endif /* SERVER_H */
