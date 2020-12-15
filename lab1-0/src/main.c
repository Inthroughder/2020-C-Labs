#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

int BMA(char* Q, int qlen, int* S, int* D, int spos) {
	for (int i = qlen - 1; i >= 0; i--) {
		printf("%d ", spos - qlen + i + 1);
		if (Q[i] != S[i]) {
			return D[S[qlen - 1]];
		}
	}
	return qlen;
}

int SfBMA(char* Q, int qlen, int* S, int* D, int add, int spos) {
	//creating S to find Q in it
	for (int i = 0; i < (qlen - add); i++) {
		S[i] = S[i + add];
	}
	for (int i = qlen - add; i < qlen; i++) {
		int c = getchar();
		if (c == EOF) return 0;
		S[i] = c;
	}
	spos = spos + add;

	//now the algorithm + recursion
	int newadd = BMA(Q, qlen, S, D, spos);
	SfBMA(Q, qlen, S, D, newadd, spos);
	return 0;
}


int main() {
	char Q[17];
	int D[257];
	int S[17];
	int h;
	h = scanf("%16[^\n]s", Q);
	if (h == 0) return 0;
	int qlen = strlen(Q);
	for (int i = 0; i < 257; i++) {
		D[i] = qlen;
	} // ZAI func
	for (int i = 0; i < qlen - 1; i++) {
		D[(int)(Q[i])] = qlen - i - 1;
	} // TAI func
	int t = getchar();
	if (t == EOF) return 0;

	// BMA is rec
	SfBMA(Q, qlen, S, D, qlen, 0);// template - size of template - current string (0) - alphabet - how much to add - current position
	return 0;
}
