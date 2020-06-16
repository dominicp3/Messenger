#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "list.h"

struct diag
{
        char ip[16];
        uint16_t port;
        list_t *list;
};

void clients_command(struct diag *d);
void clear_command(struct diag *d);
void help_command(void);

#endif /* DIAGNOSTIC_H */
