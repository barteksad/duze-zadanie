/** @file
  Interfejs struktury danych stos

  @author Bartłomiej Sadlej
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#pragma once
#include "poly.h"

typedef struct Stack Stack;

Stack* createNewStack();

Poly StackPop(Stack *stack);

void StackAdd(Stack *stack, Poly p);

size_t StackSize(Stack *stack);

Poly* StackHead(Stack *stack);

Poly* StackHead2(Stack *stack);

void StackFree(Stack *stack);


