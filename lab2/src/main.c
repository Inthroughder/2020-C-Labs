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

int main() {
	int n, plen, res;
	char S[12];
	int P[11];
	int Check[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	res = scanf("%11[^\n]s", S);
	plen = strlen(S);
	res = scanf("%7d", &n);
	res = 0;

	for (int i = 0; i < plen; i++) {
		if ((S[i] < '0') || (S[i] > '9') || (Check[S[i] - 48] == 1)) {
			printf("bad input");
			return 0;
		} else {
			P[i] = S[i] - 48;
			Check[S[i] - 48] = 1;
		}
	}

	if (n == 0) {
		return 0;
	}

	for (int i = 0; i < n; i++) {
		int j, t = 0, min = 1000, ind = 0;

		for (j = plen - 2; j >= 0; j--) {
			if (P[j] < P[j + 1]) {
				t = 1;
				break;
			}
		}

		if (t == 0) {
			return 0;
		}

		for (int k = j + 1; k < plen; k++) {
			if ((P[k] < min) && (P[k] > P[j])){
				min = P[k];
				ind = k;
			}
		}

		t = P[ind];
		P[ind] = P[j];
		P[j] = t;

		sorter(P, j + 1, plen - 1);

		for (res = 0; res < plen - 1; res++) {
			printf("%d", P[res]);
		}
		printf("%d\n", P[plen - 1]);
	}

	return 0;
}
