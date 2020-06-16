#include "list.h"

struct list_t
{
        struct client *head;
        pthread_mutex_t mutex;
};

list_t* list_init(void)
{
        struct list_t *lst = malloc(sizeof lst);
        lst->head = NULL;
        pthread_mutex_init(&lst->mutex, NULL);
        return lst;
}

void list_destroy(list_t *lst)
{
        if (lst == NULL)
                return;

        struct client *cur = lst->head;
        while (cur != NULL) {

                struct client *tmp = cur;

                pthread_join(tmp->thread, NULL);

                cur = cur->next;
                free(tmp);
        }

        pthread_mutex_destroy(&lst->mutex);
        free(lst);
}

void list_add(list_t *lst, struct client *cl)
{
        pthread_mutex_lock(&lst->mutex);
        cl->next = lst->head;
        lst->head = cl;

        if (cl->next)
                cl->next->prev = cl;
        pthread_mutex_unlock(&lst->mutex);
}

void list_remove(list_t *lst, struct client *cl)
{
        if (cl == NULL)
                return;

        pthread_mutex_lock(&lst->mutex);
        if (cl->prev)
                cl->prev->next = cl->next;
        else
                lst->head = cl->next;

        if (cl->next)
                cl->next->prev = cl->prev;

        free(cl);

        pthread_mutex_unlock(&lst->mutex);
}

struct client *list_find(list_t *lst, int id)
{
        pthread_mutex_lock(&lst->mutex);
        struct client *cur = lst->head;
        while (cur != NULL) {
                if (cur->id == id) {
                        pthread_mutex_unlock(&lst->mutex);
                        return cur;
                }
                cur = cur->next;
        }
        pthread_mutex_unlock(&lst->mutex);

        return NULL;
}

void list_print(list_t *lst)
{
        pthread_mutex_lock(&lst->mutex);
        if (lst->head == NULL)
                printf("no clients are currently being tracked!\n\n");
        struct client *cur = lst->head;
        while (cur != NULL) {
                printf("---------------------\n");
                printf("id     = %d\n", cur->id);
                printf("fd     = %d\n", cur->fd);
                printf("ip     = %d\n", cur->ip);
                printf("status = %d\n", cur->status);
                printf("---------------------\n\n");
                cur = cur->next;
        }
        pthread_mutex_unlock(&lst->mutex);
}