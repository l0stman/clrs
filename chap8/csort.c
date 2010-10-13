#include <err.h>
#include <stdio.h>
#include <stdlib.h>

static void
csort(int *A, int *B, size_t max, size_t len)
{
        register size_t i;
        size_t *C;

        if ((C = calloc(max+1, sizeof(int))) == NULL)
                err(EXIT_FAILURE, "Not enough memory");
        for (i = 0; i < len; i++)
                C[A[i]]++;

        /* C[i] now contains the number of elements equal to i. */
        for (i = 1; i <= max; i++)
                C[i] = C[i] + C[i-1];

        /* C[i] now contains the number of elements less than or equal to i. */
        i = len;
        while (i--) {
                B[C[A[i]]-1] = A[i];
                C[A[i]]--;
        }
        free(C);
}

int
main(int argc, char *argv[])
{
        int *A, *B, max;
        register size_t i;

        if ((A = malloc(--argc * sizeof(*A) * 2)) == NULL)
                err(EXIT_FAILURE, "Not enough memory.");
        B = A+argc;

        for (i = 0; i<argc; i++)
                A[i] = atoi(argv[i+1]);
        for (max = 0, i = 0; i<argc; i++)
                if (A[i] > max)
                        max = A[i];

        csort(A, B, max, argc);
        for (i = 0; i<argc; i++)
                printf("%d%c", B[i], (i == argc-1 ? '\n' : ' '));

        free(A);
        return 0;
}
