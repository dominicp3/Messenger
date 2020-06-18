#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include "stdint.h"
#include "list/list.h"

typedef struct list_t list_t;

struct client
{
        int fd;
        uint8_t id;
        uint32_t ip;
        uint8_t status;

        list_t *list;
        pthread_t thread;
        pthread_cond_t cond;
        pthread_mutex_t *mutex;
};

struct meta
{
        char *ip;
        uint16_t port;
        list_t *list;
};

void cl_destroy(void *cl_void);
int cl_check_id(void *cl_void, void *id_void);
void cl_print(void *cl_void);

#endif /* STRUCT_H */