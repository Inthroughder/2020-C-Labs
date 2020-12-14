#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

void BMA(char* Q, int qlen, char* S, int slen, int* D) {
	int spos = qlen - 1;
	while (spos < slen) {
		int inpos = qlen - 1;
		while (1) {
			printf("%d ", spos - qlen + 1 + inpos + 1);
			if (Q[inpos] == S[spos - qlen + 1 + inpos]) {
				inpos--;
			}
			else {
				spos = spos + D[S[spos]];
				break;
			}

			if (inpos == -1) {
				spos = spos + D[S[spos]];
				break;
			}
		}
		
	}
}


int main() {
	unsigned char Q[17];
	int D[257];
	unsigned char S[1000000];
	int h;
	h = scanf("%16s", Q);
	if (h == 0) return 0;
	int qlen = strlen(Q);
	for (int i = 0; i < 257; i++) {
		D[i] = qlen;
	} // ZAI func
	for (int i = 0; i < qlen - 1; i++) {
		D[Q[i]] = qlen - i - 1;
	} // TAI func
	
	char c, t;
	t = getchar();
	c = getchar();
	int slen = 0;
	if (t == EOF) return 0;
	while (c != EOF) {
		S[slen] = c;
		slen++;
		c = getchar();
	}

	
	BMA(Q, qlen, S, slen, D);
	return 0;
}
