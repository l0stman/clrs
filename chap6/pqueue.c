#include <err.h>

#define parent(i)  ((i)>>1)
#define left(i)    ((i)<<1)
#define right(i)   ((i)<<1|1)
#define elt(hp, i) ((hp)->buf[i-1])

typedef struct heap {
        int *buf;
        size_t len;
        size_t size;
} heap_t;


static inline void
swap(heap_t *hp, size_t i, size_t j)
{
        int t = elt(hp, i);
        elt(hp, i) = elt(hp, j);
        elt(hp, j) = t;
}

static inline int
heap_max(heap_t *hp)
{
        return elt(hp, 1);
}

static int
heap_extract_max(heap_t *hp)
{
        size_t max;

        if (hp->size < 1)
                err(EXIT_FAILURE, "heap underflow");
        max = elt(hp, 1);
        elt(hp, 1) = elt(hp, hp->size);
        hp->size--;
        max_heapify(hp, 1);
        return max;
}

static void
heap_increase_key(heap_t *hp, size_t i, int k)
{
        if (k < elt(hp, i))
                err(EXIT_FAILURE, "new key is smaller than current key");
        elt(hp, i) = k;
        while (i && elt(hp, parent(i)) < elt(hp, i)) {
                swap(hp, i, parent(i));
                i = parent(i);
        }
}

static max_heap_insert(heap_t *hp, int k)
{
        hp->size++;
        elt(hp, hp->size) = k-1;
        heap_increase_key(hp, hp->size, k);
}

