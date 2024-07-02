/*
 * CS 2110 Homework 10 Spring 2024
 * Author:
 */

/* we need this for uintptr_t */
#include <stdint.h>
/* we need this for memcpy/memset */
#include <string.h>
/* we need this to print out stuff*/
#include <stdio.h>
/* we need this for the metadata_t struct and my_malloc_err enum definitions */
#include "my_malloc.h"

/* Function Headers
 * Here is a place to put all of your function headers
 * Remember to declare them as static
 */

/* Our freelist structure - our freelist is represented as a singly linked list
 * the freelist is sorted by address;
 */
metadata_t *address_list;

/* Set on every invocation of my_malloc()/my_free()/my_realloc()/
 * my_calloc() to indicate success or the type of failure. See
 * the definition of the my_malloc_err enum in my_malloc.h for details.
 * Similar to errno(3).
 */
enum my_malloc_err my_malloc_errno;



// -------------------- PART 1: Helper functions --------------------

/* The following prototypes represent useful helper functions that you may want
 * to use when writing your malloc functions. We recommend reading over their
 * documentation and prototypes; having a good idea of the kinds of helpers
 * you can use will make it easier to plan your code.
 *
 * None of these functions will be graded individually. However, implementing
 * and using these will make programming easier. We have provided ungraded test
 * cases these functions that you may check your implementations against.
 */


/* HELPER FUNCTION: find_right
 * Given a pointer to a free block, this function searches the freelist for another block to the right of the provided block.
 * If there is a free block that is directly next to the provided block on its right side,
 * then return a pointer to the start of the right-side block.
 * Otherwise, return null.
 * This function may be useful when implementing my_free().
 */
metadata_t *find_right(metadata_t *freed_block) {
    UNUSED_PARAMETER(freed_block);

    metadata_t *curr = address_list;

    while (curr && (uintptr_t) freed_block >= (uintptr_t) curr) {
        curr = curr->next;
    }
    if ((uintptr_t) curr == (uintptr_t) freed_block + TOTAL_METADATA_SIZE + freed_block->size) {
        return curr;
    }

    return NULL;
}

/* GIVEN HELPER FUNCTION: find_left
 * This function is provided for you by the TAs. You do not need to use it, but it may be helpful to you.
 * This function is the same as find_right, but for the other side of the newly freed block.
 * This function will be useful for my_free(), but it is also useful for my_malloc(), since whenever you sbrk a new block,
 * you need to merge it with the block at the back of the freelist if the blocks are next to each other in memory.
 */

metadata_t *find_left(metadata_t *freed_block) {
    metadata_t *curr = address_list;

    while (curr && ((uintptr_t) freed_block > (uintptr_t) curr)) {
        uintptr_t right_of_curr = ((uintptr_t) curr) + TOTAL_METADATA_SIZE + curr->size;
        if (right_of_curr == (uintptr_t) freed_block) {
            return curr; // if `right_of_curr` is the freed block, then the `curr` block is directly left of `freed_block`
        }
        curr = curr->next;
    }
    
    return NULL;
}

/* HELPER FUNCTION: merge
 * This function should take two pointers to blocks and merge them together.
 * The most important step is to increase the total size of the left block to include the size of the right block.
 * You should also copy the right block's next pointer to the left block's next pointer. If both blocks are initially in the freelist, this will remove the right block from the list.
 * This function will be useful for both my_malloc() (when you have to merge sbrk'd blocks) and my_free().
 */
 void merge(metadata_t *left, metadata_t *right) {
    // UNUSED_PARAMETER(left);
    // UNUSED_PARAMETER(right);
    left->size = left->size + TOTAL_METADATA_SIZE + right->size;
    left->next = right->next;


 }

/* HELPER FUNCTION: split_block
 * This function should take a pointer to a large block and a requested size, split the block in two, and return a pointer to the new block (the right part of the split).
 * Remember that you must make the right side have the user-requested size when splitting. The left side of the split should have the remaining data.
 * We recommend doing the following steps:
 * 1. Compute the total amount of memory that the new block will take up (both metadata and user data).
 * 2. Using the new block's total size with the address and size of the old block, compute the address of the start of the new block.
 * 3. Shrink the size of the old/left block to account for the lost size. This block should stay in the freelist.
 * 4. Set the size of the new/right block and return it. This block should not go in the freelist.
 * This function will be useful for my_malloc(), particularly when the best-fit block is big enough to be split.
 */
 metadata_t *split_block(metadata_t *block, size_t size) {
    UNUSED_PARAMETER(block);
    UNUSED_PARAMETER(size);

    // 1. Compute the total amount of memory that the new block will take up (both metadata and user data)
    // size_t rightMem = sizeof(metadata_t) + size;
    // size_t leftMem = block->size - rightMem;

    size_t newMem = TOTAL_METADATA_SIZE + size;

    // 2. Using the new block's total size with the address and size of the old block, compute the address of the start of the new block

    metadata_t *newBlockAddress = (metadata_t *)((uintptr_t)block + TOTAL_METADATA_SIZE + block->size - newMem);

    // 3. Shrink the size of the old/left block to account for the lost size. This block should stay in the freelist

    block->size = block->size - newMem;

    // 4. Set the size of the new/right block and return it. This block should not go in the freelist

    newBlockAddress->size = size;
    return newBlockAddress;
 }

/* HELPER FUNCTION: add_to_addr_list
 * This function should add a block to freelist.
 * Remember that the freelist must be sorted by address. You can compare the addresses of blocks by comparing the metadata_t pointers like numbers (do not dereference them).
 * Don't forget about the case where the freelist is empty. Remember what you learned from Homework 9.
 * This function will be useful for my_malloc() (mainly for adding in sbrk blocks) and my_free().
 */
 void add_to_addr_list(metadata_t *block) {
    UNUSED_PARAMETER(block);

    metadata_t *curr = address_list;

    if (address_list == NULL || (uintptr_t) block < (uintptr_t) address_list) {
        block->next = address_list;
        address_list = block;
        return;
    }

    while (curr->next != NULL && (uintptr_t) curr->next < (uintptr_t) block) {
        curr = curr->next;
    }

    metadata_t *next;
    while ((next = curr->next) && (uintptr_t) block > (uintptr_t) next) {
        curr = next;
    }

    curr->next = block;    
    block->next = next;
}

/* GIVEN HELPER FUNCTION: remove_from_addr_list
 * This function is provided for you by the TAs. You are not required to use it or our implementation of it, but it may be helpful to you.
 * This function should remove a block from the freelist.
 * Simply search through the freelist, looking for a node whose address matches the provided block's address.
 * This function will be useful for my_malloc(), particularly when the best-fit block is not big enough to be split.
 */
 void remove_from_addr_list(metadata_t *block) {
    metadata_t *curr = address_list;
    if (!curr) {
        return;
    } else if (curr == block) {
        address_list = curr->next;
    }

    metadata_t *next;
    while ((next = curr->next) && (uintptr_t) block > (uintptr_t) next) {
        curr = next;
    }
    if (next == block) {
        curr->next = next->next;
    }
}
/* HELPER FUNCTION: find_best_fit
 * This function should find and return a pointer to the best-fit block. See the PDF for the best-fit criteria.
 * Remember that if you find the perfectly sized block, you should return it immediately.
 * You should not return an imperfectly sized block until you have searched the entire list for a potential perfect block.
 * Note that you should NOT be splitting the best-fit block in this function (do this within my_malloc() instead).
 */
 metadata_t *find_best_fit(size_t size) {
    UNUSED_PARAMETER(size);

    metadata_t *curr = address_list;
    metadata_t *imperfect = NULL;

    while (curr != NULL) {
        if (curr->size == size) {
            return curr;
        }
        if (curr->size > size) {
            if (imperfect == NULL || curr->size < imperfect->size) {
                imperfect = curr;
            }
        }
        curr = curr->next;
    }

    return imperfect;
 }




// ------------------------- PART 2: Malloc functions -------------------------

/* Before starting each of these functions, you should:
 * 1. Understand what the function should do, what it should return, and what the freelist should look like after it finishes
 * 2. Develop a high-level plan for how to implement it; maybe sketch out pseudocode
 * 3. Check if the parameters have any special cases that need to be handled (when they're NULL, 0, etc.)
 * 4. Consider what edge cases the implementation needs to handle
 * 5. Think about any helper functions above that might be useful, and implement them if you haven't already
 */


/* MALLOC
 * See PDF for documentation
 */
void *my_malloc(size_t size) {
    my_malloc_errno = NO_ERROR;
    UNUSED_PARAMETER(size);

    // Reminder of how to do malloc:
    // 1. Make sure the size is not too small or too big.
    
    if (size == 0) {
        my_malloc_errno = NO_ERROR;
        return NULL;
    }

    if (size > SBRK_SIZE - TOTAL_METADATA_SIZE) {
        my_malloc_errno = SINGLE_REQUEST_TOO_LARGE;
        return NULL;
    }

    // 2. Search for a best-fit block. See the PDF for information about what to check.

    metadata_t *bestFit = find_best_fit(size);

    if (bestFit) {
        if (!(bestFit->size == size || bestFit->size - size < MIN_BLOCK_SIZE)) {
            bestFit = split_block(bestFit, size);
        }
        remove_from_addr_list(bestFit);
        return (void *)(((uint8_t *) bestFit) + TOTAL_METADATA_SIZE);
    }

    metadata_t *newBlock = my_sbrk(SBRK_SIZE);
    if (newBlock == (void *) -1) {
        my_malloc_errno = OUT_OF_MEMORY;
        return NULL;
    }
    newBlock->size = SBRK_SIZE - TOTAL_METADATA_SIZE;
    newBlock->next = NULL;
    // add_to_addr_list(newBlock);
    my_free(newBlock + 1);

    bestFit = find_best_fit(size);

    if (bestFit) {
        if (!(bestFit->size == size || bestFit->size - size < MIN_BLOCK_SIZE)) {
            bestFit = split_block(bestFit, size);
        }
        remove_from_addr_list(bestFit);
        return (void *)(((uint8_t *) bestFit) + TOTAL_METADATA_SIZE);
    }

    my_malloc_errno = OUT_OF_MEMORY;
    return NULL;

    // metadata_t *newFit = find_best_fit(size);

    // if (!(newFit->size == size || newFit->size - size < MIN_BLOCK_SIZE)) {
    //     newFit = split_block(newFit, size);
    // }
    // remove_from_addr_list(newFit);
    // return (void *)((uint8_t *) newFit + TOTAL_METADATA_SIZE);
    
}

/* FREE
 * See PDF for documentation
 */
void my_free(void *ptr) {
    my_malloc_errno = NO_ERROR;
    UNUSED_PARAMETER(ptr);

    if (ptr == NULL) {
        my_malloc_errno = NO_ERROR;
        return;
    }

    // Reminder for how to do free:
    // 1. Since ptr points to the start of the user block, obtain a pointer to the metadata for the freed block.    
    metadata_t *newMetadata = (metadata_t *)((uint8_t *)ptr - TOTAL_METADATA_SIZE);

    add_to_addr_list(newMetadata);

    // 2. Look for blocks in the freelist that are positioned immediately before or after the freed block.
    metadata_t *beforeBlock = find_left(newMetadata);
    metadata_t *afterBlock = find_right(newMetadata);

    // 2.a. If a block is found before or after the freed block, then merge the blocks.
    
    if (afterBlock) {
        merge(newMetadata, afterBlock);
    }

    if (beforeBlock) {
        merge(beforeBlock, newMetadata);
        newMetadata = beforeBlock;
    }

    // 3. Once the freed block has been merged (if needed), add the freed block back to the freelist.

}

/* REALLOC
 * See PDF for documentation
 */
void *my_realloc(void *ptr, size_t size) {
    my_malloc_errno = NO_ERROR;
    UNUSED_PARAMETER(ptr);
    UNUSED_PARAMETER(size);

    // Reminder of how to do realloc:
    // 1. If ptr is NULL, then only call my_malloc(size). If the size is 0, then only call my_free(ptr).
    if (ptr == NULL) {
        return my_malloc(size);
    }
    if (size == 0) {
        my_free(ptr);
        return NULL;
    }

    // 2. Call my_malloc to allocate the requested number of bytes. If this fails, immediately return NULL and do not free the old allocation.
    void *mallocPtr = my_malloc(size);
    if (mallocPtr == NULL) {
        return NULL;
    }

    // 3. Copy the data from the old allocation to the new allocation. We recommend using memcpy to do this. Be careful not to read or write out-of-bounds!
    metadata_t *metadataPtr = (metadata_t *)((uint8_t *)ptr - TOTAL_METADATA_SIZE);
    size_t copySize;
    if (size < metadataPtr->size) {
        copySize = size;
    } else {
        copySize = metadataPtr->size;
    }
    memcpy(mallocPtr, ptr, copySize);



    // 4. Free the old allocation and return the new allocation.
    my_free(ptr);

    return mallocPtr;
}

/* CALLOC
 * See PDF for documentation
 */
void *my_calloc(size_t nmemb, size_t size) {
    my_malloc_errno = NO_ERROR;
    UNUSED_PARAMETER(nmemb);
    UNUSED_PARAMETER(size);

    // Reminder for how to do calloc:
    // 1. Use my_malloc to allocate the appropriate amount of size.
    void *mallocPtr = my_malloc(nmemb * size);

    if (mallocPtr == NULL) {
        return NULL;
    }

    // 2. Clear all of the bytes that were allocated. We recommend using memset to do this.
    memset(mallocPtr, 0, nmemb * size);
    return mallocPtr;
}
