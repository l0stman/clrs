#include <sys/time.h>

#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SIZE	1000000
#define NELEMS(x)	(sizeof(x)/sizeof((x)[0]))

/* return a number between 0 and m. */
#define rand(m)	((int)(random() * 1.0 / 0x7fffffff * (m)))

static void isort(int *, int);
static void msort(int *, int, int);
static void bsort(int *, int);
static void quicksort(int *, int, int);
static void prtime(void (*)(int *, int), char);
static void usage();
static inline void exchange(int *, int, int);

static int arr[SIZE];
static int buf[SIZE];
static int size = NELEMS(buf);

static struct {
  char *name;
  void (*sort)();
  char arity;
} sorts[] = {
  {"quicksort", quicksort, 3},
  {"merge sort", msort, 3},
  {"insertion sort", isort, 2},
  {"bubble sort", bsort, 2}
};

int
main(int argc, char **argv)
{
  int c, i, qflag;

  qflag = 0;
  while ((c = getopt(argc, argv, "hqs:")) != -1) {
	switch (c) {
	case 'q':
	  qflag = 1;
	  break;
	case 's':
	  size = ((int)strtol(optarg, NULL, 10))%(SIZE+1);
	  break;
	case 'h':
	case '?':
	default:
	  usage();
	break;
	}
  }
  argc -= optind;
  argv += optind;

  srandom(time(NULL));
  if (qflag) {
	/* the array is quasi-sorted with less than 0.1% inversions */
	int i, j, inv;

	inv = (int)(0.001 * size);
	for (i = 0; i < size; i++)
	  buf[i] = i;
	while (inv--) {
	  i = rand(size-1);
	  j = (i+rand(size-1-i))%size;
	  exchange(buf, i, j);
	}
  } else
	for (i = 0; i < size; i++)
	  buf[i] = random();
  fprintf(stderr, "%s array of size %d\n",
		  (qflag ? "quasi-sorted": "random"), size);
  for (i = 0; i < NELEMS(sorts); i++) {
	memcpy(arr, buf, size * sizeof(buf[0]));
	fprintf(stderr, "\n-- %s: ", sorts[i].name);
	prtime(sorts[i].sort, sorts[i].arity);
  }
  return 0;
}

static void
usage(void)
{
  fprintf(stderr, "usage: cmpsort [-q] [-s size]\n");
  fprintf(stderr, "Compare the execution time of sorting algorithms where:\n");
  fprintf(stderr, "\t* q\t: the array is quasi-sorted (0.1%% inversions)\n");
  fprintf(stderr, "\t* size\t: size of the array up to 1,000,000\n");
  exit(1);
}

/* exchange: exchange two elements of the array. */
static inline void
exchange(int *A, int i, int j)
{
  int k = A[i];
  A[i] = A[j];
  A[j] = k;
}

/* prtime: print the running time of the sorting function on the array
   arr */
static void
prtime(void (*sort)(), char arity)
{
  struct timeval begin, end;

  gettimeofday(&begin, NULL);
  switch (arity) {
  case 2:
	sort(arr, size);
	break;
  case 3:
	sort(arr, 0, size-1);
	break;
  default:
	return;
	break;
  }
  gettimeofday(&end, NULL);

  end.tv_sec -= begin.tv_sec;
  end.tv_usec -= begin.tv_usec;
  if (end.tv_usec < 0)
	end.tv_sec--, end.tv_usec += 1000000;
  fprintf(stderr, "%jd.%02ld\n", (intmax_t)end.tv_sec, end.tv_usec/10000);
}
  
/* isort: sort the array A using insertion sort */
static void
isort(int *A, int len)
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

/* msort: sort the array A between the index p and r using merge sort. */
static void
msort(int *A, int p, int r)
{
  int q;
  
  if (p < r) {
	q = (p+r)/2;
	msort(A, p, q);
	msort(A, q+1, r);
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
  free(L);
  free(R);
}

/* bsort: sort the array A using bubble sort. */
static void
bsort(int *A, int len)
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

static int partition(int *, int, int);

/* quicksort: sort the array A with quicksort using randomized partition */
static void
quicksort(int *A, int p, int r)
{
  int q;
  
  if (p < r) {
	q = partition(A, p, r);
	quicksort(A, p, q-1);
	quicksort(A, q+1, r);
  }
}

static int
partition(int *A, int p, int r)
{
  int i, j, x;

  i = p + rand(r-p);
  exchange(A, i, r);
  
  x = A[r];
  i = p-1;
  for (j = p; j < r; j++)
	if (A[j] <= x) {
	  i++;
	  exchange(A, i, j);
	}
  exchange(A, i+1, r);
  return i+1;
}
