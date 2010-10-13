#include <err.h>
#include <stdio.h>
#include <stdlib.h>

static inline void*
scalloc(size_t num, size_t size)
{
        void *p;

        if ((p = calloc(num, size)) == NULL)
                err(EXIT_FAILURE, "Not enough memory");
        return p;
}

static inline void*
smalloc(size_t size)
{
        void *p;

        if ((p = malloc(size)) == NULL)
                err(EXIT_FAILURE, "Not enough memory");
        return p;
}

#define NEW(p)       ((p) = smalloc(sizeof *(p)))

typedef struct list {
        struct list *next;
        struct list *prev;
        double num;
} list_t;

void
bucket_sort(double A[], size_t len)
{
        list_t **buckets, *p, *key, *next;
        size_t i, j;

        buckets = scalloc(len, sizeof(*buckets));
        for (i = 0; i < len; i++) {
                j = (size_t)(len * A[i]);
                NEW(p);
                p->next = buckets[j];
                p->prev = NULL;
                p->num = A[i];
                if (buckets[j])
                        buckets[j]->prev = p;
                buckets[j] = p;
        }

        for (i = 0; i < len; i++)
                if (buckets[i])
                        for (key = buckets[i]->next; key; key = next) {
                                next = key->next;
                                key->prev->next = next;
                                if (next)
                                        next->prev = key->prev;
                                for (p = key->prev;
                                     p && p->num>key->num;
                                     p = p->prev)
                                        ;
                                if (p) {
                                        key->next = p->next;
                                        if (p->next)
                                                p->next->prev = key;
                                        p->next = key;
                                        key->prev = p;
                                } else {
                                        buckets[i]->prev = key;
                                        key->next = buckets[i];
                                        buckets[i] = key;
                                }
                        }

        for (i = 0, j = 0; j < len; j++)
                for (key = buckets[j]; key; key = p) {
                        p = key;
                        A[i++] = p->num;
                        p = p->next;
                        free(key);
                }
        free(buckets);
}

int
main(int argc, char *argv[])
{
        double *A;
        register size_t i;

        A = smalloc(--argc * sizeof(*A));
        for (i=0; i<argc; i++)
                A[i] = strtod(argv[i+1], NULL);
        bucket_sort(A, argc);
        for (i=0; i<argc; i++)
                printf("%e%c", A[i], (i == argc-1 ? '\n' : ' '));
        free(A);
        return 0;
}

