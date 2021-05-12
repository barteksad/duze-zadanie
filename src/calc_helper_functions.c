#include "calc_helper_functions.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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


enum taskType
{
    ZERO,
    IS_COEFF,
    IS_ZERO,
    CLONE,
    ADD,
    MUL,
    NEG,
    SUB,
    IS_EQ,
    DEG,
    DEG_BY,
    AT,
    PRINT,
    POP,
    __INVALID__
};

bool isWhitespace(char c) // not new line
{
    switch(c)
    {
        case ' ':
            return true;
        case '\t':
            return true;
        case '\v':
            return true;
        case '\f':
            return true;
        case '\r':
            return true;
        default:
            return false;
    }
}

bool isAlpha(char c)
{
    if(c >= 'A' && c <= 'Z')
        return true;
    if(c >= 'a' && c <= 'z')
        return true;
    return false;
}

bool readNumberPolyCoeff(poly_coeff_t *x, poly_coeff_t sign)
{
    char c;
    bool at_least_one_numer = false;
    while(true)
    {
        c = fgetc(stdin);
        if(c == ')' || c == ',' || c == '\n' || c == EOF)
            break;

        if(c < '0' || c > '9')
        {
            BAD_INPUT(c);
            return false;
        }
        at_least_one_numer = true;
        poly_coeff_t current_number = (poly_coeff_t)(c - '0');
        if((sign > 0 && (LONG_MAX - current_number)/10 < *x) || (sign < 0 && (LONG_MIN + current_number)/10 > -*x)) // overflow
        {
            BAD_INPUT(c);
            return false;
        }
        *x *= 10;
        *x += current_number;
    }
    if(!at_least_one_numer)
    {
        BAD_INPUT(c);
        return false;
    }
    ungetc(c, stdin);
    return true;
}

bool readNumberUnsignedLong(size_t *x)
{
    char c;
    bool at_least_one_numer = false;
    while(true)
    {
        c = fgetc(stdin);
        if(c == ')' || c == ',' || c == '\n' || c == EOF)
            break;

        if(c < '0' || c > '9')
        {
            BAD_INPUT(c);
            return false;
        }
        at_least_one_numer = true;
        size_t current_number = (size_t)(c - '0');
        if((ULONG_MAX - current_number)/10 < *x) // overflow
        {
            BAD_INPUT(c);
            return false;
        }
        *x *= 10;
        *x += current_number;
    }
    if(!at_least_one_numer)
    {
        BAD_INPUT(c);
        return false;
    }
    ungetc(c, stdin);
    return true; 
}

bool readNumberPolyExp(poly_exp_t *x)
{
    char c;
    bool at_least_one_numer = false;
    while(true)
    {
        c = fgetc(stdin);
        if(c == ')' || c == ',' || c == '\n' || c == EOF)
            break;

        if(c < '0' || c > '9')
        {
            BAD_INPUT(c);
            return false;
        }
        at_least_one_numer = true;
        int current_number = (size_t)(c - '0');
        if((INT_MAX - current_number)/10 < *x) // overflow
        {
            BAD_INPUT(c);
            return false;
        }
        *x *= 10;
        *x += current_number;
    }
    if(!at_least_one_numer)
    {
        BAD_INPUT(c);
        return false;
    }
    ungetc(c, stdin);
    return true; 
}


Poly* readPoly()
{
    char c = fgetc(stdin);
    if(c != '(')
    {
        BAD_INPUT(c);
        return NULL;
    }

    Poly *p = NULL;
    c = fgetc(stdin);
    if(c != '-' && c != '\n' && c != EOF)
        ungetc(c, stdin);
    if(c == '(')
    {
        p = readPoly();
        if(p == NULL)
        return NULL;
    }
    else if(c == '-' || (c >= '0' && c <= '9'))
    {
        poly_coeff_t sign = 1;
        poly_coeff_t coeff = 0;
        if(c == '-')
            sign = -1;
        bool success = readNumberPolyCoeff(&coeff, sign);
        if(!success)
        {
            if(p)
            {
                PolyDestroy(p);
                free(p);
            }
            return NULL;
        }
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
        PolyDestroy(p);
        free(p);
        return NULL;
    }
    else
    {
        poly_exp_t exp = 0;
        bool success = readNumberPolyExp(&exp);
        if(exp > 2147483647) 
            success = false;
        if(!success)
        {
            PolyDestroy(p);
            free(p);
            return NULL;
        }

        if((exp == 0 && PolyIsCoeff(p)) || (PolyIsCoeff(p) && p->coeff == 0))
            q = p;
        else
        {
            q = malloc(sizeof(Poly));
            CHECK_PTR(q);
            Mono m[] = {MonoFromPoly(p, exp)};
            *q = PolyAddMonos(1, m);
            free(p);
        }
    }

    c = getc(stdin);
    if(c != ')')
    {
        PolyDestroy(q);
        free(q);
        BAD_INPUT(c);
        return NULL;
    }

    c = getc(stdin);
    if(c == '\n' || c == EOF || c == ',')
    {
        ungetc(c, stdin);
        return q;
    }
    else if(c == '+')
    {
        p = readPoly();
        if(p == NULL)
        {
            PolyDestroy(q);
            free(q);
            return NULL;
        }
        Poly *w = malloc(sizeof(Poly));
        *w = PolyAdd(p, q);
        PolyDestroy(p);
        PolyDestroy(q);
        free(p);
        free(q);
        return w;
    }
    else
    {
        BAD_INPUT(c);
        PolyDestroy(q);
        free(q);
        return NULL;
    }
}

Poly* readPolyCoeff(char c)
{
    poly_coeff_t sign = 1;
    if(c == '-')
        sign = -1;
    else
        ungetc(c, stdin);

    poly_coeff_t value = 0;
    bool success = readNumberPolyCoeff(&value, sign);
    if(success)
    {
     c = fgetc(stdin);
    if(c != '\n' && c != EOF)
        success = false;
    ungetc(c, stdin);
    }
    if(!success)
    {
        BAD_INPUT(c);
        return NULL;
    }

    value *= sign;
    Poly *p = malloc(sizeof(Poly));
    CHECK_PTR(p);
    *p = PolyFromCoeff(value);
    return p;
}

enum taskType decodeTask(char* input_string)
{
    if(!strcmp(input_string, "ZERO"))
        return ZERO;
    if(!strcmp(input_string, "IS_COEFF"))
        return IS_COEFF;
    if(!strcmp(input_string, "IS_ZERO"))
        return IS_ZERO;
    if(!strcmp(input_string, "ADD"))
        return ADD;
    if(!strcmp(input_string, "MUL"))
        return MUL;
    if(!strcmp(input_string, "NEG"))
        return NEG;
    if(!strcmp(input_string, "SUB"))
        return SUB;
    if(!strcmp(input_string, "IS_EQ"))
        return IS_EQ;
    if(!strcmp(input_string, "DEG"))
        return DEG;
    if(!strcmp(input_string, "DEG_BY"))
        return DEG_BY;
    if(!strcmp(input_string, "AT"))
        return AT;
    if(!strcmp(input_string, "PRINT"))
        return PRINT;
    if(!strcmp(input_string, "POP"))
        return POP;
    if(!strcmp(input_string, "CLONE"))
        return CLONE;
    return __INVALID__;
    
}

void readTask(Stack *stack, char c, size_t row_number)
{
    char* input_string = malloc(9 * sizeof(char));
    CHECK_PTR(input_string);
    size_t i = 0;

    while(c != '\n' && c != EOF && c != ' ')
    {
        if((c != '_' && !(c >='A' && c <= 'Z')) || i > 7)
        {
            if(isWhitespace(c)) // jestśi AT'\t'jest jako zły numer
            {
                input_string[i] = '\0';
                enum taskType task = decodeTask(input_string);
                switch(task)
                {
                    case DEG_BY:
                        fprintf(stderr, "ERROR %ld DEG BY WRONG VARIABLE\n", row_number);
                        break;
                    case AT:
                        fprintf(stderr, "ERROR %ld AT WRONG VALUE\n", row_number);
                        break;
                    default:
                        fprintf(stderr, "ERROR %ld WRONG COMMAND\n", row_number);
                        break;
                }
            }
            else
                fprintf(stderr, "ERROR %ld WRONG COMMAND\n", row_number);

            BAD_INPUT(c);
            free(input_string);
            return;
        }
        input_string[i++] = c;
        c = fgetc(stdin);
    }
    ungetc(c, stdin);
    input_string[i] = '\0';

    enum taskType task = decodeTask(input_string);
    free(input_string);
    if(task == __INVALID__)
    {
        c = fgetc(stdin);
        BAD_INPUT(c);
        fprintf(stderr, "ERROR %ld WRONG COMMAND\n", row_number);
        return;
    }
    
    size_t var_idx;
    poly_coeff_t x;
    if(task == DEG_BY)
    {
        bool success = false;
        var_idx = 0;
        c = fgetc(stdin);
        if(c != ' ')
            success = false;
        else
        {
          success = readNumberUnsignedLong(&var_idx);
          c = fgetc(stdin);
        }
        if(!success || (c != '\n' && c != EOF))
        {
            BAD_INPUT(c);
            fprintf(stderr, "ERROR %ld DEG BY WRONG VARIABLE\n", row_number);
            return;
        }
        ungetc(c, stdin);
    }
    else if(task == AT)
    {
        poly_coeff_t sign = 1;
        bool success = false;
        x = 0;
        c = fgetc(stdin);
        if(c != ' ')
            success = false;
        else
        {
            c = fgetc(stdin);
            if(c == '-')
                sign = -1;
            else
                ungetc(c, stdin);
            success = readNumberPolyCoeff(&x, sign);
            c = fgetc(stdin);
        }
        if(!success || (c != '\n' && c != EOF))
        {
            BAD_INPUT(c);
            fprintf(stderr, "ERROR %ld AT WRONG VALUE\n", row_number);
            return;
        }
        ungetc(c, stdin);
        x *= sign;
    }
    else
    {
        c = fgetc(stdin);
        if(c != '\n' && c != EOF)
        {
            BAD_INPUT(c);
            fprintf(stderr, "ERROR %ld WRONG COMMAND\n", row_number);
            return;
        }
        ungetc(c, stdin);
    }

    switch (task)
    {
        case ZERO:
            taskZero(stack);
            break;
        case IS_COEFF:
            taskIsCoeff(stack, row_number);
            break;
        case IS_ZERO:
            taskIsZero(stack, row_number);
            break;
        case CLONE:
            taskClone(stack, row_number);
            break;
        case ADD:
            taskAdd(stack, row_number);
            break;
        case MUL:
            taskMul(stack, row_number);
            break;
        case NEG:
            taskNeg(stack, row_number);
            break;
        case SUB:
            taskSub(stack, row_number);
            break;
        case IS_EQ:
            taskIsEq(stack, row_number);
            break;
        case DEG:
            taskDeg(stack, row_number);
            break;
        case DEG_BY:
            taskDegBy(stack, var_idx, row_number);
            break;
        case AT:
            taskAt(stack, x,  row_number);
            break;
        case PRINT:
            taskPrint(stack, row_number);
            break;
        case POP:
            taskPop(stack, row_number);
            break;
        default:
            break;
    }

}

void taskZero(Stack *stack)
{
    Poly *p = malloc(sizeof(Poly));
    CHECK_PTR(p);
    *p = PolyZero();
    StackAdd(stack, p);
}

void taskIsCoeff(Stack *stack, size_t row_number)
{
    if(stack->size == 0)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        bool is_coef = PolyIsCoeff(StackHead(stack));
        printf("%d\n", is_coef);
    }
}

void taskIsZero(Stack *stack, size_t row_number)
{
    if(stack->size == 0)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        bool is_zero = PolyIsZero(StackHead(stack));
        printf("%d\n", is_zero);
    }
}

void taskClone(Stack *stack, size_t row_number)
{
    if(stack->size == 0)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        Poly *p = StackHead(stack);
        Poly *q = malloc(sizeof(Poly));
        CHECK_PTR(q);
        *q = PolyClone(p);
        StackAdd(stack, q);
    }
}

void taskAdd(Stack *stack, size_t row_number)
{
    if(stack->size < 2)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        Poly *p = StackPop(stack);
        Poly *q = StackPop(stack);
        Poly *r = malloc(sizeof(Poly));
        CHECK_PTR(r);
        *r = PolyAdd(p, q);
        PolyDestroy(p);
        PolyDestroy(q);
        free(p);
        free(q);
        StackAdd(stack, r);
    }
}

void taskMul(Stack *stack, size_t row_number)
{
    if(stack->size < 2)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        Poly *p = StackPop(stack);
        Poly *q = StackPop(stack);
        Poly *r = malloc(sizeof(Poly));
        CHECK_PTR(r);
        *r = PolyMul(p, q);
        PolyDestroy(p);
        PolyDestroy(q);
        free(p);
        free(q);
        StackAdd(stack, r);
        // free(r);
    }
}

void taskNeg(Stack *stack, size_t row_number)
{
    if(stack->size == 0)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        Poly *p = StackPop(stack);
        Poly *q = malloc(sizeof(Poly));
        CHECK_PTR(q);
        *q = PolyNeg(p);
        PolyDestroy(p);
        free(p);
        StackAdd(stack, q);
    }
}

void taskSub(Stack *stack, size_t row_number)
{
    if(stack->size < 2)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        Poly *p = StackPop(stack);
        Poly *q = StackPop(stack);
        Poly *r = malloc(sizeof(Poly));
        CHECK_PTR(r);
        *r = PolySub(p, q);
        PolyDestroy(p);
        PolyDestroy(q);
        free(p);
        free(q);
        StackAdd(stack, r);
        // free(r);
    }
}

void taskIsEq(Stack *stack, size_t row_number)
{
    if(stack->size < 2)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        Poly *p = StackHead(stack);
        Poly *q = StackHead2(stack);
        bool is_eq = PolyIsEq(p, q);
        printf("%d\n", is_eq);
    }
}

void taskDeg(Stack *stack, size_t row_number)
{
    if(stack->size == 0)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        poly_exp_t exp = PolyDeg(StackHead(stack));
        printf("%d\n", exp);
    }
}

void taskDegBy(Stack *stack, size_t var_idx, size_t row_number)
{
    if(stack->size == 0)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        poly_exp_t exp = PolyDegBy(StackHead(stack), var_idx);
        printf("%d\n", exp);
    }
}

void taskAt(Stack *stack, unsigned long long x, size_t row_number)
{
    if(stack->size == 0)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        Poly *p = StackPop(stack);
        Poly *q = malloc(sizeof(Poly));
        CHECK_PTR(q);
        *q = PolyAt(p, x);
        PolyDestroy(p);
        free(p);
        StackAdd(stack, q);
    }
}

void printMono(Mono *m)
{
    printf("(");
    printPoly(&m->p);
    printf(",%d)", m->exp);
}

void printPoly(Poly *p)
{
    if(PolyIsCoeff(p))
    {
        printf("%ld", p->coeff);
        return;
    }
    
    for(size_t i = 0; i < p->size; i++)
    {
        if(i>0)
            printf("+");
        printMono(&p->arr[i]);
    }
}

void taskPrint(Stack *stack, size_t row_number)
{
    if(stack->size == 0)
    {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
        return;
    }

    printPoly(StackHead(stack));
    printf("\n");
}

void taskPop(Stack *stack, size_t row_number)
{
    if(stack->size == 0)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        Poly *p = StackPop(stack);
        PolyDestroy(p);
        free(p);
    }
}

