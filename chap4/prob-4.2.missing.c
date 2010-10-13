#include <stdio.h>
#include <stdlib.h>

#define bit(j, m)       (((m) >> (j)) & 1)

unsigned long
find_missing(unsigned long *A, unsigned int n)
{
        unsigned long *indexes;
        unsigned int len;
        unsigned long res;
        int count, total, i, j, k;
        char keep;

        if ((indexes = malloc(n*sizeof(*indexes))) == NULL) {
                printf("Not enough memory.\n");
                exit(1);
        }
        len = n;
        for (j = 0; j < len; j++)
                indexes[j] = j;

        res = 0;
        for (j = 0; (n>>j); j++) {
                for (count = 0, i = 0; i < len; i++)
                        count += bit(j, A[indexes[i]]);

                /* number of integers between 0 and n whose last j
                 * bits are equals to res and have the (j+1)-th bit
                 * equal to 1 */
                total = (n>>j);
                if (res > (n & ~(~0 << j)))
                        --total;
                total = (total+1)/2;

                if (count < total) {
                        /* the (j+1)-th bit of the missing integer is 1 */
                        res ^= 1<<j;
                        keep = 1;
                } else
                        keep = 0;

                for (k = 0, i = 0; i < len; i++)
                        if (bit(j, A[indexes[i]]) == keep)
                                indexes[k++] = indexes[i];
                len = k;
        }
        free(indexes);
        return res;
}

int
main(int argc, char *argv[])
{
        int i;
        unsigned long *A;

        if (--argc == 0)
                return 1;
        if ((A = malloc(argc * sizeof(*A))) == NULL) {
                printf("Not enough memory.\n");
                return 1;
        }
        for (i = 0; i < argc; i++)
                A[i] = (unsigned long)strtol(*++argv, NULL, 10);
        printf("%lu\n", find_missing(A, argc));
        return 0;
}
