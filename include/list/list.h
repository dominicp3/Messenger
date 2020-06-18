#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <pthread.h>

typedef struct list_t list_t;
typedef struct node_t node_t;

list_t *lst_init(void (*destroy)(void*), int (*check)(void*, void*), void (*print_element)(void* element));
node_t *lst_prepend(list_t *lst, void *element);
node_t *lst_append(list_t *lst, void *element);
void lst_remove(list_t *lst, node_t *n);
node_t *lst_find(list_t *lst, void *attr);
void *lst_get(node_t *n);
void lst_destroy(list_t *lst);
void lst_print(list_t *lst);

#endif /* LIST_H */