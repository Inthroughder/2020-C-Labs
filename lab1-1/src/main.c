#define _CRT_SECURE_NO_WARNINGS
#define BUFFER 409600

#include <stdio.h>
#include <string.h>
#include "header.h"

int main(void) {
	char Q[17];
	char S[BUFFER + 16];

	if (scanf("%16[^\n]s", Q) == 0)
		return 0;

	int qlen = strlen(Q);

	int t = getchar();
	if (t == 0)
		return 0;

	RKA(S, Q, qlen, hashf(Q, qlen));

	return 0;
}
