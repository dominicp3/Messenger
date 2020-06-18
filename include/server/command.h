#ifndef COMMAND_H
#define COMMAND_H

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
#include "list.h"
#include "sendrecv/sendrecv.h"

void clients_command(struct meta *m);
void clear_command(struct meta *m);
void help_command(void);
char **split_string(char *str);
void print_message(int fd);
void status_to_open(struct client *cl);
void connect_id(int fd, struct client **peer_ptr, list_t *list);
void ready_connection(struct client *cl, struct client *peer);

#endif /* COMMAND_H */