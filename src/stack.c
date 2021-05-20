/** @file
  Implementacja interfejsu struktury danych stos

  @author Bartłomiej Sadlej
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include "stack.h"
#include <stdlib.h>

#define CHECK_PTR(p)  \
  do {                \
    if (p == NULL) {  \
      exit(1);        \
    }                 \
  } while (0)


#define INITIAL_STACK_SIZE 512
#define STACK_RESIZE_MULTIPLIER 2

/**
 * To jest struktura przechowująca stos.
 * stos jest przechowywany na tablicy o początkowym rozmiarze INITIAL_STACK_SIZE
 * w przypadku braku miejsca w tablicy jest ona rozszerzana STACK_RESIZE_MULTIPLIER razy
 */
struct Stack{
    Poly* arr;
    size_t size;
    size_t arr_size;
 };


Stack* createNewStack()
{
  Stack *s = malloc(sizeof(Stack));
  CHECK_PTR(s);
  s->arr = malloc(sizeof(Poly) * INITIAL_STACK_SIZE);
  CHECK_PTR(s->arr);
  s->size = 0;
  s->arr_size = INITIAL_STACK_SIZE;
  return s;
}

size_t StackSize(Stack *stack)
{
  return stack->size;
}

Poly StackPop(Stack *stack)
{
    assert(stack->size > 0);
    Poly p = stack->arr[stack->size - 1];
    stack->size--;
    return p;
}

void StackAdd(Stack *stack, Poly p)
{
  if(stack->size >= stack->arr_size)
  {
    size_t new_size = STACK_RESIZE_MULTIPLIER * stack->arr_size;
    stack->arr = realloc(stack->arr, sizeof(Poly) * new_size);
    stack->arr_size = new_size;
  }
  stack->arr[stack->size] = p;
  stack->size++;
}

Poly* StackHead(Stack *stack)
{
  assert(stack->size > 0);
  return &stack->arr[stack->size - 1];
}

Poly* StackHead2(Stack *stack)
{
  assert(stack->size > 1);
  return &stack->arr[stack->size - 2];
}

void StackFree(Stack *stack)
{
    for(size_t i = 0; i < stack->size; i++)
      PolyDestroy(&stack->arr[i]);
    free(stack->arr);
    free(stack);
}
