#ifndef SETUP_H
#define SETUP_H

#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <byteswap.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "diagnostic.h"
#include "sendrecv.h"
#include "list.h"



int config_server(uint16_t port, char *ip);
void *diagnostic();
void run_server(int sockfd);
void *client_server_talk(void *c_ptr);
struct client *try_client_comm(struct client *client);
struct client *accept_client_connection(int sockfd);

#endif /* SETUP_H */
