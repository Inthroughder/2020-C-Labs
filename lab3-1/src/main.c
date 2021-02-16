#include <stdio.h>

int QS(int* A, int l, int r) {
	while (l < r) {
		int m = AD(A, l, r);
		QS(A, l, m - 1);
		l = m + 1;
	}
	return 0;
}

int AD(int* A, int l, int r) {
	int res = l;
	int k = r - l;
	l++;
	for (int i = 0; i < k; i++) {
		if (A[l] <= A[res]) {
			l++;
			continue;
		}
		if (A[r] >= A[res]) {
			r--;
			continue;
		}
		int t = A[r];
		A[r] = A[l];
		A[l] = t;
		r--;
		l++;
		i++;
	}

	int t = A[r];
	A[r] = A[res];
	A[res] = t;

	return r;
}

int main() {
	int n;
	int A[200];
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d", &A[i]);
	}
	
	QS(A, 0, n - 1);

	for (int i = 0; i < n; i++) {
		printf("%d ", A[i]);
	}
	return 0;
}
