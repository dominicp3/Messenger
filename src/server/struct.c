#include "server/struct.h"

void cl_destroy(void *cl_void)
{
        struct client *cl = cl_void;
        pthread_cond_destroy(&cl->cond);
        pthread_mutex_destroy(cl->mutex);
        free(cl);
}

int cl_check_id(void *cl_void, void *id_void)
{
        struct client *cl = cl_void;
        int *id = id_void;
        return cl->id == *id;
}

void cl_print(void *cl_void)
{
        struct client *cl = cl_void;
        printf("---------------------\n");
        printf("id     = %d\n", cl->id);
        printf("fd     = %d\n", cl->fd);
        printf("ip     = %d\n", cl->ip);
        printf("status = %d\n", cl->status);
        printf("---------------------\n\n");
}