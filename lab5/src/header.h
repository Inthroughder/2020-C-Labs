#pragma once
#ifndef HEADER__H
#define HEADER__H

struct knot;

int LetterCounter(int* CharacterTable, FILE* f, int* inputLength);

int HTSorter(struct knot* Tree, int leaves);

int HTBuilder(int* CharacterTable, struct knot* Tree);

int CTBuilder(struct knot curKnot, char** CodesTable, char* curCode, int curCodeLen);

int HTCoder(struct knot *curKnot, unsigned char* Output, int *counter);

int OutputCreator(unsigned char* Output, int counter, FILE* fi, FILE* fo, char** CodesTable);

int OutputWriter(int mode, unsigned char* Output, int counter, FILE* fi, FILE* fo, char** CodesTable);

int HTDecoder(struct knot *curKnot, FILE* fi, int *curPos, unsigned char* Input, char* t);

int Decoder(struct knot *startKnot, FILE* fi, FILE* fo, unsigned char* Input, int inputLength, int curPos, int t);

int MFree(struct knot* point);

#endif
