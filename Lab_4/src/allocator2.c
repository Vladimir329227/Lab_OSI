#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

typedef struct Block {
    size_t size;
    struct Block *next;
    int free;
} Block;

typedef struct Allocator {
    void *memory;
    size_t total_size;
    Block *free_list;
} Allocator;

void *allocator_create(size_t size) {
    void *memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (memory == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }

    Allocator *allocator = (Allocator *)memory;
    allocator->memory = memory;
    allocator->total_size = size;
    allocator->free_list = (Block *)(memory + sizeof(Allocator));
    allocator->free_list->size = size - sizeof(Allocator) - sizeof(Block);
    allocator->free_list->next = NULL;
    allocator->free_list->free = 1;

    return allocator;
}

void allocator_destroy(void *const allocator) {
    Allocator *alloc = (Allocator *)allocator;
    if (munmap(alloc->memory, alloc->total_size) == -1)
        perror("munmap");
}

void *allocator_alloc(void *const allocator, const size_t size) {
    Allocator *alloc = (Allocator *)allocator;
    Block *prev = NULL;
    Block *curr = alloc->free_list;

    while (curr) {
        if (curr->free && curr->size >= size) {
            if (curr->size > size + sizeof(Block)) {
                Block *new_block = (Block *)((char *)curr + sizeof(Block) + size);
                new_block->size = curr->size - size - sizeof(Block);
                new_block->next = curr->next;
                new_block->free = 1;
                curr->size = size;
                curr->next = new_block;
            } else {
                if (prev) {
                    prev->next = curr->next;
                } else {
                    alloc->free_list = curr->next;
                }
            }
            curr->free = 0;
            return (void *)((char *)curr + sizeof(Block));
        }
        prev = curr;
        curr = curr->next;
    }

    return NULL;
}

void allocator_free(void *const allocator, void *const memory) {
    Allocator *alloc = (Allocator *)allocator;
    Block *block = (Block *)((char *)memory - sizeof(Block));
    block->free = 1;

    Block *curr = alloc->free_list;
    Block *prev = NULL;

    while (curr && curr < block) {
        prev = curr;
        curr = curr->next;
    }

    if (prev && (char *)prev + sizeof(Block) + prev->size == (char *)block) {
        prev->size += sizeof(Block) + block->size;
        prev->next = block->next;
    } else {
        if (prev) {
            block->next = prev->next;
            prev->next = block;
        } else {
            block->next = alloc->free_list;
            alloc->free_list = block;
        }
    }

    if (curr && (char *)block + sizeof(Block) + block->size == (char *)curr) {
        block->size += sizeof(Block) + curr->size;
        block->next = curr->next;
    }
}
