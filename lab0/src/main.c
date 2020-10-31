#include <stdio.h>
#include <string.h>


char min(char a, char b) {
	if (a >= b) return b; else return a;
}

long long int pwr(char a, char b) {
	long long int res = 1;
	for (int i = b; i > 0; i--) {
		res = res * a;
	}
	return res;
}

void translator(char b1, char b2, char* S, char Slen, char* result) {
	long long int res = 0;
	long long int q = pwr(b1, Slen - 1);
	for (int i = 0; i < Slen; i++) {
		if (('A' <= S[i]) & (S[i] <= 'F')) {
			res = res + q * (S[i] - 55);
		}
		else {
			res = res + q * (S[i] - 48);
		}
		q = q / b1;
	}

	int i = 0;
	while (res > 0) {
		result[i] = res % b2;
		res = res / b2;
		i++;
	}
	result[49] = i;
}



void ftranslator(char b1, char b2, char* S, char Slen, char* result) {
	double res = 0;
	double q = 1;
	for (int i = 0; i < Slen; i++) {
		q = q / b1;
		if (('A' <= S[i]) & (S[i] <= 'F')) {
			res = res + (S[i] - 55) * q;
		}
		else {
			res = res + (S[i] - 48) * q;
		}
	}
	int i;
	q = 1;
	for (i = 0; i < 12; i++) {
		q = q / b2;
		char t = 0;
		while (res >= (q * (t + 1))) {
			t++;
		}
		result[i] = t;

		res = res - q * t;
		result[13] = i + 1;
		if (res == 0) {
			break;
		}
	}
}

int main() {
	int b1, b2;
	char S[14];
	int fuck_this_compilator_m;
	fuck_this_compilator_m = scanf("%d %d\n%13s", &b1, &b2, S);
	fuck_this_compilator_m++;
	if ((1 < b1) && (b1 < 17) && (1 < b2) && (b2 < 17)) {
		int fuck_this_compilator = 0;
		for (unsigned int i = 0; i < strlen(S); i++) {
			int a = 0;
			if (S[i] == '.') {
				if (fuck_this_compilator == 1) {
					printf("bad input");
					return 0;
				}
				fuck_this_compilator = 1;
				continue;
			}
			if ((S[i] >= '0') && (S[i] <= '9')) {
				a = S[i] - '0';
			}
			else if ((S[i] >= 'A') && (S[i] <= 'F')) {
				a = S[i] - 'A';
			}
			else if ((S[i] >= 'a') && (S[i] <= 'f')) {
				S[i] = S[i] - 32;
				a = S[i] - 'A';
			}
			else {
				printf("bad input");
				return 0;
			}
			if (a >= b1) {
				printf("bad input");
				return 0;
			}
		}
	}
	else {
		printf("bad input");
		return 0;
	}

	char SI[2][12];
	char t = 0;
	unsigned int EntLen;
	for (EntLen = 0; EntLen < strlen(S); EntLen++) {
		if (S[EntLen] == '.') {
			t = 1;
			break;
		}
		else {
			SI[0][EntLen] = S[EntLen];
		}
	}
	if ((S[strlen(S) - 1] == '.') || (S[0] == '.')) {
		printf("bad input");
		return 0;
	}
	unsigned int MantLen;
	if (t == 1) {
		for (MantLen = 0; MantLen < (strlen(S) - EntLen - 1); MantLen++) {
			SI[1][MantLen] = S[EntLen + 1 + MantLen];
		}
	}

	char EntRes[50];
	char MantRes[14];
	translator(b1, b2, SI[0], EntLen, EntRes);
	if (t == 1) {
		ftranslator(b1, b2, SI[1], MantLen, MantRes);
	}

	if (EntRes[49] == 0) {
		printf("0");
	}
	for (int j = EntRes[49] - 1; j >= 0; j--) {
		if ((10 <= EntRes[j]) & (EntRes[j] <= 15)) {
			printf("%c", EntRes[j] + 55);
		}
		else {
			printf("%d", EntRes[j]);
		}
	}
	if (t == 1) {
		printf(".");
		for (int j = 0; j < min(12, MantRes[13]); j++) {
			if ((10 <= MantRes[j]) & (MantRes[j] <= 15)) {
				printf("%c", MantRes[j] + 55);
			}
			else {
				printf("%d", MantRes[j]);
			}
		}
	}
	return 0;
}
