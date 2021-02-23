#include <stdio.h>
#include "header.h"

#define BUFFER 2000000

int main(void) {
	int n;
	int A[BUFFER];

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
