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

Stack* createNewStack()
{
  Stack *s = malloc(sizeof(Stack));
  CHECK_PTR(s);
  s->head = NULL;
  s->size = 0;
  return s;
}

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

Poly* StackHead(Stack *stack)
{
  return stack->head->p;
}

Poly* StackHead2(Stack *stack)
{
  return stack->head->next->p;
}

void StackNodeFree(StackNode *node)
{
    if(node)
    {
      StackNodeFree(node->next);
      PolyDestroy(node->p);
      free(node->p);
      free(node);
    }
}

void StackFree(Stack *stack)
{
    StackNodeFree(stack->head);
    free(stack);
}
