#ifndef COMMAND_H
#define COMMAND_H

#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "struct.h"
#include "sendrecv/sendrecv.h"

void peer2peer(int fd);
void print_message(int fd);
void connection_found(struct data *d);
void recieve_id(struct data *d);
char **split_string(char *str);
void open_client(struct data *d);
void connect_id(int fd, char *id_str);
void command_help(void);

#endif /* COMMAND_H */