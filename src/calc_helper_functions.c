#include "calc_helper_functions.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

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
bool read_number(long *x)
{
    char c;
    bool at_least_one_numer = false;
    while((c = fgetc(stdin)) != '\n' || c != EOF)
    {
        if(c == ')' || c == ',')
            break;

        if(c < '0' || c > '9')
        {
            BAD_INPUT(c);
            return false;
        }
        at_least_one_numer = true;
        *x *= 10;
        *x += (long)(c - '0');
    }
    if(!at_least_one_numer)
    {
        BAD_INPUT(c);
        return false;
    }
    ungetc(c, stdin);
    return true;
}

// (1,2)+((-1,0)+(-2,1)+(-1,2),1)+((1,2),0)
Poly* read_poly()
{
    char c = fgetc(stdin);
    if(c != '(')
    {
        BAD_INPUT(c);
        return NULL;
    }

    Poly *p = NULL;
    c = fgetc(stdin);
    if(c == '(')
    {
        ungetc(c, stdin);
        p = read_poly();
    }
    else if(c == '-' || (c >= '0' && c <= '9'))
    {
        long sign = 1;
        poly_coeff_t coeff = 0;
        if(c == '-')
            sign = -1;
        else
            ungetc(c, stdin);
        bool success = read_number(&coeff);
        if(!success)
            return NULL;
        coeff *= sign;
        p = malloc(sizeof(Poly));
        CHECK_PTR(p);
        *p = PolyFromCoeff(coeff);
    }
    else
    {
        BAD_INPUT(c);
        return NULL;
    }

    c = fgetc(stdin);
    Poly *q;
    if(c != ',')
    {
        BAD_INPUT(c);
        if(p!=NULL)
            PolyDestroy(p);
        return NULL;
    }
    else
    {
        poly_exp_t exp = 0;
        bool success = read_number(&exp);
        // TODO check boundry
        if(!success)
        {
            PolyDestroy(p);
            return NULL;
        }

        if(exp == 0 && PolyIsCoeff(p))
            return p;

        q = malloc(sizeof(Poly));
        CHECK_PTR(q);
        Mono m[] = {MonoFromPoly(p, exp)};
        *q = PolyAddMonos(1, m);
        // PolyDestroy(p);
        // q->size = 1;
        // q->arr = malloc(sizeof(Mono));
        // CHECK_PTR(q->arr);
        // q->arr[0] = MonoFromPoly(p, exp);
    }

    c = getc(stdin);
    if(c != ')')
    {
        PolyDestroy(p);
        BAD_INPUT(c);
        return NULL;
    }

    c = getc(stdin);
    if(c == '\n' || c == EOF)
        return q;
    else if(c == '+')
    {
        p = read_poly();
        Poly *w = malloc(sizeof(Poly));
        *w = PolyAdd(p, q);
        PolyDestroy(p);
        PolyDestroy(q);
        return w;
    }
    else
    {
        BAD_INPUT(c);
        PolyDestroy(q);
        return NULL;
    }
}

void read_task(Stack *stack, char c);