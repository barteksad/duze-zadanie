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

Poly* StackPop(Stack *stack)
{
    assert(stack->size > 0);
    Poly *p = stack->head->p;
    StackNode *temp = stack->head->next;
    free(stack->head);
    stack->head = temp;
    stack->size--;
    return p;
}

void StackAdd(Stack *stack, Poly *p)
{
    StackNode *new_head = malloc(sizeof(StackNode));
    CHECK_PTR(new_head);
    new_head->p = p;
    if(!stack->head)
    {
        new_head->next = NULL;
        stack->head = new_head;
    }
    else
    {
        new_head->next = stack->head;
        stack->head = new_head;
    }
    stack->size++;
}

void StackNodeFree(StackNode *node)
{
    if(node)
    {
      StackNodeFree(node->next);
      PolyDestroy(node->p);
      free(node);
    }
}

void StackFree(Stack *stack)
{
    StackNodeFree(stack->head);
}