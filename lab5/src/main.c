#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"
#define _CRT_SECURE_NO_WARNINGS
#define BUFFER 32 + 256 * 256 + 511 + 514

struct knot {
	int letter;
	int count;
	struct knot *pointer_l;
	struct knot *pointer_r;
};

int main(void) {
	FILE *fo, *fi;
	char mode[3];

	fi = fopen("in.txt", "rb");
	fo = fopen("out.txt", "wb");
	int t = fread(mode, sizeof(char), 3, fi);
	if (t == 0) { 
		fclose(fi);
		fclose(fo);
		return 0;
	}

	if (mode[0] == 'c') {

		struct knot HT[512 - 1];
		unsigned char Output[BUFFER] = { 0 };
		int CharacterTable[256] = { 0 };

		int inputLength = 0;
		int leaves = LetterCounter(CharacterTable, fi, &inputLength);
		if (inputLength == 0) {
			fclose(fi);
			fclose(fo);
			return 0;
		}

		HTBuilder(CharacterTable, HT);

		char* CodesTable[256];
		char* p = malloc(leaves * 257 * sizeof(char));
		for (int i = 0; i < 256; i++) {
			if (CharacterTable[i] != 0) {
				CodesTable[i] = p;
				p = p + 257;
			} else {
				CodesTable[i] = NULL;
			}
		}
	
		if (leaves == 1) {
			CodesTable[HT[0].letter] = "1\0";
		} else {
			char curCode[257] = { '\0' };
			CTBuilder(HT[leaves * 2 - 2], CodesTable, curCode, 0);
		}

		fwrite(&inputLength, 1, sizeof(int), fo);
		int counter = 0;
		HTCoder(&(HT[leaves * 2 - 2]), Output, &counter);

		fseek(fi, 0, SEEK_SET);
		int t = fread(mode, sizeof(char), 3, fi);
		if (t == 0) {
			fclose(fi);
			fclose(fo);
			return 0;
		}
		OutputCreator(Output, counter, fi, fo, CodesTable);

	} else if (mode[0] == 'd') {

		int inputLength;
		char t = fread(&inputLength, sizeof(int), 1, fi);
		if ((t == 0) || (inputLength == 0)) {
			fclose(fi);
			fclose(fo);
			return 0;
		}

		unsigned char Input[33] = { 0 };
		t = fread(Input, sizeof(char), 3, fi);
		if (t >= 1) t = 1;
		int curPos = 0;

		struct knot* startKnot = malloc(sizeof(struct knot));
		HTDecoder(startKnot, fi, &curPos, Input, &t);

		for (int i = 3; i < 33; i++) {
			if (t == 1) {
				t = fread(Input + i, sizeof(char), 1, fi);
			} else {
				break;
			}
		}
		Decoder(startKnot, fi, fo, Input, inputLength, curPos, t);
		MFree(startKnot);
	}

	fclose(fi);
	fclose(fo);

	return 0;
}
