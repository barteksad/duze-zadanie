/** @file
  Interfejs struktury danych stos

  @author Bartłomiej Sadlej
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#pragma once
#include "poly.h"

struct StackNode;

typedef struct Stack
{
    struct StackNode *head;
    size_t size;

 }Stack;

typedef struct StackNode
{
    Poly *p;
    struct StackNode *next;
} StackNode;

Stack* createNewStack();

Poly* StackPop(Stack *stack);

void StackAdd(Stack *stack, Poly *p);

Poly* StackHead(Stack *stack);

Poly* StackHead2(Stack *stack);

void StackNodeFree(StackNode *node);

void StackFree(Stack *stack);


