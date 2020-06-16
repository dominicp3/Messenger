#ifndef CLIENT_H
#define CLIENT_H

#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "sendrecv.h"

struct data
{
        int fd;
        int id;
        int status;
        char **s;

        pthread_t in_thread;
        pthread_t out_thread;
        pthread_t comm_thread;
};

int config_client(uint16_t port_number, char *ip_addr);
char **split_string(char *str);
void print_message(int fd);
void connect_id(int fd, char *id_str);
void open_client(struct data *d);
void connection_found(struct data *d);
void command_help(void);
void recieve_id(struct data *d);
void *outgoing_commands(void *data_ptr);
void *incomming_commands(void *data_ptr);
void *start_comm(void *d_void);
void *incoming(void *fd_ptr);
void *outgoing(void *fd_ptr);

#endif /* CLIENT_H */