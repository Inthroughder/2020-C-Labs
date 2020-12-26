#define _CRT_SECURE_NO_WARNINGS
#define BUFFER 409600

#include <stdio.h>
#include <string.h>
#include "header.h"

int main(void) {
	char Q[17];
	int D[256];
	char S[BUFFER + 16];

	if (scanf("%16[^\n]s", Q) == 0)
		return 0;

	int qlen = strlen(Q);

	Dfill(D, Q, qlen);

	int t = getchar();
	if (t == 0)
		return 0;

	BMA(S, Q, qlen, D);

	return 0;
}
