#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int Dijkstra(unsigned int* Matrix, char* Marked, unsigned int* Distance, int v, unsigned int* Parent, int f, int s) {

	unsigned int pathsToFinish = 0;
	unsigned int min = 0;
	int curVert = s;
	while (min < ((unsigned int)INT_MAX + (unsigned int)1)) {

		Marked[curVert] = 1;
		for (int i = 0; i < v; i++) {
			if (Matrix[curVert * v + i] < ((unsigned int)INT_MAX + (unsigned int)1)) {

				if (i == f) pathsToFinish++;

				if (Distance[i] == ((unsigned int)INT_MAX + (unsigned int)2)) {


					if ((Matrix[curVert * v + i] + Distance[curVert]) > INT_MAX) {
						Distance[i] = (unsigned int)INT_MAX + (unsigned int)1;
					} else {
						Distance[i] = Matrix[curVert * v + i] + Distance[curVert];
					}

					Parent[i] = curVert;
					}
				else {

					if (Matrix[curVert * v + i] + Distance[curVert] < Distance[i]) {
						Distance[i] = Matrix[curVert * v + i] + Distance[curVert];
						Parent[i] = curVert;
					}

				}

			}
		}

		min = (unsigned int)INT_MAX + (unsigned int)1;
		for (int i = 0; i < v; i++) {
			if ((Distance[i] < min) && (Marked[i] == 0)) {
				min = Distance[i];
				curVert = i;
			}
		}

	}

	return pathsToFinish;
}
