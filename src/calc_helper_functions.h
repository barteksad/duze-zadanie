/** @file
  Interfejs kalkulatora wielomianów rzadkich wielu zmiennych

  @author Bartłomiej Sadlej
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#pragma once
#include "stack.h"

#include <stdio.h>

/**
 * Enum reprezentujący możliwe błędy
 */
enum errorType
{
    INVALID_COMMAND,
    WRONG_POLY,
    DEG_BY_WRONG_VARIABLE,
    AT_WRONG_VALUE,
    COMPOSE_WRONG_VALUE,
    STACK_UNDERFLOW,
};

/**
 * Sprawdza czy dany znak jest literą.
 * @param[in] c : znak
 * @return czy litera?
 */
bool IsAlpha(int c);

/**
 * Sprawdza czy dany znak jest białym znakiem.
 * @param[in] c : znak
 * @return czy biały znak?
 */
bool IsWhitespace(int c);

/**
 * Wczytuje wielomian nie będący współczynnikiem
 * funkcja działa rekurencyjnie dla całej linii
 * @return wielomian
 */
Poly* ReadPoly();

/**
 * Wczytuje wielomian będący współczynnikiem
 * @param[in] stack stos z wielomianami
 * @return wielomian
 */
bool ReadPolyCoeff(Stack *stack);

/**
 * Wczytuje zadanie.
 * @param[in] stack stos z wielomianami
 * @param[in] c pierwszy znak z aktualnie wczytywaniej lini
 * @param[in] row_number numer aktualnego wiersza
 */
void ReadTask(Stack *stack, int c, size_t row_number);

/**
 * Wypisuje komunikat o błędzie.
 * @param[in] error typ błędu
 * @param[in] row_number numer aktualnego wiersza
 */
void PrintError(enum errorType error, size_t row_number);