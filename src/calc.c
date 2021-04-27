#include "stack.h"
#include "calc_helper_functions.h"
#include <stdlib.h>
#include <stdio.h>

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
            row_number++;
            break;
        case '\n':
            row_number++;
            break;
        case '(':
            ungetc(c, stdin);
            Poly *p = read_poly();
            if(p == NULL)
            {

              printf("ERROR!\n");
            }
            else
              StackAdd(stack, p);
            break;
        default:
            break;
            // read_task(stack, c);
        }
    }

    StackFree(stack);
}