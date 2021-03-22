#include <stdio.h>
#define BUFFER 3000

int optoint(char op) {
	if (op == '+') {
		return -3;
	} else if (op == '-') {
		return -4;
	} else if (op == '*') {
		return -1;
	} else {
		return -2;
	}
}

int InputCheck(char *S, int n) {
	int CBScur = 0;
	for (int i = 0; i < n; i++) {
		if (S[i] == '(') {
			CBScur++;
		} else if (S[i] == ')') {
			CBScur--;
			if ((i > 0) && (S[i - 1] == '(')) return 0;
		} else if (((S[i] >= 48) && (S[i] <= 57)) || (S[i] == 42) || (S[i] == 43) || (S[i] == 47) || (S[i] == 45)) {
			continue;
		} else {
			return 0;
		}

		if (CBScur < 0) return 0;
	}

	if (CBScur > 0) return 0;
	return 1;
}

int ItoP(char* Input, int isize, long long* RPN) {
	char OpsStack[BUFFER];
	int ostop = -1;
	int curnum = 1;
	int curout = 0;

	for (int i = 0; i < isize; i++) {

		if (Input[i] == '(') {

			ostop++;
			OpsStack[ostop] = '(';

		} else if (Input[i] == ')') {

			while (OpsStack[ostop] != '(') {
				RPN[curout] = optoint(OpsStack[ostop]);
				ostop--;
				curout++;
			}
			ostop--;

		} else if ((Input[i] == '+') || (Input[i] == '-')) {

			if (ostop != -1) {
				if (OpsStack[ostop] == '(') {
					ostop++;
					OpsStack[ostop] = Input[i];
				} else {
					RPN[curout] = optoint(OpsStack[ostop]);
					OpsStack[ostop] = Input[i];
					curout++;
				}
			} else {
				ostop++;
				OpsStack[ostop] = Input[i];
			}
			
		} else if ((Input[i] == '*') || (Input[i] == '/')) {

			if (ostop != -1) {
				if ((OpsStack[ostop] == '*') || (OpsStack[ostop] == '/')) {
					RPN[curout] = optoint(OpsStack[ostop]);
					OpsStack[ostop] = Input[i];
					curout++;
				} else {
					ostop++;
					OpsStack[ostop] = Input[i];
				}
			}
			else {
				ostop++;
				OpsStack[ostop] = Input[i];
			}

		} else {

			if (curnum == 1) {
				if ((Input[i] == 48) && ((Input[i + 1] >= 48) && (Input[i + 1] <= 57)) == 1) return -1;
				RPN[curout] = Input[i] - 48;
			} else {
				RPN[curout] = RPN[curout] * 10 + (Input[i] - 48);
			}

			if (((Input[i + 1] >= 48) && (Input[i + 1] <= 57)) == 0) {
				curout++;
				curnum = 1;
			} else {
				curnum = 2;
			}

		}
	}

	for (int i = ostop; i >= 0; i--) {
		RPN[curout] = optoint(OpsStack[i]);
		curout++;
	}

	return curout;
	
}

int Calculator(long long* RPN, int rpnlen) {
	int Stack[BUFFER];
	int top = -1;
	for (int i = 0; i < rpnlen; i++) {
		if (RPN[i] < 0) {
			if (top < 1) {
				printf("syntax error");
				return 0;
			}
			if (RPN[i] == -1) {
				Stack[top - 1] = Stack[top - 1] * Stack[top];
				top--;
			} else if (RPN[i] == -2) {
				if (Stack[top] == 0) {
					printf("division by zero");
					return 0;
				}
				Stack[top - 1] = Stack[top - 1] / Stack[top];
				top--;
			} else if (RPN[i] == -3) {
				Stack[top - 1] = Stack[top - 1] + Stack[top];
				top--;
			} else {
				Stack[top - 1] = Stack[top - 1] - Stack[top];
				top--;
			}
		} else {
			top++;
			Stack[top] = RPN[i];
		}
	}

	if (top > 0) printf("syntax error");
	printf("%d", Stack[0]);
	return 0;
}
