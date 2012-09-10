#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct _list list;

struct _list {
    void * data;
    list *next;
};

const size_t list_size = sizeof(list);

list * list_new(void *data) {
    list *new = malloc(list_size);
    new->next = NULL;
    new->data = data;
    return new;
}

list * list_add(list *lst, void *data) {
    lst->next = list_new(data);
    return lst->next;
}

void list_free(list *lst) {
    list *tmp = lst;
    while(tmp != NULL) {
        if (tmp->data != NULL) {
            free(tmp->data);
        }
        tmp = tmp->next;
    }
}

main() {
    list *one = list_new(strdup("Gabriel"));
    list *two = list_add(one, strdup("Falcao"));

    list *current = one;
    while(current != NULL) {
        printf("\033[1;32m%s \033[0m", (char *) current->data);

        current = current->next;
    }
    printf("\n");
    list_free(one);
    return 0;
}
