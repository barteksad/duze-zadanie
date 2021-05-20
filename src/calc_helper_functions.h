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
 * Enum reprezentujący operacje wykonywane przez kalkulator
 */
enum taskType;

/**
 * Sprawdza czy dany znak jest literą.
 * @param[in] c : znak
 * @return czy litera?
 */
bool IsAlpha(char c);

/**
 * Sprawdza czy dany znak jest białym znakiem.
 * @param[in] c : znak
 * @return czy biały znak?
 */
bool IsWhitespace(char c);

/**
 * Wczytuje wielomian nie będący współczynnikiem
 * funkcja działa rekurencyjnie dla całej linii
 * @return wielomian
 */
Poly* ReadPoly();

/**
 * Wczytuje wielomian będący współczynnikiem
 * @return wielomian
 */
bool ReadPolyCoeff(Stack *stack, char c);

/**
 * Wczytuje liczbę typu poly_coeff_t.
 * @param[in] x zmienna do zapisu
 * @param[in] sign znak liczby
 * @return czy się udało?
 */
bool ReadNumberPolyCoeff(poly_coeff_t *x, poly_coeff_t sign);

/**
 * Wczytuje liczbę typu size_t.
 * @param[in] x zmienna do zapisu
 * @return czy się udało?
 */
bool ReadNumberUnsignedLong(size_t *x);

/**
 * Wczytuje liczbę typu poly_exp_t.
 * @param[in] x zmienna do zapisu
 * @return czy się udało?
 */
bool ReadNumberPolyExp(poly_exp_t *x);

/**
 * Wczytuje zadanie.
 * @param[in] stack stos z wielomianami
 * @param[in] size_t numer aktualnego wiersza
 */
void ReadTask(Stack *stack, char c, size_t row_number);

/**
 * Rozpoznaje zadanie na podstawie napisu.
 * @param[in] input_string wczytany napis
 * @return enum z typem zadania
 */
enum taskType DecodeTask(char* input_string);

/**
 * Wczytuje liczbę do zadania DEG_BY
 * zapewnia prawidłowe działanie po wczytaniu napisu DEG_BY 
 * @param[in] var_idx zmienna do zapisu
 * @param[in] row_number numer aktualnego wiersza
 * @return czy się udało?
 */
bool ReadNumerToDegBy(size_t *var_idx, size_t row_number);

/**
 * Wczytuje liczbę do zadania AT
 * zapewnia prawidłowe działanie po wczytaniu napisu AT 
 * @param[in] x zmienna do zapisu
 * @param[in] row_number numer aktualnego wiersza
 * @return czy się udało?
 */
bool ReadNumberToAt(poly_coeff_t *x, size_t row_number);

/**
 * Obsługa zadania ZERO.
 * @param[in] stack stos z wielomianami
 */
void TaskZero(Stack *stack);

/**
 * Obsługa zadania IS_COEFF.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void TaskIsCoeff(Stack *stack, size_t row_number);

/**
 * Obsługa zadania IS_ZERO.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void TaskIsZero(Stack *stack, size_t row_number);

/**
 * Obsługa zadania CLONE.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void TaskClone(Stack *stack, size_t row_number);

/**
 * Obsługa zadania ADD.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void TaskAdd(Stack *stack, size_t row_number);

/**
 * Obsługa zadania MUL.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void TaskMul(Stack *stack, size_t row_number);

/**
 * Obsługa zadania NEG.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void TaskNeg(Stack *stack, size_t row_number);

/**
 * Obsługa zadania SUB.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void TaskSub(Stack *stack, size_t row_number);

/**
 * Obsługa zadania IS_EQ.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void TaskIsEq(Stack *stack, size_t row_number);

/**
 * Obsługa zadania DEG.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void TaskDeg(Stack *stack, size_t row_number);

/**
 * Obsługa zadania DEG_BY.
 * @param[in] stack stos z wielomianami
 * @param[in] var_idx wartość dla polecenia DEG_BY
 * @param[in] row_number numer aktualnego wiersza
 */
void TaskDegBy(Stack *stack, size_t var_idx, size_t row_number);

/**
 * Obsługa zadania AT.
 * @param[in] stack stos z wielomianami
 * @param[in] poly_coeff_t wartość dla polecenia AT
 * @param[in] row_number numer aktualnego wiersza
 */
void TaskAt(Stack *stack, poly_coeff_t x, size_t row_number);

/**
 * Obsługa zadania PRINT.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void TaskPrint(Stack *stack, size_t row_number);

/**
 * Obsługa zadania POP.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void TaskPop(Stack *stack, size_t row_number);

/**
 * Wypisuje wielomian
 * @param[in] p wielomian
 */
void PrintPoly(Poly *p);

/**
 * Wypisuje jednomian
 * @param[in] m jednomian
 */
void PrintMono(Mono *m);

