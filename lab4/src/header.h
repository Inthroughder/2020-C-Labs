#pragma once
#ifndef HEADER__H
#define HEADER__H

int optoint(char op);

int InputCheck(char *S, int n);

int ItoP(char* Input, int isize, long long* RPN);

int Calculator(long long* RPN, int rpnlen);

#endif
