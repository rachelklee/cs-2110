#include "tests.h"

// DONT SHOW THIS IN LAB LOL
int main() {

    LinkedList *list = malloc(sizeof(LinkedList));
    list->head = NULL;

    if (add_at_index(list, 0, 1) == 0) {
        printf("Added at index 1 to list with no nodes, recieved SUCCESS, expected FAILURE\n");
        destroy_list(list);
        abort();
    }

    if (add_at_index(list, 0, 0) == -1) {
        printf("Added at index 0 to list with no nodes, recieved FAILURE, expected SUCCESS\n");
        destroy_list(list);
        abort();
    }

    if (list->head == NULL) {
        printf("Added at index 0 to list with no nodes, head was NULL, expected non NULL\n");
        destroy_list(list);
        abort();
    }

    if (add_at_index(list, 2, 1) == -1) {
        printf("Added at index 1 to list with 1 node, recieved FAILURE, expected SUCCESS\n");
        destroy_list(list);
        abort();
    }

    if (add_at_index(list, 1, 1) == -1) {
        printf("Added at index 1 to list with 2 nodes, recieved FAILURE, expected SUCCESS\n");
        destroy_list(list);
        abort();
    }

    if (add_at_index(list, 3, 3) == -1) {
        printf("Added at index 3 to list with 3 nodes, recieved FAILURE, expected SUCCESS\n");
        destroy_list(list);
        abort();
    }

    if (add_at_index(list, 3, 6) == 0) {
        printf("Added at index 6 to list with 4 nodes, recieved SUCCESS, expected FAILURE\n");
        destroy_list(list);
        abort();
    }

    printf("Checking list, should be 0 -> 1 -> 2 -> 3 -> NULL\n");
    Node *curr = list->head;
    for (int i = 0; i < 4; i++) {
        if (curr == NULL) {
            printf("Node at index %d was NULL\n", i);
            destroy_list(list);
            abort();
        } 

        if (curr->val != i) {
            printf("Node at index %d has value %d instead of %d\n", i, curr->val, i);
            destroy_list(list);
            abort();
        } 
        curr = curr->next;
    }
    destroy_list(list);

    // Node **waste = NULL;
    // int count = 0;
    // while (1) {
    //     Node *new = malloc(sizeof(Node));
    //     if (new == NULL)
    //         break;

    //     new->val = 0;

    //     waste = realloc(waste, (count + 1) * sizeof(Node*));
    //     waste[count] = new;
    //     count++;
    // }

    // while(1)
    // {
    //     void *m = malloc(1024*1024);
    //     memset(m,0,1024*1024);
    // }
 
    printf("All tests passed!\n");
    return 0;
}


void destroy_list(LinkedList *list) {
    Node *curr = list->head;
    while (curr != NULL) {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(list);
}