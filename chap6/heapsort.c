#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#define parent(i)       ((i)>>1)
#define left(i)         ((i)<<1)
#define right(i)        ((i)<<1|1)
#define elt(hp, i)      ((hp)->buf[i-1])

typedef struct heap {
        int *buf;
        size_t len;
        size_t size;
} heap_t;

static inline heap_t *
nheap(int *A, size_t len)
{
        heap_t *hp;

        if ((hp = malloc(sizeof(*hp))) == NULL)
                err(EXIT_FAILURE, "Not enough memory");
        hp->buf = A;
        hp->len = hp->size = len;
        return hp;
}

static inline void
swap(heap_t *hp, int i, int j)
{
        int t = elt(hp, i);
        elt(hp, i) = elt(hp, j);
        elt(hp, j) = t;
}

void
max_heapify(heap_t *hp, size_t i)
{
        size_t l, r, max;

        for (;;) {
                l = left(i);
                r = right(i);
                max = (l <= hp->size && elt(hp, l) > elt(hp, i) ? l : i);
                if (r <= hp->size && elt(hp, r) > elt(hp, max))
                        max = r;
                if (max == i)
                        break;
                swap(hp, i, max);
                i = max;
        }
}

heap_t *
build_maxheap(int *A, size_t len)
{
        heap_t *hp;
        size_t i;

        hp = nheap(A, len);
        for (i = hp->size >> 1 ; i; i--)
                max_heapify(hp, i);
        return hp;
}

void
hsort(int *A, size_t len)
{
        heap_t *hp;
        int i;

        hp = build_maxheap(A, len);
        for (i = hp->len; i > 1; i--) {
                swap(hp, 1, i);
                hp->size--;
                max_heapify(hp, 1);
        }
        free(hp);
}

int
main(int argc, char *argv[])
{
        int *A, i;

        if ((A = malloc(--argc * sizeof(*A))) == NULL)
                err(EXIT_FAILURE, "Not enough memory.");
        for (i = 0; i<argc; i++)
                A[i] = atoi(argv[i+1]);
        hsort(A, argc);
        for (i = 0; i < argc; i++)
                printf("%d%c", A[i], (i == argc-1 ? '\n' : ' '));
        free(A);
        return 0;
}
