#include <stdio.h>
#include <string.h>

void sorter(int* A, int begin, int end) {
	int l = begin, r = end;
	int v = A[l + (r - l) / 2];
	while (l <= r) {
		while (A[l] < v) l++;
		while (A[r] > v) r--;
		if (l <= r) {
			int t = A[l];
			A[l] = A[r];
			A[r] = t;
			l++;
			r--;
		}
	}
	if (begin < r) sorter(A, begin, r);
	if (l < end) sorter(A, l, end);
}
