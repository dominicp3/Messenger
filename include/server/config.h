#ifndef CONFIG_H
#define CONFIG_H

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
#include "server.h"

int config_server(uint16_t port, char *ip);

#endif /* CONFIG_H */