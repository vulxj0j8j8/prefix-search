#ifndef MPOOL
#define MPOOL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct mPool {
    struct mPool *head;
    struct mPool *end;
    struct mPool *next;
    //struct mPool *nextPool;
} mPool;

mPool *mPool_allocate(size_t size);
void *pool_access(mPool *pool, size_t size);
void poolFree(mPool *pool);
#endif


