/** @file
  Kalkulator wielomianów rzadkich wielu zmiennych

  @author Bartłomiej Sadlej
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include "stack.h"
#include "calc_helper_functions.h"
#include <stdlib.h>
#include <stdio.h>

#define BAD_INPUT(c) \
  do { \
    while(c != '\n' && c != EOF) { \
      c=getc(stdin); \
    } \
    ungetc(c, stdin); \
  } while (0)

#define CHECK_PTR(p)  \
  do {                \
    if (p == NULL) {  \
      exit(1);        \
    }                 \
  } while (0)

int main()
{
    Stack* stack = createNewStack();

    size_t row_number = 1;
    char c;

    while((c = fgetc(stdin)) != EOF)
    {
        switch (c)
        {
          case '#':
              while(c != '\n' && c != EOF)
                c = fgetc(stdin);
              row_number++;
              break;
          case '\n':
              row_number++;
              break;
          case '(' :
              ungetc(c, stdin);
              Poly* p = readPoly(stack);
              if(p == NULL)
                fprintf(stderr, "ERROR %ld WRONG POLY\n", row_number);
              else
              {
                StackAdd(stack, *p);
                free(p);
              }
              break;
          default:
              if(c >= 'A' && c <= 'Z')
                readTask(stack, c, row_number);
              else if ((c >= '0' && c <= '9') || c == '-')
              {
                if(!readPolyCoeff(stack, c))
                  fprintf(stderr, "ERROR %ld WRONG POLY\n", row_number);
              }
              else
              {
                if(isAlpha(c))
                  fprintf(stderr, "ERROR %ld WRONG COMMAND\n", row_number);
                else
                  fprintf(stderr, "ERROR %ld WRONG POLY\n", row_number);
                BAD_INPUT(c);
              }
              break;
        }
    }
    StackFree(stack);
}