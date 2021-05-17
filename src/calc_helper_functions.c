/** @file
  Implementacja interfejsu kalkulatora wielomianów rzadkich wielu zmiennych

  @author Bartłomiej Sadlej
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include "calc_helper_functions.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// wczytuje znaki do końca złego wiersza
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

#define POLY_EXP_MAX 2147483647
#define MAX_TASK_LEN 8 // IS_COEFF

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

// '\n' nie jest tu celowo rozpatrywany
bool IsWhitespace(char c)
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

bool IsAlpha(char c)
{
    if(c >= 'A' && c <= 'Z')
        return true;
    if(c >= 'a' && c <= 'z')
        return true;
    return false;
}

bool ReadNumberPolyCoeff(poly_coeff_t *x, poly_coeff_t sign)
{
    /*
        wczytuje znak po znaku
        weryfikacja czy wystąpił overflow polega na sprawdzeniu czy 
            stara wartość > 0:
                MAX - nowa_wartość < stara wartość
            stara wartość < 0
                min + nowa_wartość > stara wartość
    */
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

bool ReadNumberUnsignedLong(size_t *x)
{
    /*
        wczytuje znak po znaku
        weryfikacja czy wystąpił overflow polega na sprawdzeniu czy 
            stara wartość > 0:
                MAX - nowa_wartość < stara wartość
            stara wartość < 0
                min + nowa_wartość > stara wartość
    */
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

bool ReadNumberPolyExp(poly_exp_t *x)
{
    /*
        wczytuje znak po znaku
        weryfikacja czy wystąpił overflow polega na sprawdzeniu czy 
            stara wartość > 0:
                MAX - nowa_wartość < stara wartość
            stara wartość < 0
                min + nowa_wartość > stara wartość
    */
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


Poly* ReadPoly()
{
    char c = fgetc(stdin);
    if(c != '(') // musi zaczynać się ()
    {
        BAD_INPUT(c);
        return NULL;
    }

    Poly *p = NULL;
    c = fgetc(stdin);
    if(c != '-' && c != '\n' && c != EOF)
        ungetc(c, stdin);
    if(c == '(') // jeśli coef to jednomian to wywołujemy się rekurencyjnie
    {
        p = ReadPoly();
        if(p == NULL)
        return NULL;
    }
    else if(c == '-' || (c >= '0' && c <= '9')) // wczytywanie coeff-a
    {
        poly_coeff_t sign = 1;
        poly_coeff_t coeff = 0;
        if(c == '-')
            sign = -1;
        bool success = ReadNumberPolyCoeff(&coeff, sign);
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
    else // nie coeff, nie mono to błąd
    {
        BAD_INPUT(c);
        return NULL;
    }

    c = fgetc(stdin);
    Poly *q;
    if(c != ',') // następnie musi być przecinek
    {
        BAD_INPUT(c);
        PolyDestroy(p);
        free(p);
        return NULL;
    }
    else // wczytywanie wykładnika i tworzenie wielomianu
    {
        poly_exp_t exp = 0;
        bool success = ReadNumberPolyExp(&exp);
        if(exp > POLY_EXP_MAX) 
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
    if(c != ')') // potem musi być )
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
    else if(c == '+') // jeśli + to rekurencyjnie wczytujemy następny i zwracamy sumę
    {
        p = ReadPoly();
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
    else // nie +, nie koniec lini to błąd
    {
        BAD_INPUT(c);
        PolyDestroy(q);
        free(q);
        return NULL;
    }
}

bool ReadPolyCoeff(Stack *stack, char c)
{
    poly_coeff_t sign = 1;
    if(c == '-')
        sign = -1;
    else
        ungetc(c, stdin);

    poly_coeff_t value = 0;
    bool success = ReadNumberPolyCoeff(&value, sign);
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
        return false;
    }

    value *= sign;
    Poly p = PolyFromCoeff(value);
    StackAdd(stack, p);
    return true;
}

enum taskType DecodeTask(char* input_string)
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

void ReadTask(Stack *stack, char c, size_t row_number)
{
    char* input_string = malloc( (MAX_TASK_LEN + 1) * sizeof(char));
    CHECK_PTR(input_string);
    size_t i = 0;

    while(c != '\n' && c != EOF && c != ' ')
    {
        if((c != '_' && !(c >='A' && c <= 'Z')) || i > 7)
        {
            if(IsWhitespace(c)) // AT(tab) jako zły numer
            {
                input_string[i] = '\0';
                enum taskType task = DecodeTask(input_string);
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

    enum taskType task = DecodeTask(input_string);
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
        if(!ReadNumerToDegBy(&var_idx, row_number))
            return;
    }
    else if(task == AT)
    {
        if(!ReadNumberToAt(&x, row_number))
            return;
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
            TaskZero(stack);
            break;
        case IS_COEFF:
            TaskIsCoeff(stack, row_number);
            break;
        case IS_ZERO:
            TaskIsZero(stack, row_number);
            break;
        case CLONE:
            TaskClone(stack, row_number);
            break;
        case ADD:
            TaskAdd(stack, row_number);
            break;
        case MUL:
            TaskMul(stack, row_number);
            break;
        case NEG:
            TaskNeg(stack, row_number);
            break;
        case SUB:
            TaskSub(stack, row_number);
            break;
        case IS_EQ:
            TaskIsEq(stack, row_number);
            break;
        case DEG:
            TaskDeg(stack, row_number);
            break;
        case DEG_BY:
            TaskDegBy(stack, var_idx, row_number);
            break;
        case AT:
            TaskAt(stack, x,  row_number);
            break;
        case PRINT:
            TaskPrint(stack, row_number);
            break;
        case POP:
            TaskPop(stack, row_number);
            break;
        default:
            break;
    }

}

bool ReadNumerToDegBy(size_t *var_idx, size_t row_number)
{
    bool success = false;
    *var_idx = 0;
    char c = fgetc(stdin);
    if(c != ' ')
        success = false;
    else
    {
      success = ReadNumberUnsignedLong(var_idx);
      c = fgetc(stdin);
    }
    if(!success || (c != '\n' && c != EOF))
    {
        BAD_INPUT(c);
        fprintf(stderr, "ERROR %ld DEG BY WRONG VARIABLE\n", row_number);
        return false;
    }
    ungetc(c, stdin);
    return true;
}

bool ReadNumberToAt(poly_coeff_t *x, size_t row_number)
{
    poly_coeff_t sign = 1;
    bool success = false;
    *x = 0;
    char c = fgetc(stdin);
    if(c != ' ')
        success = false;
    else
    {
        c = fgetc(stdin);
        if(c == '-')
            sign = -1;
        else
            ungetc(c, stdin);
        success = ReadNumberPolyCoeff(x, sign);
        c = fgetc(stdin);
    }
    if(!success || (c != '\n' && c != EOF))
    {
        BAD_INPUT(c);
        fprintf(stderr, "ERROR %ld AT WRONG VALUE\n", row_number);
        return false;;
    }
    ungetc(c, stdin);
    *x *= sign;
    return true;
}

void TaskZero(Stack *stack)
{
    Poly p = PolyZero();
    StackAdd(stack, p);
}

void TaskIsCoeff(Stack *stack, size_t row_number)
{
    if(StackSize(stack) == 0)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        bool is_coef = PolyIsCoeff(StackHead(stack));
        printf("%d\n", is_coef);
    }
}

void TaskIsZero(Stack *stack, size_t row_number)
{
    if(StackSize(stack) == 0)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        bool is_zero = PolyIsZero(StackHead(stack));
        printf("%d\n", is_zero);
    }
}

void TaskClone(Stack *stack, size_t row_number)
{
    if(StackSize(stack) == 0)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        Poly *p = StackHead(stack);
        Poly q = PolyClone(p);
        StackAdd(stack, q);
    }
}

void TaskAdd(Stack *stack, size_t row_number)
{
    if(StackSize(stack) < 2)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        Poly p = StackPop(stack);
        Poly q = StackPop(stack);
        Poly r = PolyAdd(&p, &q);
        PolyDestroy(&p);
        PolyDestroy(&q);
        StackAdd(stack, r);
    }
}

void TaskMul(Stack *stack, size_t row_number)
{
    if(StackSize(stack) < 2)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        Poly p = StackPop(stack);
        Poly q = StackPop(stack);
        Poly r = PolyMul(&p, &q);
        PolyDestroy(&p);
        PolyDestroy(&q);
        StackAdd(stack, r);
    }
}

void TaskNeg(Stack *stack, size_t row_number)
{
    if(StackSize(stack) == 0)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        Poly p = StackPop(stack);
        Poly q = PolyNeg(&p);
        PolyDestroy(&p);
        StackAdd(stack, q);
    }
}

void TaskSub(Stack *stack, size_t row_number)
{
    if(StackSize(stack) < 2)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        Poly p = StackPop(stack);
        Poly q = StackPop(stack);
        Poly r = PolySub(&p, &q);
        PolyDestroy(&p);
        PolyDestroy(&q);
        StackAdd(stack, r);
    }
}

void TaskIsEq(Stack *stack, size_t row_number)
{
    if(StackSize(stack) < 2)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        Poly *p = StackHead(stack);
        Poly *q = StackHead2(stack);
        bool is_eq = PolyIsEq(p, q);
        printf("%d\n", is_eq);
    }
}

void TaskDeg(Stack *stack, size_t row_number)
{
    if(StackSize(stack) == 0)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        poly_exp_t exp = PolyDeg(StackHead(stack));
        printf("%d\n", exp);
    }
}

void TaskDegBy(Stack *stack, size_t var_idx, size_t row_number)
{
    if(StackSize(stack) == 0)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        poly_exp_t exp = PolyDegBy(StackHead(stack), var_idx);
        printf("%d\n", exp);
    }
}

void TaskAt(Stack *stack, poly_coeff_t x, size_t row_number)
{
    if(StackSize(stack) == 0)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        Poly p = StackPop(stack);
        Poly q = PolyAt(&p, x);
        PolyDestroy(&p);
        StackAdd(stack, q);
    }
}

void PrintMono(Mono *m)
{
    printf("(");
    PrintPoly(&m->p);
    printf(",%d)", m->exp);
}

void PrintPoly(Poly *p)
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
        PrintMono(&p->arr[i]);
    }
}

void TaskPrint(Stack *stack, size_t row_number)
{
    if(StackSize(stack) == 0)
    {
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
        return;
    }

    PrintPoly(StackHead(stack));
    printf("\n");
}

void TaskPop(Stack *stack, size_t row_number)
{
    if(StackSize(stack) == 0)
        fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
    else
    {
        Poly p = StackPop(stack);
        PolyDestroy(&p);
    }
}

