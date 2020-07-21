#ifndef CONFIG_H
#define CONFIG_H

#include <arpa/inet.h>
#include <byteswap.h>

#include "request.h"
#include "struct.h"

int config_client(uint16_t port_number, char *ip_addr);

#endif /* CONFIG_H */