#include <stdio.h>
#include <string.h>
#define BUFFER 3000

int main(void) {
	char In[BUFFER + 1];
	long long RPN[BUFFER];

	char* k = fgets(In, BUFFER, stdin);
	int n = strlen(In) - 1;
	if ((n == 0) || (k == 0)) {
		printf("syntax error");
		return 0;
	}

	if (InputCheck(In, n) == 0) {
		printf("syntax error");
		return 0;
	}

	In[n + 1] = '#';

	int rpnlen = ItoP(In, n, RPN);
	if (rpnlen == -1) {
		printf("syntax error");
		return 0;
	}

	Calculator(RPN, rpnlen);

	return 0;
}
