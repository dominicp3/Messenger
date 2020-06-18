#include "list/list.h"

struct list_t
{
        node_t *head;
        node_t *tail;
        pthread_mutex_t mutex;
        void (*destroy)(void* element);
        int (*check)(void* element, void* attr);
        void (*print_element)(void* element);
};

struct node_t
{
        void *element;
        struct node_t *prev;
        struct node_t *next;
};

list_t *lst_init(void (*destroy)(void*), int (*check)(void*, void*), void (*print_element)(void*))
{
        list_t *lst = malloc(sizeof *lst);
        lst->head = NULL;
        lst->tail = NULL;
        pthread_mutex_init(&lst->mutex, NULL);
        lst->destroy = destroy;
        lst->check = check;
        lst->print_element = print_element;
        return lst;
}

node_t *lst_prepend(list_t *lst, void *element)
{
        pthread_mutex_lock(&lst->mutex);
        node_t *n = malloc(sizeof *n);
        n->element = element;
        n->prev = NULL;
        n->next = lst->head;

        if (lst->head)
                lst->head->prev = n;
        
        if (lst->tail == NULL)
                lst->tail = n;

        lst->head = n;
        pthread_mutex_unlock(&lst->mutex);
        return n;
}

node_t *lst_append(list_t *lst, void *element)
{
        pthread_mutex_lock(&lst->mutex);
        node_t *n = malloc(sizeof *n);
        n->element = element;
        n->prev = lst->tail;
        n->next = NULL;

        if (lst->tail)
                lst->tail->next = n;

        if (lst->head == NULL)
                lst->head = n;

        lst->tail = n;
        pthread_mutex_unlock(&lst->mutex);

        return n;
}

void lst_remove(list_t *lst, node_t *n)
{       
        pthread_mutex_lock(&lst->mutex);
        node_t **n_ptr;

        n_ptr = n->prev ? &n->prev->next : &lst->head;
        *n_ptr = n->next;

        n_ptr = n->next ? &n->next->prev : &lst->tail;
        *n_ptr = n->prev;

        lst->destroy(n->element);
        pthread_mutex_unlock(&lst->mutex);
}

node_t *lst_find(list_t *lst, void *attr)
{       
        pthread_mutex_lock(&lst->mutex);
        node_t *cur = lst->head;
        while (cur) {
                if (lst->check(cur->element, attr)) {
                        pthread_mutex_unlock(&lst->mutex);
                        return cur;
                }
                        
                cur = cur->next;
        }
        pthread_mutex_unlock(&lst->mutex);

        return NULL;
}

void *lst_get(node_t *n)
{
        if (n == NULL)
                return NULL;
        return n->element;
}

void lst_destroy(list_t *lst)
{
        if (lst == NULL)
                return;

        pthread_mutex_lock(&lst->mutex);
        node_t *cur = lst->head;
        while (cur) {
                node_t *tmp = cur;
                cur = cur->next;

                lst->destroy(tmp->element);
                free(tmp);
        }
        pthread_mutex_unlock(&lst->mutex);

        free(lst);
}

void lst_print(list_t *lst)
{
        pthread_mutex_lock(&lst->mutex);
        node_t *cur = lst->head;
        while (cur) {
                lst->print_element(cur->element);
                cur = cur->next;
        }
        pthread_mutex_unlock(&lst->mutex);
}