/** @file
  Kalkulator wielomianów rzadkich wielu zmiennych

  @author Bartłomiej Sadlej
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include "stack.h"
#include "macros.h"
#include "calc_helper_functions.h"

#include <stdlib.h>
#include <stdio.h>

int main()
{
    Stack* stack = CreateNewStack();

    size_t row_number = 1;
    int c;

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
              Poly* p = ReadPoly(stack);
              if(p == NULL)
                PrintError(WRONG_POLY, row_number);
              else
              {
                StackAdd(stack, *p);
                free(p);
              }
              break;
          default:
              if(c >= 'A' && c <= 'Z') // task
                ReadTask(stack, c, row_number);
              else if ((c >= '0' && c <= '9') || c == '-') // poly coeff
              {
                ungetc(c, stdin);
                if(!ReadPolyCoeff(stack))
                  PrintError(WRONG_POLY, row_number);
              }
              else // błąd
              {
                if(IsAlpha(c))
                  PrintError(INVALID_COMMAND, row_number);
                else
                  PrintError(WRONG_POLY, row_number);
                BAD_INPUT(c);
              }
              break;
        }
    }
    StackFree(stack);
}