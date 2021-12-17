#include <stdint.h>

#ifndef DEF_MEMORY_BUCKET
# define DEF_MEMORY_BUCKET
typedef struct memory_bucket {
    intptr_t* elements;
    uintmax_t count;
    uintmax_t size;
    struct memory_bucket* next;
} MEMORY_BUCKET;

MEMORY_BUCKET* mb_new(int size) {
    MEMORY_BUCKET* new_bucket = (MEMORY_BUCKET*) malloc(sizeof(MEMORY_BUCKET));
    new_bucket->count = 0;
    new_bucket->size = size;
    new_bucket->elements = (intptr_t*) calloc(new_bucket->size, sizeof(intptr_t));

    return new_bucket;
}

MEMORY_BUCKET* mb_add_element(MEMORY_BUCKET* root, intptr_t element, uintmax_t next_allocation_size) {
    MEMORY_BUCKET* current = root;
    while (current != NULL && current->next != NULL && current->count == current->size) {
        current = current->next;
    }

    if (current == NULL || current->count == current->size) {
        MEMORY_BUCKET* new_bucket = mb_new(next_allocation_size);

        if (current != NULL) {
            current->next = new_bucket;
        }
        
        current = new_bucket;
    }

    current->elements[current->count++] = element;

    return current;
}

void mb_for_each(MEMORY_BUCKET* root, void (*fun)(intptr_t, intptr_t), intptr_t extra_arg) {
    if (root == NULL) {
        return;
    }

    MEMORY_BUCKET* current = root;
    while(current) {
        for (int i = 0; i < current->count; i++) {
            fun(current->elements[i], extra_arg);
        }

        current = current->next;
    }
}

intmax_t mb_count_elements(MEMORY_BUCKET* root) {
    int count = 0;
    while (root != NULL) {
        count += root->count;
        root = root->next;
    }

    return count;
}
#endif