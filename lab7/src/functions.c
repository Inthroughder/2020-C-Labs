#include <stdio.h>
#include <stdlib.h>

int BFS(char* Matrix, int* Result, char* Cond, int v, int* curFreeRes, int curVertex) {

	for (int i = 0; i < v; i++) {
		char t = 1 & ((*(Matrix + (curVertex * v + i) / 8)) >> (7 - (curVertex * v + i) % 8));
		if (t == 1) {
			if (*(Cond + i) == 0) {
				*(Cond + i) = 1;
				int t = BFS(Matrix, Result, Cond, v, curFreeRes, i);
				if (t == -1) return -1;
			} else if (*(Cond + i) == 1) {
				return -1;
			}

		}

	}
	*(Cond + curVertex) = 2;
	*(Result + *(curFreeRes)) = curVertex + 1;
	(*curFreeRes)++;
	return 0;
}
