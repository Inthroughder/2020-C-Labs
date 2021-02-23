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
	if (l < r) {
		int m = AD(A, l, r);
		QS(A, l, m);
		QS(A, m + 1, r);
	}
	return 0;
}

int main(void) {
	int n;
	int A[200];

	if (scanf("%d", &n) != 1) {
		return -1;
	}
	for (int i = 0; i < n; i++) {
		if (scanf("%d", &A[i]) != 1) {
			return -1;
		}
	}
	
	QS(A, 0, n - 1);

	for (int i = 0; i < n; i++) {
		printf("%d ", A[i]);
	}
	return 0;
}
