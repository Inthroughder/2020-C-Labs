#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

int BMA(char* Q, int qlen, char* S, int* D, int spos) {
	for (int i = qlen - 1; i >= 0; i--) {
		printf("%d ", spos - qlen + i + 1);
		if (Q[i] != S[i]) {
			return D[S[qlen - 1]];
		}
	}
	return qlen;
}

int SfBMA(char* Q, int qlen, char* S, int* D, int add, int spos) {
	//creating S to find Q in it
	for (int i = 0; i < (qlen - add); i++) {
		S[i] = S[i + add];
	}
	for (int i = qlen - add; i < qlen; i++) {
		char c = getchar();
		if (c == EOF) return 0;
		S[i] = c;
	}
	spos = spos + add;

	//now the algorithm + recursion
	int newadd = BMA(Q, qlen, S, D, spos);
	SfBMA(Q, qlen, S, D, newadd, spos);
}


int main() {
	char Q[17];
	int D[257];
	char S[17];
	int h;
	h = scanf("%[^\n]s", Q);
	if (h == 0) return 0;
	int qlen = strlen(Q);
	for (int i = 0; i < 257; i++) {
		D[i] = qlen;
	} // ZAI func
	for (int i = 0; i < qlen - 1; i++) {
		D[(int)(Q[i])] = qlen - i - 1;
	} // TAI func
	char t = getchar();

	// BMA is rec
	SfBMA(Q, qlen, S, D, qlen, 0);// template - size of template - current string (0) - alphabet - how much to add - current position
	return 0;
}
