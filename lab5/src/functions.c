#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUFFER 32 + 256 * 256 + 511 + 514

struct knot {
	int letter;
	int count;
	struct knot *pointer_l;
	struct knot *pointer_r;
};

//coding

int LetterCounter(int* CharacterTable, FILE* f, int* inputLength) {
	int leaves = 0;
	int t = fgetc(f);
	while (t != EOF) {
		if (CharacterTable[t] == 0) leaves++;
		CharacterTable[t]++;
		(*inputLength)++;
		t = fgetc(f);
	}
	return leaves;
}

int HTSorter(struct knot* Tree, int leaves) {

	for (int i = 1; i < leaves; i++) {
		int j = i - 1;
		struct knot cur = Tree[i];
		while ((j >= 0) && (cur.count < Tree[j].count)) {
			Tree[j + 1] = Tree[j];
			j--;
		}
		Tree[j + 1] = cur;
	}

	return 0;
}

int HTBuilder(int* CharacterTable, struct knot* Tree) {

	int leaves = 0;
	for (int i = 0; i < 256; i++) {
		if (CharacterTable[i] != 0) {
			Tree[leaves].letter = i;
			Tree[leaves].count = CharacterTable[i];
			Tree[leaves].pointer_l = NULL;
			Tree[leaves].pointer_r = NULL;
			leaves++;
		}
	}
	HTSorter(Tree, leaves);

	for (int i = 0; i < leaves - 1; i++) {
		Tree[leaves + i].letter = -1;
		Tree[leaves + i].count = Tree[2 * i].count + Tree[2 * i + 1].count;
		Tree[leaves + i].pointer_l = Tree + 2 * i;
		Tree[leaves + i].pointer_r = Tree + 2 * i + 1;
		HTSorter(Tree + i * 2 + 2, leaves - i - 1);
	}

	return 0;
}


int CTBuilder(struct knot curKnot, char** CodesTable, char* curCode, int curCodeLen) {
	
	if (curKnot.pointer_l == NULL) {
		for (int i = 0; i <= curCodeLen; i++) {
			CodesTable[curKnot.letter][i] = curCode[i];
		}
	} else {
		curCode[curCodeLen] = '0';
		curCode[curCodeLen + 1] = '\0';
		CTBuilder(*(curKnot.pointer_l), CodesTable, curCode, curCodeLen + 1);
		curCode[curCodeLen] = '1';
		curCode[curCodeLen + 1] = '\0';
		CTBuilder(*(curKnot.pointer_r), CodesTable, curCode, curCodeLen + 1);
	}

	return 0;
}

int HTCoder(struct knot *curKnot, unsigned char* Output, int *counter) {
	if ((*curKnot).pointer_l != NULL) {
		Output[*counter / 8] = Output[*counter / 8] | (1 << (7 - *counter % 8));
		(*counter)++;
		HTCoder((*curKnot).pointer_r, Output, counter);
		HTCoder((*curKnot).pointer_l, Output, counter);
	}
	else {
		(*counter)++;
		for (int i = 0; i < 8; i++) {
			if (((*curKnot).letter >> (7 - i)) & 1) {
				Output[*counter / 8] = Output[*counter / 8] | (1 << (7 - *counter % 8));
			}
			(*counter)++;
		}
	}
	return 0;
}

int OutputCreator(unsigned char* Output, int counter, FILE* fi, FILE* fo, char** CodesTable);

int OutputWriter(int mode, unsigned char* Output, int counter, FILE* fi, FILE* fo, char** CodesTable) {
	if (mode) {
		fwrite(Output, sizeof(char), counter / 8, fo);
		if (counter % 8 != 0) {
			fwrite(Output + (counter / 8), sizeof(char), 1, fo);
		}
	}
	else {
		fwrite(Output, sizeof(char), counter / 8, fo);
		Output[0] = Output[counter / 8];
		memset(Output + 1, 0, BUFFER - 1);
		OutputCreator(Output, counter % 8, fi, fo, CodesTable);
	}
	return 0;
}

int OutputCreator(unsigned char* Output, int counter, FILE* fi, FILE* fo, char** CodesTable) {
	while (counter < BUFFER - 8) {
		int c = fgetc(fi);
		if (c == EOF) {
			OutputWriter(1, Output, counter, fi, fo, CodesTable);
			return 0;
		}

		for (unsigned int i = 0; i < strlen(CodesTable[c]); i++) {
			if (CodesTable[c][i] == '1') {
				Output[counter / 8] = Output[counter / 8] | (1 << (7 - counter % 8));
			}
			counter++;
		}
	}
	OutputWriter(0, Output, counter, fi, fo, CodesTable);
	return 0;
}

//decoding

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
	return 0;
}
