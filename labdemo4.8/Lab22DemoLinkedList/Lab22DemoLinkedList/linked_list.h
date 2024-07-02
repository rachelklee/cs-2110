#include <stdlib.h>

#define SUCCESS 0
#define FAILURE -1

typedef struct Node {
    int val;
    struct Node *next;
} Node;

typedef struct LinkedList {
    struct Node *head;
} LinkedList;


int add_at_index(LinkedList *list, int val, int index);

int remove_from_front(LinkedList *list);

