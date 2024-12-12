#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct FreeBlock {
    struct FreeBlock *next;
    size_t size;
} FreeBlock;

typedef struct {
    void *memory;
    size_t size;
    size_t used;
    FreeBlock *free_list;
} Allocator;

Allocator* allocator_create(void *const memory, const size_t size) {
    Allocator *allocator = (Allocator*)malloc(sizeof(Allocator));
    allocator->memory = memory;
    allocator->size = size;
    allocator->used = 0;
    allocator->free_list = NULL;
    return allocator;
}

void allocator_destroy(Allocator *const allocator) {
    free(allocator);
}

void* allocator_alloc(Allocator *const allocator, const size_t size) {
    size_t block_size = 1;
    while (block_size < size) {
        block_size <<= 1;
    }

    FreeBlock *prev = NULL;
    FreeBlock *curr = allocator->free_list;

    while (curr) {
        if (curr->size >= block_size) {
            if (curr->size > block_size) {
                FreeBlock *new_block = (FreeBlock*)((char*)curr + block_size);
                new_block->size = curr->size - block_size;
                new_block->next = curr->next;
                if (prev) {
                    prev->next = new_block;
                } else {
                    allocator->free_list = new_block;
                }
            } else {
                if (prev) {
                    prev->next = curr->next;
                } else {
                    allocator->free_list = curr->next;
                }
            }
            allocator->used += block_size;
            return curr;
        }
        prev = curr;
        curr = curr->next;
    }

    if (allocator->used + block_size > allocator->size) {
        return NULL;
    }

    void *ptr = (char*)allocator->memory + allocator->used;
    allocator->used += block_size;
    return ptr;
}

void allocator_free(Allocator *const allocator, void *const memory) {
    FreeBlock *block = (FreeBlock*)memory;
    block->next = allocator->free_list;
    allocator->free_list = block;
    allocator->used -= block->size;
}
