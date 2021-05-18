#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int HTDecoder(struct knot *curKnot, FILE* fi, int *curPos, unsigned char* Input, char* t) {

	while ((*curPos > 7) && (*t == 1)) {
		Input[0] = Input[1];
		Input[1] = Input[2];
		*t = fread(Input + 2, sizeof(char), 1, fi);
		*curPos = *curPos - 8;
	}
	unsigned char cur = 1 & (Input[*curPos / 8] >> (7 - *curPos % 8));
	(*curPos)++;
	if (cur == 1) {
		(*curKnot).letter = -1;
		(*curKnot).pointer_r = malloc(sizeof(struct knot));
		(*curKnot).pointer_l = malloc(sizeof(struct knot));
		HTDecoder((*curKnot).pointer_r, fi, curPos, Input, t);
		HTDecoder((*curKnot).pointer_l, fi, curPos, Input, t);
	} else {
		(*curKnot).pointer_r = NULL;
		(*curKnot).pointer_l = NULL;
		unsigned char c = (Input[*curPos / 8] << (*curPos % 8)) | (Input[*curPos / 8 + 1] >> (8 - *curPos % 8));
		*curPos = *curPos + 8;
		(*curKnot).letter = c;
	}
	return 0;
}

int Decoder(struct knot *startKnot, FILE* fi, FILE* fo, unsigned char* Input, int inputLength, int curPos, int t) {

	for (int i = 0; i < inputLength; i++) {
		while ((curPos > 7) && (t == 1)) {
			for (int i = 0; i < 32; i++) Input[i] = Input[i + 1];
			t = fread(Input + 32, sizeof(char), 1, fi);
			curPos = curPos - 8;
		}
		struct knot curKnot = *startKnot;
		while (curKnot.letter == -1) {
			unsigned char c = 1 & (Input[curPos / 8] >> (7 - curPos % 8));
			if (c == 1) {
				curKnot = *(curKnot.pointer_r);
			} else {
				curKnot = *(curKnot.pointer_l);
			}
			curPos++;
		}

		fwrite(&(curKnot.letter), sizeof(char), 1, fo);
	}
	return 0;
}

int MFree(struct knot* point) {
	if ((*point).pointer_l != NULL) {
		MFree((*point).pointer_l);
		MFree((*point).pointer_r);
		free(point);
	}
}
