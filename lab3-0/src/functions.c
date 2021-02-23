#include <stdio.h>

int HC(int *A, int n, int root) {
	int l = 2 * root + 1;
	int r = 2 * root + 2;
	int cur = root;

	if ((l < n) && (A[l] > A[cur])) cur = l;
	if ((r < n) && (A[r] > A[cur])) cur = r;

	if (root != cur) {
		int t = A[root];
		A[root] = A[cur];
		A[cur] = t;

		HC(A, n, cur);
	}

	return 0;
}

int HS(int *A, int n) {
	for (int i = (n / 2) - 1; i >= 0; i--) HC(A, n, i);

	for (int i = n - 1; i >= 0; i--) {
		int t = A[0];
		A[0] = A[i];
		A[i] = t;
		HC(A, i, 0);
	}
	return 0;
}
