#include "mpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

mPool *mPool_allocate(size_t size)
{
    mPool *pool = (mPool *) malloc(sizeof(mPool) + size);
    //mPool *pool = (mPool *) calloc(size, sizeof(char));
    if (pool) {
        pool->head = pool->next = pool + sizeof(mPool);
        pool->end = pool + size + sizeof(mPool);
        printf("pool->head = %p\n", pool->head);
        printf("pool->head = %p\n", pool->end);
    }

    return pool;
}


void *pool_access(mPool *pool, size_t size)
{
    printf("size = %lu\n", size);
    //the unit of size is sizeof(mPool) which is 24 bytes
    printf("pool->end - pool->next = %zu \n", (pool->end - pool->next));
    if (pool->end - pool->next < size) {
        return NULL;
    }
    void *thisPtr = pool->next;
    pool->next = pool->next + size;
    printf("thisPtr = %p \n", thisPtr);
    printf("pool->next = %p \n", pool->next);
    return thisPtr;
}

void poolFree(mPool *pool)
{
    free(pool->head);
    free(pool);
}
