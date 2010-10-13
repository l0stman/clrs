#include <err.h>
#include <stdio.h>
#include <stdlib.h>

static inline void
swap(int *A, size_t i, size_t j)
{
        int t = A[i];
        A[i] = A[j];
        A[j] = t;
}

/*
 * Modification of counting-sort that sorts in place but is not a
 * stable sort.
 */
static void
csort(int *A, size_t max, size_t len)
{
        register size_t i;
        size_t *B, *C;

        if ((B = calloc(2*(max+1), sizeof(int))) == NULL)
                err(EXIT_FAILURE, "Not enough memory");
        C = B+max+1;
        for (i = 0; i < len; i++)
                B[A[i]]++;

        /* B[i] now contains the number of elements equal to i. */
        for (C[0] = B[0], i = 1; i <= max; i++)
                C[i] = B[i] + C[i-1];

        /* C[i] now contains the number of elements less than or equal to i. */
        for (i = max; i; i--) {
                while (i && !B[i])
                        i--;
                if (!i)
                        break;
                /* Place in the array the elements equal to i. */
                while (B[i]) {
                        int j;
                        while (A[C[i]-1] == i)
                                C[i]--, B[i]--;
                        j = C[i]-1;
                        if (B[i]) {
                                while (A[C[A[j]]-1] != i) {
                                        B[A[j]]--;
                                        swap(A, --C[A[j]], j);
                                }
                                B[i]--, B[A[j]]--;
                                swap(A, --C[A[j]], --C[i]);
                        }
                }
        }

        free(B);
}

int
main(int argc, char *argv[])
{
        int *A, max;
        register size_t i;

        if ((A = malloc(--argc * sizeof(*A))) == NULL)
                err(EXIT_FAILURE, "Not enough memory.");

        for (i = 0; i<argc; i++)
                A[i] = atoi(argv[i+1]);
        for (max = 0, i = 0; i<argc; i++)
                if (A[i] > max)
                        max = A[i];

        csort(A, max, argc);
        for (i = 0; i<argc; i++)
                printf("%d%c", A[i], (i == argc-1 ? '\n' : ' '));

        free(A);
        return 0;
}
