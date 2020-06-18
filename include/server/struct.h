#ifndef STRUCT_H
#define STRUCT_H

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

        struct client *next;
        struct client *prev;
};

struct meta
{
        char *ip;
        uint16_t port;
        list_t *list;
};

#endif /* STRUCT_H */


