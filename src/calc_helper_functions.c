/** @file
  Implementacja interfejsu kalkulatora wielomianów rzadkich wielu zmiennych

  @author Bartłomiej Sadlej
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#pragma once
#include "calc_helper_functions.h"
#include "macros.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#define POLY_EXP_MAX 2147483647 // specyfikacja zadania
#define MAX_TASK_LEN 8 // IS_COEFF

/**
 * Enum reprezentujący operacje wykonywane przez kalkulator
 */
static enum taskType
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
    COMPOSE,
    __INVALID__
};


bool IsWhitespace(int c)
{
// '\n' nie jest tu celowo rozpatrywany
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

bool IsAlpha(int c)
{
    if(c >= 'A' && c <= 'Z')
        return true;
    if(c >= 'a' && c <= 'z')
        return true;
    return false;
}

/**
 * Rozpoznaje zadanie na podstawie napisu.
 * @param[in] input_string wczytany napis
 * @return enum z typem zadania
 */
static enum taskType DecodeTask(char* input_string)
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
    if(!strcmp(input_string, "COMPOSE"))
        return COMPOSE;
    return __INVALID__;
    
}

/**
 * Obsługa zadania ZERO.
 * @param[in] stack stos z wielomianami
 */
static void TaskZero(Stack *stack)
{
    Poly p = PolyZero();
    StackAdd(stack, p);
}

/**
 * Obsługa zadania IS_COEFF.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
static void TaskIsCoeff(Stack *stack, size_t row_number)
{
    if(StackSize(stack) == 0)
        PrintError(STACK_UNDERFLOW, row_number);
    else
    {
        bool is_coef = PolyIsCoeff(StackHead(stack));
        printf("%d\n", is_coef);
    }
}

/**
 * Obsługa zadania IS_ZERO.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
static void TaskIsZero(Stack *stack, size_t row_number)
{
    if(StackSize(stack) == 0)
        PrintError(STACK_UNDERFLOW, row_number);
    else
    {
        bool is_zero = PolyIsZero(StackHead(stack));
        printf("%d\n", is_zero);
    }
}

/**
 * Obsługa zadania CLONE.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
static void TaskClone(Stack *stack, size_t row_number)
{
    if(StackSize(stack) == 0)
        PrintError(STACK_UNDERFLOW, row_number);
    else
    {
        Poly *p = StackHead(stack);
        Poly q = PolyClone(p);
        StackAdd(stack, q);
    }
}

/**
 * Obsługa zadania ADD.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
static void TaskAdd(Stack *stack, size_t row_number)
{
    if(StackSize(stack) < 2)
        PrintError(STACK_UNDERFLOW, row_number);
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

/**
 * Obsługa zadania MUL.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
static void TaskMul(Stack *stack, size_t row_number)
{
    if(StackSize(stack) < 2)
        PrintError(STACK_UNDERFLOW, row_number);
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

/**
 * Obsługa zadania NEG.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
static void TaskNeg(Stack *stack, size_t row_number)
{
    if(StackSize(stack) == 0)
        PrintError(STACK_UNDERFLOW, row_number);
    else
    {
        Poly p = StackPop(stack);
        Poly q = PolyNeg(&p);
        PolyDestroy(&p);
        StackAdd(stack, q);
    }
}

/**
 * Obsługa zadania SUB.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
static void TaskSub(Stack *stack, size_t row_number)
{
    if(StackSize(stack) < 2)
        PrintError(STACK_UNDERFLOW, row_number);
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

/**
 * Obsługa zadania IS_EQ.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
static void TaskIsEq(Stack *stack, size_t row_number)
{
    if(StackSize(stack) < 2)
        PrintError(STACK_UNDERFLOW, row_number);
    else
    {
        Poly *p = StackHead(stack);
        Poly *q = StackHead2(stack);
        bool is_eq = PolyIsEq(p, q);
        printf("%d\n", is_eq);
    }
}

/**
 * Obsługa zadania DEG.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
static void TaskDeg(Stack *stack, size_t row_number)
{
    if(StackSize(stack) == 0)
        PrintError(STACK_UNDERFLOW, row_number);
    else
    {
        poly_exp_t exp = PolyDeg(StackHead(stack));
        printf("%d\n", exp);
    }
}

/**
 * Obsługa zadania DEG_BY.
 * @param[in] stack stos z wielomianami
 * @param[in] var_idx wartość dla polecenia DEG_BY
 * @param[in] row_number numer aktualnego wiersza
 */
static void TaskDegBy(Stack *stack, size_t var_idx, size_t row_number)
{
    if(StackSize(stack) == 0)
        PrintError(STACK_UNDERFLOW, row_number);
    else
    {
        poly_exp_t exp = PolyDegBy(StackHead(stack), var_idx);
        printf("%d\n", exp);
    }
}

/**
 * Obsługa zadania AT.
 * @param[in] stack stos z wielomianami
 * @param[in] poly_coeff_t wartość dla polecenia AT
 * @param[in] row_number numer aktualnego wiersza
 */
static void TaskAt(Stack *stack, poly_coeff_t x, size_t row_number)
{
    if(StackSize(stack) == 0)
        PrintError(STACK_UNDERFLOW, row_number);
    else
    {
        Poly p = StackPop(stack);
        Poly q = PolyAt(&p, x);
        PolyDestroy(&p);
        StackAdd(stack, q);
    }
}

/**
 * Wypisuje wielomian
 * @param[in] p wielomian
 */
static void PrintPoly(Poly *p);

/**
 * Wypisuje jednomian
 * @param[in] m jednomian
 */
static void PrintMono(Mono *m)
{
    printf("(");
    PrintPoly(&m->p);
    printf(",%d)", m->exp);
}

static void PrintPoly(Poly *p)
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

/**
 * Obsługa zadania PRINT.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
static void TaskPrint(Stack *stack, size_t row_number)
{
    if(StackSize(stack) == 0)
    {
        PrintError(STACK_UNDERFLOW, row_number);
        return;
    }

    PrintPoly(StackHead(stack));
    printf("\n");
}

/**
 * Obsługa zadania POP.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
static void TaskPop(Stack *stack, size_t row_number)
{
    if(StackSize(stack) == 0)
        PrintError(STACK_UNDERFLOW, row_number);
    else
    {
        Poly p = StackPop(stack);
        PolyDestroy(&p);
    }
}

/**
 * Obsługa zadania COMPOSE.
 * @param[in] stack stos z wielomianami
 * @param[in] k liczba wielomianów do wykorzystania przy operacji COMPOSE
 * @param[in] row_number numer aktualnego wiersza
 */
static void TaskCompose(Stack *stack, size_t k, size_t row_number)
{
    if(StackSize(stack) < k + 1 ||  k == ULONG_MAX)
    {
        PrintError(STACK_UNDERFLOW, row_number);
        return;
    }
    else
    {
        Poly p = StackPop(stack);
        Poly *q = malloc(k * sizeof(Poly)); 
        CHECK_PTR(q);
    
        for(size_t i = 1; i <= k; i++)
            q[k - i] = StackPop(stack);
    
        Poly p2 = PolyCompose(&p, k, q);
        PolyDestroy(&p);
        StackAdd(stack, p2);
    
        for(size_t i = 0; i < k; i++)
            PolyDestroy(&q[i]);
        free(q);
    }
}

void ReadTask(Stack *stack, int c, size_t row_number)
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
                        PrintError(DEG_BY_WRONG_VARIABLE, row_number);
                        break;
                    case AT:
                        PrintError(AT_WRONG_VALUE, row_number);
                        break;
                    default:
                        PrintError(INVALID_COMMAND, row_number);
                        break;
                }
            }
            else
                PrintError(INVALID_COMMAND, row_number);

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
        PrintError(INVALID_COMMAND, row_number);
        return;
    }
    
    if(task == DEG_BY || task == AT || task == COMPOSE)
    {
        bool success = true;
        c = fgetc(stdin);
        if(c != ' ')
            success = false;
        else
        {
            c = fgetc(stdin);
            if((!('0' <= c && c <= '9') && !(c == '-' && task == AT)))
                success = false;
            else
                ungetc(c, stdin);
        }
        if(!success)
        {
            BAD_INPUT(c);
            switch(task)
            {
                case DEG_BY:
                    PrintError(DEG_BY_WRONG_VARIABLE, row_number);
                    break;
                case AT:
                    PrintError(AT_WRONG_VALUE, row_number);
                    break;
                case COMPOSE:
                    PrintError(COMPOSE_WRONG_VALUE, row_number);
                    break;
                default:
                    break;
            }            
            return;
        }
    }
    size_t var_idx;
    poly_coeff_t x;

    if(task == DEG_BY)
    {
        if(scanf("%lu", &var_idx) != 1 || errno != 0)
        {
            BAD_INPUT(c);
            PrintError(DEG_BY_WRONG_VARIABLE, row_number);
            return;
        }
    }
    else if(task == AT)
    {
        if(scanf("%ld", &x) != 1 || errno != 0)
        {
            BAD_INPUT(c);
            PrintError(AT_WRONG_VALUE, row_number);      
            return;  
        }
    }
    else if(task == COMPOSE)
    {
        if(scanf("%lu", &var_idx) != 1 || errno != 0)
        {
            BAD_INPUT(c);
            PrintError(COMPOSE_WRONG_VALUE, row_number);
            return;
        }
    }

    c = fgetc(stdin);
    if(c != '\n' && c != EOF)
    {
        BAD_INPUT(c);
        switch(task)
        {
            case DEG_BY:
                PrintError(DEG_BY_WRONG_VARIABLE, row_number);
                break;
            case AT:
                PrintError(AT_WRONG_VALUE, row_number);
                break;
            case COMPOSE:
                PrintError(COMPOSE_WRONG_VALUE, row_number);
                break;
            default:
                PrintError(INVALID_COMMAND, row_number);
                break;
        }
        return;
    }
    ungetc(c, stdin);

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
        case COMPOSE:
            TaskCompose(stack, var_idx, row_number);
            break;
        default:
            break;
    }

}

Poly* ReadPoly()
{
    int c = fgetc(stdin);
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
        {
            BAD_INPUT(c);
            return NULL;
        }
    }
    else if(c == '-' || (c >= '0' && c <= '9')) // wczytywanie coeff-a
    {
        ungetc(c, stdin);
        poly_coeff_t coeff = 0;
        int success = scanf("%lu", &coeff);
        if(success != 1 || errno != 0)
        {
            if(p)
            {
                PolyDestroy(p);
                free(p);
            }
            BAD_INPUT(c);
            return NULL;
        }

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
        int success = scanf("%d", &exp);
        if(exp > POLY_EXP_MAX || exp < 0 ) 
            success = 0;
        if(success != 1 || errno != 0)
        {
            PolyDestroy(p);
            free(p);
            BAD_INPUT(c);
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
        CHECK_PTR(w);
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

bool ReadPolyCoeff(Stack *stack)
{
    char c;
    poly_coeff_t value = 0;
    int success = scanf("%ld", &value);
    if(success == 1 && errno == 0)
    {
        c = fgetc(stdin);
        if(c != '\n' && c != EOF)
            success = false;
        ungetc(c, stdin);
    }

    if(success != 1 || errno != 0) 
    {
        BAD_INPUT(c);
        return false;
    }

    Poly p = PolyFromCoeff(value);
    StackAdd(stack, p);
    return true;
}

void PrintError(enum errorType error, size_t row_number)
{
    switch(error)
    {
        case STACK_UNDERFLOW:
            fprintf(stderr, "ERROR %ld STACK UNDERFLOW\n", row_number);
            break;
        case INVALID_COMMAND:
            fprintf(stderr, "ERROR %ld WRONG COMMAND\n", row_number);
            break;
        case DEG_BY_WRONG_VARIABLE:
            fprintf(stderr, "ERROR %ld DEG BY WRONG VARIABLE\n", row_number);
            break;
        case AT_WRONG_VALUE:
            fprintf(stderr, "ERROR %ld AT WRONG VALUE\n", row_number);
            break;
        case COMPOSE_WRONG_VALUE:
            fprintf(stderr, "ERROR %ld COMPOSE WRONG PARAMETER\n", row_number);
            break;
        case WRONG_POLY:
            fprintf(stderr, "ERROR %ld WRONG POLY\n", row_number);
            break;
        default:
            assert(false);
            break;
    }
}