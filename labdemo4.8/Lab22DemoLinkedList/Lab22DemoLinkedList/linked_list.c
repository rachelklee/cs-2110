#include "linked_list.h"

/** add_at_index
 * 
 * Creates and adds a node at the specified index in the provided linked list.
 * 
 * @param head Pointer to the list which will be added to
 * @param val Val attribute of the node to be added
 * @param index Index in the linked list where the node will be added (0 indexed)
 * @return FAILURE if list is NULL, if malloc fails, or if index is invalid
 * SUCCESS otherwise.
*/
int add_at_index(LinkedList *list, int val, int index) {
    if (!list) {
        return FAILURE;
    }   

    Node *node = (Node *) malloc(sizeof(Node));

    if (!node) {
        return FAILURE;
    }

    node-> val = val;

    if (index == 0) {
        node->next = list->head;
        list->head = node;
        return SUCCESS;
    } else if (!list->head) {
        free(node);
        return FAILURE;
    }

    Node* prev = list->head;

    for (int i = 0; i < index - 1; ++i) {
        prev = prev->next;

        if (!prev) {
            free(node);
        }
    }

    node->next = prev->next;
    prev->next = node;
    return SUCCESS;


}
