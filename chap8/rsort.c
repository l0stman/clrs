#include <err.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char**
csort(unsigned char *A[], size_t len, size_t i)
{
        static size_t C[10];
        register size_t j;
        unsigned char **B;

        if ((B = malloc(len * sizeof(*B))) == NULL)
                err(EXIT_FAILURE, "Not enough memory");

        bzero(C, sizeof(C));
        for (j = 0; j<len; j++)
                C[A[j][i]]++;

        for (j = 1; j<sizeof(C)/sizeof(C[0]); j++)
                C[j] = C[j] + C[j-1];

        j = len;
        while (j--) {
                B[C[A[j][i]]-1] = A[j];
                C[A[j][i]]--;
        }
        return B;
}

static void
radix_sort(unsigned char *A[], size_t len, int digits)
{
        register int i;
        unsigned char **B;

        i = digits;
        while (i--) {
                B = csort(A, len, i);
                free(A);
                A = B;
        }
}

static unsigned char*
int2arr(int n, int digits)
{
        unsigned char *res, *p;

        if ((res = calloc(digits, sizeof(char))) == NULL)
                err(EXIT_FAILURE, "Not enough memory");
        p = res+digits;
        do
                *--p = n%10;
        while ((n /= 10) > 0);
        return res;
}

static int
arr2int(unsigned char *a, int digits)
{
        int res, radix;

        res = 0;
        radix = 1;
        while (--digits > 0) {
                res += radix * a[digits];
                radix *= 10;
        }
        return res;
}

int
main(int argc, char *argv[])
{
        unsigned char **A;
        int digits;
        register size_t i;

        if ((A = malloc(--argc * sizeof(*A))) == NULL)
                err(EXIT_FAILURE, "Not enough memory");

        digits = (int)log10(INT_MAX);
        for (i=0; i<argc; i++)
                A[i] = int2arr(atoi(argv[i+1]), digits);
        radix_sort(A, argc, digits);
        for (i=0; i<argc; i++) {
                printf("%d%c", arr2int(A[i], digits),
                       (i == argc-1 ? '\n' : ' '));
                free(A[i]);
        }
        free(A);
        return 0;
}
