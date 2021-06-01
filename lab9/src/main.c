#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int Dijkstra(unsigned int* Matrix, char* Marked, unsigned int* Distance, int v, unsigned int* Parent) {

	unsigned int min = 0;
	int curVert = 0;
	while (min <= INT_MAX) {
		min = (unsigned int)INT_MAX + (unsigned int)1;
		for (int i = 0; i < v; i++) {
			if ((Distance[i] < min) && (Marked[i] == 0)) {
				min = Distance[i];
				curVert = i;
			}
		}

		//printf("\n min is %u, curVert is %d", min, curVert);

		Marked[curVert] = 1;
		for (int i = 0; i < v; i++) {
			if (Matrix[curVert * v + i] < (unsigned int)INT_MAX + (unsigned int)1) {
				

				for (int i = 0; i < v; i++) {

					if (Matrix[curVert * v + i] < (unsigned int)INT_MAX + (unsigned int)1) {//edge check

						if (Distance[i] == (unsigned int)INT_MAX + (unsigned int)2) {//no path yet

							if (Matrix[curVert * v + i] + Distance[curVert] > INT_MAX) {
								Distance[i] = (unsigned int)INT_MAX + (unsigned int)1;
							}
							else {
								Distance[i] = Matrix[curVert * v + i] + Distance[curVert];
							}
							Parent[i] = curVert;

						}
						else {//path already exists

							if (Matrix[curVert * v + i] + Distance[curVert] < Distance[i]) {
								Distance[i] = Matrix[curVert * v + i] + Distance[curVert];
								Parent[i] = curVert;
							}

						}

					}
				}


			}
		}
	}

	return 0;
}

int main() {

	int v, s, f, e;

	int t = scanf("%d", &v);
	if (t < 1) {
		printf("bad number of lines");
		return 0;
	}
	if ((v < 0) || (v > 5000)) {
		printf("bad number of vertices");
		return 0;
	}

	t = scanf("%d %d", &s, &f);
	if (t < 1) {
		printf("bad number of lines");
		return 0;
	}
	if ((s < 1) || (s > v) || (f < 1) || (f > v)) {
		printf("bad vertex");
		return 0;
	}

	t = scanf("%d", &e);
	if (t < 1) {
		printf("bad number of lines");
		return 0;
	}
	if (e > (v * (v + 1) / 2)) {
		printf("bad number of edges");
		return 0;
	}

	unsigned int* Matrix = malloc(v * v * sizeof(unsigned int));
	for (int i = 0; i < v; i++) {
		for (int j = 0; j < v; j++) {
			Matrix[i * v + j] = (unsigned int)INT_MAX + (unsigned int)1;
		}
	}

	for (int i = 0; i < e; i++) {
		int from, to, length;
		t = scanf("%d %d %d", &from, &to, &length);
		if (t < 3) {
			printf("bad number of lines");
			free(Matrix);
			return 0;
		}
		if ((from < 1) || (from > v) || (to < 1) || (to > v)) {
			printf("bad vertex");
			free(Matrix);
			return 0;
		}
		if ((length > INT_MAX) || (length < 0)) { //or length < 1?
			printf("bad length");
			free(Matrix);
			return 0;
		}
		Matrix[(from - 1) * v + to - 1] = (unsigned int)length;
		Matrix[(to - 1) * v + from - 1] = (unsigned int)length;
	}

	char* Marked = malloc(v * sizeof(char));
	unsigned int* Distance = malloc(v * sizeof(unsigned int));
	unsigned int* Parent = malloc(v * sizeof(unsigned int));

	for (int i = 0; i < v; i++) {
		Marked[i] = 0;
		Distance[i] = (unsigned int)INT_MAX + (unsigned int)2;
	}
	Distance[s - 1] = 0;

	Dijkstra(Matrix, Marked, Distance, v, Parent);

	//printf("\n\nDistances: ");
	//for (int i = 0; i < v; i++) {
	//	printf("%u ", Distance[i]);
	//}
	//printf("\n\n");

	for (int i = 0; i < v; i++) {
		if (Distance[i] == (unsigned int)INT_MAX + (unsigned int)2) {
			printf("oo ");
		}
		else if (Distance[i] == (unsigned int)INT_MAX + (unsigned int)1) {
			printf("INT_MAX+ ");
		}
		else {
			printf("%u ", Distance[i]);
		}
	}

	if (Distance[f - 1] == (unsigned int)INT_MAX + (unsigned int)2) {
		printf("\nno path");
	}
	else if (Distance[f - 1] == (unsigned int)INT_MAX + (unsigned int)1) {
		printf("\noverflow");
	}
	else {
		printf("\n");
		f--;
		s--;
		//printf("\n%u", Distance[f - 1]); //path
		while (f != s) {
			printf("%d ", f + 1);
			f = Parent[f];
		}
		printf("%d", s + 1);
	}

	free(Matrix);
	free(Marked);
	free(Distance);
	free(Parent);

	return 0;
}
