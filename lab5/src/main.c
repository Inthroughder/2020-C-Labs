#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS
#define BUFFER 32 + 256 * 256 + 511 + 514 // first is length; seconds and third is tree (codes and knots); last is whatever

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
			fwrite(Output + (counter / 8), sizeof(char), 1, fo);
			//char t = Output[counter / 8];
			//t = ((t >> (8 - counter % 8)) << (8 - counter % 8));
			//fwrite(&t, sizeof(char), 1, fo);
		}
	}
	else {
		//mid
		fwrite(Output, sizeof(char), counter / 8, fo);
		Output[0] = Output[counter / 8];
		memset(Output + 1, 0, BUFFER - 1);
		OutputCreator(Output, counter % 8, fi, fo, CodesTable);
	}
	return 0;
}

int OutputCreator(unsigned char* Output, int counter, FILE* fi, FILE* fo, char** CodesTable) {
	while (counter < BUFFER - 8) {
		//printf("\ncounter in OC = %d", counter);
		int c = fgetc(fi);
		//printf("\ngot %d just now", c);
		if (c == EOF) {
			OutputWriter(1, Output, counter, fi, fo, CodesTable); //last output
			return 0;
		}

		for (unsigned int i = 0; i < strlen(CodesTable[c]); i++) {
			//printf("\n\nin OC i = %d, c = %d", i, c);
			if (CodesTable[c][i] == '1') {
				Output[counter / 8] = Output[counter / 8] | (1 << (7 - counter % 8)); // that's 1
			}
			counter++;
		}
	}
	OutputWriter(0, Output, counter, fi, fo, CodesTable); //mid-writing output
	return 0;
}

int HTDecoder(struct knot *curKnot, FILE* fi, int *curPos, unsigned char* Input, char* t, int *genPos) {
	//printf("\n\ncalled HTDecoder; curPos = %d, genPos = %d", *curPos, *genPos);

	while ((*curPos > 7) && (*t == 1)) {
		Input[0] = Input[1];
		Input[1] = Input[2];
		*t = fread(Input + 2, sizeof(char), 1, fi);
		*curPos = *curPos - 8;
	}
	unsigned char cur = 1 & (Input[*curPos / 8] >> (7 - *curPos % 8));
	(*curPos)++;
	(*genPos)++;
	if (cur == 1) {
		//printf("\n\ntook 1 at curPos = %d, genPos = %d", *curPos - 1, *genPos - 1);
		(*curKnot).letter = -1;
		(*curKnot).pointer_r = malloc(sizeof(struct knot));
		(*curKnot).pointer_l = malloc(sizeof(struct knot));
		HTDecoder((*curKnot).pointer_r, fi, curPos, Input, t, genPos);
		HTDecoder((*curKnot).pointer_l, fi, curPos, Input, t, genPos);
	} else {
		//printf("\n\ntook 0 at curPos = %d, genPos = %d", *curPos - 1, *genPos - 1);
		(*curKnot).pointer_r = NULL;
		(*curKnot).pointer_l = NULL;
		unsigned char c = (Input[*curPos / 8] << (*curPos % 8)) | (Input[*curPos / 8 + 1] >> (8 - *curPos % 8));
		*curPos = *curPos + 8;
		*genPos = *genPos + 8;
		(*curKnot).letter = c;
		//printf("\n\ntook a letter %X; now curPos = %d, genPos = %d", c, *curPos, *genPos);
	}
	return 0;
}

int Decoder(struct knot *startKnot, FILE* fi, FILE* fo, unsigned char* Input, int inputLength, int curPos, int t) {

	for (int i = 0; i < inputLength; i++) {
		//printf("\ndecoding %d letter", i);
		while ((curPos > 7) && (t == 1)) {
			Input[0] = Input[1];
			Input[1] = Input[2];
			t = fread(Input + 2, sizeof(char), 1, fi);
			curPos = curPos - 8;
		}
		struct knot curKnot = *startKnot;
		while (curKnot.letter == -1) {
			int c = 1 & (Input[curPos / 8] >> (7 - curPos % 8));
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
	char mode[3];

	fi = fopen("in.txt", "rb");
	fo = fopen("out.txt", "wb");
	int t = fread(mode, sizeof(char), 3, fi);
	if (t == 0) { 
		fclose(fi);
		fclose(fo);
		return 0;
	}

	//printf("\n0");
	//printf("\nt = %d", t);

	if (mode[0] == 'c') {

		struct knot HT[512 - 1];
		unsigned char Output[BUFFER] = { 0 };
		int CharacterTable[256] = { 0 };

		int inputLength = 0;
		int leaves = LetterCounter(CharacterTable, fi, &inputLength);
		//printf("\nlength = %d, leaves = %d", inputLength, leaves);
		if (inputLength == 0) {
			fclose(fi);
			fclose(fo);
			return 0;
		}

		//BRUUUUUUUUH

		/*if (inputLength == 2583) {
			FILE* f = fopen("test27.txt", "wb");
			fseek(fi, 0, SEEK_SET);
			int t = fgetc(fi);
			while (t != EOF) {
				fwrite(&t, sizeof(char), 1, f);
				t = fgetc(fi);
			}
		}
		t = fread(mode, sizeof(char), 3, fi);*/

		HTBuilder(CharacterTable, HT);

		//FILE* fht = fopen("HTCoding.txt", "wb");
		/*for (int i = 0; i < leaves * 2 - 1; i++) {
			printf("\n\nknot number %d, letter = %X, count = %d", i, HT[i].letter, HT[i].count);
			if (HT[i].pointer_l != NULL) {
				printf(", left pointer has count = %d and letter = %X, right pointer has count = %d and letter = %X", (*(HT[i].pointer_l)).count, (*(HT[i].pointer_l)).letter, (*(HT[i].pointer_r)).count, (*(HT[i].pointer_r)).letter);

			}
		}*/


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
		if (t == 0) {
			fclose(fi);
			fclose(fo);
			return 0;
		}
		//printf("\n\nbefore the OC counter = %d", counter);
		OutputCreator(Output, counter, fi, fo, CodesTable);
		//printf("%d %d", inputLength, leaves);

	} else if (mode[0] == 'd') {
		//printf("\n1");
		int inputLength;
		char t = fread(&inputLength, sizeof(int), 1, fi);
		if ((t == 0) || (inputLength == 0)) {
			fclose(fi);
			fclose(fo);
			return 0;
		}
		//printf("\ninputLength = %d", inputLength);
		//printf("\n2");
		unsigned char Input[3] = { 0 };
		t = fread(&Input, sizeof(char), 3, fi);
		if (t >= 1) t = 1;
		int curPos = 0;
		
		int curKnotMem = 0;

		int genPos = 0;
		struct knot* startKnot = malloc(sizeof(struct knot));
		HTDecoder(startKnot, fi, &curPos, Input, &t, &genPos);
		Decoder(startKnot, fi, fo, Input, inputLength, curPos, t);

		/*for (int i = 0; i < 30; i++) {
			if (HT[i].pointer_l != NULL) {
				printf("\n\nknot number %d, letter = %X, left pointer = %X, right pointer = %X", i, HT[i].letter, (*(HT[i].pointer_l)).letter, (*(HT[i].pointer_r)).letter);
			} else {
				printf("\n\nknot number %d, letter = %X", i, HT[i].letter);
			}
			
		}*/

		//printf("%d", inputLength);
	}

	fclose(fi);
	fclose(fo);

	return 0; //1126, 17 different
}
