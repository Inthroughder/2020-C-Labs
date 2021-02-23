#include <stdio.h>

int AD(int* A, int l, int r) {
	int p = A[(l + r) / 2];
	while (r >= l){
		while (A[l] < p) l++;
		while (A[r] > p) r--;
		if (r > l) {
			int t = A[l];
			A[l] = A[r];
			A[r] = t;
			l++;
			r--;
		} else {
			break;
		}
	}
	return r;
}

int QS(int* A, int l, int r) {
	while (l < r) {
		int m = AD(A, l, r);
		QS(A, l, m);
		l = m + 1;
	}
	return 0;
}
