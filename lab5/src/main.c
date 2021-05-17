#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS
#define BUFFER 32 + 256 * 8 + 510 + 514 // first is length; seconds and third is tree; last is whatever

struct knot {
	int letter;
	int count;
	struct knot *pointer_l;
	struct knot *pointer_r;
};

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
		Tree[leaves + i].pointer_l = &Tree[2 * i];
		Tree[leaves + i].pointer_r = &Tree[2 * i + 1];
		HTSorter(&Tree[i * 2 + 2], leaves - i - 1);
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
		Output[*counter / 8] = Output[*counter / 8] | (1 << (7 - *counter % 8)); // that's 1
		(*counter)++;
		HTCoder((*curKnot).pointer_r, Output, counter);
		HTCoder((*curKnot).pointer_l, Output, counter);
	}
	else {
		(*counter)++;
		for (int i = 0; i < 8; i++) {
			if (((*curKnot).letter >> (7 - i)) & 1) {
				Output[*counter / 8] = Output[*counter / 8] | (1 << (7 - *counter % 8)); // that's 1
			}
			(*counter)++;
		}
	}
	return 0;
}

int OutputCreator(unsigned char* Output, int counter, FILE* fi, FILE* fo, char** CodesTable);

int OutputWriter(int mode, unsigned char* Output, int counter, FILE* fi, FILE* fo, char** CodesTable) {
	if (mode) {
		//last
		fwrite(Output, sizeof(char), counter / 8, fo);
		if (counter % 8 != 0) {
			char t = Output[counter / 8];
			t = ((t >> (8 - counter % 8)) << (8 - counter % 8));
			fwrite(&t, sizeof(char), 1, fo);
		}
	}
	else {
		//mid
		fwrite(Output, sizeof(char), counter / 8, fo);
		Output[0] = Output[counter / 8];
		memset(&Output[1], 0, BUFFER - 1);
		OutputCreator(Output, counter % 8, fi, fo, CodesTable);
	}
	return 0;
}

int OutputCreator(unsigned char* Output, int counter, FILE* fi, FILE* fo, char** CodesTable) {
	while (counter < BUFFER - 8) {
		int c = fgetc(fi);
		if (c == EOF) {
			OutputWriter(1, Output, counter, fi, fo, CodesTable); //last output
			return 0;
		}

		for (unsigned int i = 0; i < strlen(CodesTable[c]); i++) {
			if (CodesTable[c][i] == '1') {
				Output[counter / 8] = Output[counter / 8] | (1 << (7 - counter % 8)); // that's 1
			}
			counter++;
		}
	}
	OutputWriter(0, Output, counter, fi, fo, CodesTable); //mid-writing output
	return 0;
}

int HTDecoder(struct knot *startKnot, struct knot *curKnot, int* curKnotMem, FILE* fi, int *curPos, unsigned char* Input, int* t) {
	if ((*curPos > 7) && (*t)) {
		Input[0] = Input[1];
		Input[1] = Input[2];
		*t = fread(Input + 2, sizeof(char), 1, fi);
		*curPos = *curPos - 8;
	}
	unsigned char cur = 1 & (Input[*curPos / 8] >> (7 - *curPos % 8));
	(*curPos)++;
	if (cur == 1) {
		(*curKnot).letter = -1;
		(*curKnot).pointer_r = startKnot + *curKnotMem + 1;
		(*curKnot).pointer_l = startKnot + *curKnotMem + 2;
		*curKnotMem = *curKnotMem + 2;
		HTDecoder(startKnot, (*curKnot).pointer_r, curKnotMem, fi, curPos, Input, t);
		HTDecoder(startKnot, (*curKnot).pointer_l, curKnotMem, fi, curPos, Input, t);
	} else {
		(*curKnot).pointer_r = NULL;
		(*curKnot).pointer_l = NULL;
		unsigned char c = 0;
		
		if (*curPos % 8 == 0) {
			c = Input[*curPos / 8];
		} else {
			c = (Input[*curPos / 8] << (*curPos % 8)) | (Input[*curPos / 8 + 1] >> (8 - *curPos % 8));
		}
		*curPos = *curPos + 8;
		(*curKnot).letter = c;
	}
	return 0;
}

int Decoder(struct knot *startKnot, FILE* fi, FILE* fo, unsigned char* Input, int inputLength, int curPos, int t) {

	for (int i = 0; i < inputLength; i++) {
		while ((curPos > 7) && (t)) {
			Input[0] = Input[1];
			Input[1] = Input[2];
			t = fread(Input + 2, sizeof(char), 1, fi);
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

int main(void) {
	FILE *fo, *fi;
	struct knot HT[512 - 1];
	char mode[3];

	fi = fopen("in.txt", "rb");
	fo = fopen("out.txt", "wb");
	int t = fread(mode, sizeof(char), 3, fi);
	if (t == 0) return 0;

	if (mode[0] == 'c') {

		unsigned char Output[BUFFER] = { 0 };
		int CharacterTable[256] = { 0 };

		int inputLength = 0;
		int leaves = LetterCounter(CharacterTable, fi, &inputLength);
		if (inputLength == 0) return 0;

		HTBuilder(CharacterTable, HT);

		//debug
		//printf("\n\ninputLength = %d; HT = \n\n", inputLength);
		//for (int i = 0; i < 2 * leaves - 1; i++) {
		//	printf("\nHT[%d] = %X\n", i, HT[i].letter);
		//}

		char* CodesTable[256];
		char* p = malloc(leaves * 257 * sizeof(char));
		for (int i = 0; i < 256; i++) {
			if (CharacterTable[i] != 0) {
				CodesTable[i] = p;
				p = p + 257;
			}
			else {
				CodesTable[i] = NULL;
			}
		}
		char curCode[257] = { '\0' };
		if (leaves == 1) {
			CodesTable[HT[0].letter] = "1\0";
		} else {
			CTBuilder(HT[leaves * 2 - 2], CodesTable, curCode, 0);
		}
		//debug
		//for (int i = 0; i < 256; i++) {
		//	if (CharacterTable[i] != 0) {
		//		printf("\n%d = %s\n", i, CodesTable[i]);
		//	}
		//}

		fwrite(&inputLength, 1, sizeof(int), fo);
		int counter = 0;
		HTCoder(&(HT[leaves * 2 - 2]), Output, &counter);

		fseek(fi, 0, SEEK_SET);
		int t = fread(mode, sizeof(char), 3, fi);
		if (t == 0) return 0;
		OutputCreator(Output, counter, fi, fo, CodesTable);

	} else {

		int inputLength;
		int t = fread(&inputLength, sizeof(int), 1, fi);
		if ((t == 0) || (inputLength == 0)) return 0;
		unsigned char Input[3] = { 0 };
		t = fread(&Input, sizeof(char), 3, fi);
		if (t > 1) t = 1;
		int curKnot = 0;
		int curPos = 0;
		HTDecoder(HT, HT, &curKnot, fi, &curPos, Input, &t);
		Decoder(HT, fi, fo, Input, inputLength, curPos, t);

	}

	fclose(fi);
	fclose(fo);

	return 0;
}
