#include <stdio.h>
#include <stdlib.h>

int BFS(int* Matrix, int* Result, int* Cond, int v, int* curFreeRes, int curVertex) {

	for (int i = 0; i < v; i++) {

		if (*(Matrix + curVertex * v + i) == 1) {
			//printf("found a way from %d to %d\n", curVertex + 1, i + 1);
			if (*(Cond + i) == 0) {
				*(Cond + i) = 1;
				int t = BFS(Matrix, Result, Cond, v, curFreeRes, i);
				if (t == -1) return -1;
			} else if (*(Cond + i) == 1) {
				return -1;
			}

		}
		//else {
		//	printf("can't find a way from %d to %d\n", curVertex + 1, i + 1);
		//}
	}
	//printf("ended BFS for %d\n", curVertex + 1);
	*(Cond + curVertex) = 2;
	*(Result + *(curFreeRes)) = curVertex + 1;
	(*curFreeRes)++;
	//printf("now %d vertex is in condition %d; added it in Result at position %d\n", curVertex + 1, *(Cond + curVertex), (*curFreeRes) - 1);
	return 0;
}

int main() {

	int v, e;
	int t = scanf("%d", &v);
	if ((t != 1) || (v < 0) || (v > 2000)) {
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

	int* Matrix = malloc(v * v * sizeof(int));
	int* Cond = malloc(v * sizeof(int));
	int* Result = malloc(v * sizeof(int));

	for (int i = 0; i < v; i++) {
		*(Cond + i) = 0;
		*(Result + i) = 0;
		for (int j = 0; j < v; j++) {
			*(Matrix + i * v + j) = 0;
		}
	}

	for (int i = 0; i < e; i++) {
		int from, to;
		t = scanf("%d %d", &from, &to);
		if ((from > v) || (from < 1) || (to > v) || (to < 0)) {
			printf("bad vertex");
			free(Matrix);
			free(Cond);
			free(Result);
			return 0;
		}
		if (t < 2) {
			printf("bad number of lines");
			free(Matrix);
			free(Cond);
			free(Result);
			return 0;
		}
		*(Matrix + (from - 1) * v + to - 1) = 1;
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
			//printf("calling BFS for %d vertex in condition %d\n", i + 1, *(Cond + i));
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
