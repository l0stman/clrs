#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* insertion_sort: sort the array A using insertion sort */
static void
insertion_sort(int A[], int len)
{
  int i, j, key;

  for (j = 1; j < len; j++) {
	key = A[j];
	/* insert A[j] into the sorted sequence A[1], ..., A[j-1] */
	for (i = j-1; i >= 0 && A[i] > key; i--)
	  A[i+1] = A[i];
	A[i+1] = key;
  }
}

static void merge(int *, int, int, int);

/* merge_sort: sort the array A between the index p and r */
static void
merge_sort(int A[], int p, int r)
{
  int q;
  
  if (p < r) {
	q = (p+r)/2;
	merge_sort(A, p, q);
	merge_sort(A, q+1, r);
	merge(A, p, q, r);
  }
}

#define INF	INT_MAX

/* merge: merge two sorted lists into a sorted list */
static void
merge(int *A, int p, int q, int r)
{
  int n1, n2, i, j, k;
  int *L, *R;
  
  n1 = q-p+1;
  n2 = r-q;
  if ((L = malloc(sizeof(*L) * (n1+1))) == NULL ||
	  (R = malloc(sizeof(*R) * (n2+1))) == NULL)
	exit(1);
  for (i = 0; i < n1; i++)
	L[i] = A[p+i];
  for (j = 0; j < n2; j++)
	R[j] = A[q+j+1];
  L[n1] = INF;
  R[n2] = INF;
  i = j = 0;
  for (k = p; k <= r; k++) {
	if (L[i] <= R[j])
	  A[k] = L[i++];
	else
	  A[k] = R[j++];
  }
}

/* bubble_sort: sort the array A */
static void
bubble_sort(int A[], int len)
{
  int i, j, k;

  for (i = 0; i < len; i++)
	for (j = len-1; j >= i+1; j--)
	  if (A[j] < A[j-1]) {
		k = A[j-1];
		A[j-1] = A[j];
		A[j] = k;
	  }
}

int
main(int argc, char *argv[])
{
  int *A;
  int i;

  if ((A = malloc(sizeof(*A) * --argc)) == NULL) {
	perror("malloc");
	return 1;
  }
  for (i = 0; i < argc; i++)
	A[i] = atoi(argv[i+1]);
  bubble_sort(A, argc);
  for (i = 0; i < argc; i++)
	printf("%d%c", A[i], (i == argc-1 ? '\n': ' '));
  return 0;
}
