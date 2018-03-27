#include "mpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

mPool *mPool_allocate(size_t size)
{
    mPool *pool = (mPool *) malloc(sizeof(mPool));
    pool->head = pool->next = (char *) calloc(1, size);
    pool->end = pool->head + size;

    return pool;
}

void *pool_access(mPool *pool, size_t size)
{
    if (pool->end - pool->head < size) {
        return NULL;
    }
    void *thisPtr = pool->next;
    pool->next = pool->next + size;

    return thisPtr;
}

void poolFree(mPool *pool)
{
    free(pool->head);
    free(pool);
}
