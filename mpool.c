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
        //pool->head = pool->next = pool + sizeof(mPool);
        pool->head = pool->next = pool + 1;
        pool->end = pool + size + sizeof(mPool);
    }

    return pool;
}


void *pool_access(mPool *pool, size_t size)
{
    //the unit of size is sizeof(mPool) which is 24 bytes
    if (pool->end - pool->next < size) {
        return NULL;
    }
    void *thisPtr = pool->next;
    pool->next = (mPool *)((char *)pool->next + size);

    return thisPtr;
}

void poolFree(mPool *pool)
{
    //free(pool->head);
    free(pool);
}
