#define _CRT_SECURE_NO_WARNINGS
#define BUFFER 409600

#include <stdio.h>
#include <string.h>
#include <math.h>

int hashf(char *S, int slen) {
	int hash = 0;
	for (int i = 0; i < slen; i++) {
		hash = hash + ((unsigned char)(S[i]) % 3) * (pow(3, i));
	}
	return hash;
}

int RKA(char *S, char *Q, int qlen, int qhash) {
	int gpos = 1;
	int spos = 0;
	int pwr = pow(3, qlen - 1);
	int letters_summ = fread(S, sizeof(char), BUFFER, stdin);
	printf("%d", qhash);
	int shash = hashf(S, qlen);
	while (gpos < letters_summ - qlen + 2) {
		if (shash == qhash) {
			for (int i = 0; i < qlen; i++) {
				printf(" %d", gpos + i);
				if (S[spos + i] != Q[i])
					break;
			}
		}

		gpos++;
		spos++;
		shash = (shash - ((unsigned char)(S[spos - 1]) % 3)) / 3 + ((unsigned char)(S[spos + qlen - 1]) % 3) * pwr;

		if (gpos == letters_summ - qlen + 1) {
			for (int i = 0; i < qlen; i++) {
				S[i] = S[spos + i];
			}
			letters_summ = letters_summ + fread(S + qlen, sizeof(char), BUFFER, stdin);
			spos = 0;
		}
	}
	return 0;
}
