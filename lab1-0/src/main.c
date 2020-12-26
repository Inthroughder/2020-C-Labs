#define _CRT_SECURE_NO_WARNINGS
#define BUFFER 409600

#include <stdio.h>
#include <string.h>

void Dfill(int *D, char *Q, int qlen) {
	for (int i = 0; i < 256; i++) {
		D[i] = qlen;
	}
	for (int i = 0; i < qlen - 1; i++) {
		D[(int)(Q[i])] = qlen - i - 1;
	}
}

int BMA(char *S, char *Q, int qlen, int *D) {
	int letters_summ = fread(S, sizeof(char), BUFFER, stdin);
	
	int gpos = qlen;
	int spos = qlen - 1;
	int qpos = qlen - 1;
	int t = 0;
	int pm = 0;
	while (letters_summ - gpos >= 0) {
		printf("%d ", gpos);
		if (S[spos] == Q[qpos]) {
			if (qpos == 0) {
				qpos = qlen - 1;
				int kostyl = 2 * qlen - 1;
				spos = spos + kostyl;
				gpos = gpos + kostyl;
				t = 0;
			} else {
				gpos--;
				qpos--;
				spos--;
				t++;
			}
		} else {
			int kostyl = D[(int)(S[spos + t])] + t;
			gpos = gpos + kostyl;
			spos = spos + kostyl;
			t = 0;
			qpos = qlen - 1;
		}
		
		if (gpos > letters_summ) {
			if (feof(stdin))
				return 0;
			int smove = qlen - gpos + letters_summ;
			for (int i = 0; i <= smove; i++) {
				S[i] = S[BUFFER + pm - smove + i];
			}
			spos = qlen - 1;
			int k = fread(S + smove, sizeof(char), BUFFER, stdin);
			letters_summ = letters_summ + k;
			pm = smove;
		}
	}
	return 0;
}

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
