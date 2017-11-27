#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include "malloc_b.h"

t_block first_block = NULL;

/* 局部函數聲明 */
static t_block find_block(t_block * last, size_t size);
static t_block extend_heap(t_block last, size_t s);
static void split_block(t_block b, size_t s);
static size_t align8(size_t s);
static t_block get_block(void * p);
static bool valid_addr(void * p);
static t_block fusion(t_block b);
static void copy_block(t_block src, t_block dst);

void * malloc_b(size_t size)
{
    t_block b, last;
    size_t s;

    /* 對齊地址 */
    s = align8(size);
    if (first_block)
    {
        /* 查找適合的 bolock */
        last = first_block;
        b = find_block(&last, s);
        if (b)
        {
            /* 如果可以，則分裂 */
            if ((b->size - s) >= (BLOCK_SIZE + 8))
                split_block(b, s);
            b->free = 0;
        }
        else
        {
            /* 沒有合適的 block,　開闢一個新的 */
            b = extend_heap(last, s);
            if (!b)
                return NULL;
        }

    }
    else
    {
        b = extend_heap(NULL, s);
        if (!b)
            return NULL;

        first_block = b;
    }

    return b->data;
}

void * calloc_b(size_t number, size_t size)
{
    size_t * dymem;
    size_t s8, i;

    dymem = (size_t *) malloc_b(number * size);
    if (dymem)
    {
        s8 = align8(number * size) >> 3;
        for (i = 0; i < s8; i++)
            dymem[i] = 0;
    }

    return dymem;
}

void * realloc_b(void * p, size_t size)
{
    size_t s;
    t_block b, newp;
    void * dymem;

    /* 根據標準文檔，當 p 傳入 NULL 時，相當於調用 malloc */
    if (!p)
        return malloc_b(size);

    if (valid_addr(p))
    {
        s = align8(size);
        b = get_block(p);

        if (b->size >= s)
        {
            if (b->size - s >= (BLOCK_SIZE + 8))
                split_block(b, s);
        }
        else
        {
            /* 看是否可以進行合並 */
            if (b->next && b->next->free && (b->size + BLOCK_SIZE + b->next->size) >= s)
            {
                fusion(b);
                if(b->size - s >= (BLOCK_SIZE + 8))
                    split_block(b, s);
            }
            else
            {
                /* 新　malloc */
                dymem = malloc_b(s);
                if (!dymem)
                    return NULL;

                newp = get_block(dymem);
                copy_block(b, newp);
                free_b(p);
                return (dymem);
            }
        }
        return (p);
    }

    return NULL;
}

void free_b(void * p)
{
    t_block b;
    if (valid_addr(p))
    {
        b = get_block(p);
        b->free = 1;

        if (b->prev && b->prev->free)
            b = fusion(b->prev);

        if (b->next)
            fusion(b);
        else
        {
            if (b->prev)
                b->prev->prev = NULL;
            else
                first_block = NULL;

            brk(b);
        }
    }
}


/* 局部函數定義 */

/* First fit */
static t_block find_block(t_block * last, size_t size)
{
    t_block b = first_block;

    while (b && !(b->free && b->size >= size))
    {
        *last = b;
        b = b->next;
    }

    return b;
}

/* 開闢新的 block */
static t_block extend_heap(t_block last, size_t s)
{
    t_block b;
    b = (t_block) sbrk(0);

    if (sbrk(BLOCK_SIZE + s) == (void *) -1)
        return NULL;

    b->size = s;
    b->next = NULL;
    b->prev = last;

    if (last)
        last->next = b;

    b->free = 0;
    b->ptr = (void *) b->data;

    return b;
}

/* 分裂 block */
static void split_block(t_block b, size_t s)
{
    t_block dymem;

    dymem = (t_block) b->data + s;
    dymem->size = b->size - s - BLOCK_SIZE;
    dymem->next = b->next;
    dymem->prev = b->prev;
    dymem->free = 1;
    b->size = s;
    b->next = dymem;
}

/* 8 對齊 */
static size_t align8(size_t s)
{
    if ((s & 0x7) == 0)
        return s;

    return ((s >> 3) + 1) << 3;
}



static void copy_block(t_block src, t_block dst)
{
    size_t * sdata, * ddata;
    size_t i;

    sdata = (size_t *) src->ptr;
    ddata = (size_t *) dst->ptr;

    for (i = 0; (i * 8) < src->size && (i * 8) < dst->size; i++)
        ddata[i] = sdata[i];
}


static t_block get_block(void * p)
{
    char * tmp = (char *) p;

    tmp -= BLOCK_SIZE;
    p = (void *) tmp;

    return (t_block) p;
}

static bool valid_addr(void * p)
{
    if (first_block)
        if ((t_block) p > first_block && p < sbrk(0))
            return p == (get_block(p)->ptr);

    return false;
}

static t_block fusion(t_block b)
{
    if (b->next && b->next->free)
    {
        b->size += BLOCK_SIZE + b->next->size;
        b->next = b->next->next;

        if (b->next)
            b->next->prev = b;
    }

    return b;
}
