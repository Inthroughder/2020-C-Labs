#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int main() {

	int v, e;
	int t = scanf("%d", &v);
	if (t < 1) {
		printf("bad number of lines");
		return 0;
	} else if ((v < 0) || (v > 2000)){
		printf("bad number of vertices");
		return 0;
	}
	t = scanf("%d", &e);
	if (t != 1) {
		printf("bad number of lines");
		return 0;
	}
	if ((e < 0) || (e > v * (v + 1) / 2)) {
		printf("bad number of edges");
		return 0;
	}

	char* Matrix = malloc(((v * v / 8) + 1) * sizeof(char));
	char* Cond = malloc(v * sizeof(char));
	int* Result = malloc(v * sizeof(int));

	for (int i = 0; i < v; i++) {
		*(Cond + i) = 0;
		*(Result + i) = 0;
		
	}

	for (int i = 0; i < (v * v / 8) + 1; i++) {
		*(Matrix + i) = 0;
	}

	for (int i = 0; i < e; i++) {
		int from, to;
		t = scanf("%d %d", &from, &to);

		if (t < 2) {
			printf("bad number of lines");
			free(Matrix);
			free(Cond);
			free(Result);
			return 0;
		}

		if ((from > v) || (from < 1) || (to > v) || (to < 0)) {
			printf("bad vertex");
			free(Matrix);
			free(Cond);
			free(Result);
			return 0;
		}
		
		*(Matrix + ((from - 1) * v + to - 1) / 8) = *(Matrix + ((from - 1) * v + to - 1) / 8) | (1 << (7 - ((from - 1) * v + to - 1) % 8));
	}


	if (v == 0) {
		printf("0");
		free(Matrix);
		free(Cond);
		free(Result);
		return 0;
	}

	int curFreeRes = 0;
	for (int i = 0; i < v; i++) {
		if (*(Cond + i) != 2) {
			t = BFS(Matrix, Result, Cond, v, &curFreeRes, i);
		}

		if (t == -1) {
			printf("impossible to sort");
			free(Matrix);
			free(Cond);
			free(Result);
			return 0;
		}
	}

	for (int i = v - 1; i > -1; i--) {
		printf("%d ", *(Result + i));
	}

	free(Matrix);
	free(Cond);
	free(Result);

	return 0;
}
