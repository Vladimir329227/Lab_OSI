#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define MIN_BLOCK_SIZE 16
#define MAX_ORDER 20

typedef struct BuddyAllocator {
    void *memory;
    size_t size;
    size_t order;
    struct BuddyBlock *free_lists[MAX_ORDER + 1];
} BuddyAllocator;

typedef struct BuddyBlock {
    struct BuddyBlock *next;
    size_t order;
} BuddyBlock;

void *allocator_create(void *const memory, const size_t size) {
    BuddyAllocator *allocator = (BuddyAllocator *)memory;
    allocator->memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (allocator->memory == MAP_FAILED)
        return NULL;
    
    allocator->size = size;
    allocator->order = 0;
    while ((1UL << allocator->order) < size) 
        allocator->order++;
    
    for (size_t i = 0; i <= MAX_ORDER; i++)
        allocator->free_lists[i] = NULL;

    BuddyBlock *block = (BuddyBlock *)allocator->memory;
    block->next = NULL;
    block->order = allocator->order;
    allocator->free_lists[allocator->order] = block;

    return allocator;
}

void allocator_destroy(void *const allocator) {
    BuddyAllocator *buddy = (BuddyAllocator *)allocator;
    munmap(buddy->memory, buddy->size);
}

void *allocator_alloc(void *const allocator, const size_t size) {
    BuddyAllocator *buddy = (BuddyAllocator *)allocator;
    size_t order = 0;
    while ((1UL << order) < size) 
        order++;
    
    for (size_t i = order; i <= MAX_ORDER; i++) {
        if (buddy->free_lists[i]) {
            BuddyBlock *block = buddy->free_lists[i];
            buddy->free_lists[i] = block->next;
            while (i > order) {
                i--;
                BuddyBlock *buddy_block = (BuddyBlock *)((char *)block + (1UL << i));
                buddy_block->next = buddy->free_lists[i];
                buddy_block->order = i;
                buddy->free_lists[i] = buddy_block;
            }
            return block;
        }
    }
    return NULL;
}

void allocator_free(void *const allocator, void *const memory) {
    BuddyAllocator *buddy = (BuddyAllocator *)allocator;
    BuddyBlock *block = (BuddyBlock *)memory;
    size_t order = block->order;

    while (order < MAX_ORDER) {
        BuddyBlock *buddy_block = (BuddyBlock *)((char *)block + (1UL << order));
        if (buddy_block >= (BuddyBlock *)buddy->memory && buddy_block < (BuddyBlock *)((char *)buddy->memory + buddy->size) && buddy_block->order == order) {
            buddy->free_lists[order] = buddy_block->next;
            order++;
            block = (BuddyBlock *)((char *)block - (1UL << (order - 1)));
        } else {
            break;
        }
    }
    block->next = buddy->free_lists[order];
    block->order = order;
    buddy->free_lists[order] = block;
}