#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "struct.h"

typedef struct list_t list_t;

list_t* list_init(void);
void list_destroy(list_t *lst);
void list_add(list_t *lst, struct client *cl);
void list_remove(list_t *lst, struct client *cl);
struct client *list_find(list_t *lst, int id);
void list_print(list_t *lst);

#endif /* LIST_H */