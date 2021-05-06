#pragma once
#include "stack.h"
#include <stdio.h>

enum taskType;

bool isAlpha(char c);

bool isWhitespace(char c);

Poly* readPoly();

Poly* readPolyCoeff(char c);

bool readNumberPolyCoeff(poly_coeff_t *x, poly_coeff_t sign);

bool readNumberUnsignedLong(size_t *x);

bool readNumberPolyExp(poly_exp_t *x);

void readTask(Stack *stack, char c, size_t row_number);

enum taskType decodeTask(char* input_string);

void taskZero(Stack *stack, size_t row_number);

void taskIsCoeff(Stack *stack, size_t row_number);

void taskIsZero(Stack *stack, size_t row_number);

void taskClone(Stack *stack, size_t row_number);

void taskAdd(Stack *stack, size_t row_number);

void taskMul(Stack *stack, size_t row_number);

void taskNeg(Stack *stack, size_t row_number);

void taskSub(Stack *stack, size_t row_number);

void taskIsEq(Stack *stack, size_t row_number);

void taskDeg(Stack *stack, size_t row_number);

void taskDegBy(Stack *stack, size_t var_idx, size_t row_number);

void taskAt(Stack *stack, unsigned long long int x, size_t row_number);

void taskPrint(Stack *stack, size_t row_number);

void taskPop(Stack *stack, size_t row_number);

