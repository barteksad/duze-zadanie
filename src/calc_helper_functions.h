/** @file
  Interfejs kalkulatora wielomianów rzadkich wielu zmiennych

  @author Bartłomiej Sadlej
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#pragma once
#include "stack.h"
#include <stdio.h>

enum taskType;

/**
 * Sprawdza czy dany znak jest literą.
 * @param[in] c : znak
 * @return czy litera?
 */
bool isAlpha(char c);

/**
 * Sprawdza czy dany znak jest białym znakiem.
 * @param[in] c : znak
 * @return czy biały znak?
 */
bool isWhitespace(char c);

/**
 * Wczytuje wielomian nie będący współczynnikiem
 * funkcja działa rekurencyjnie dla całej linii
 * @return wielomian
 */
Poly* readPoly();

/**
 * Wczytuje wielomian będący współczynnikiem
 * @return wielomian
 */
bool readPolyCoeff(Stack *stack, char c);

/**
 * Wczytuje liczbę typu poly_coeff_t.
 * @param[in] x zmienna do zapisu
 * @param[in] sign znak liczby
 * @return czy się udało?
 */
bool readNumberPolyCoeff(poly_coeff_t *x, poly_coeff_t sign);

/**
 * Wczytuje liczbę typu size_t.
 * @param[in] x zmienna do zapisu
 * @return czy się udało?
 */
bool readNumberUnsignedLong(size_t *x);

/**
 * Wczytuje liczbę typu poly_exp_t.
 * @param[in] x zmienna do zapisu
 * @return czy się udało?
 */
bool readNumberPolyExp(poly_exp_t *x);

/**
 * Wczytuje zadanie.
 * @param[in] stack stos z wielomianami
 * @param[in] size_t numer aktualnego wiersza
 */
void readTask(Stack *stack, char c, size_t row_number);

/**
 * Rozpoznaje zadanie na podstawie napisu.
 * @param[in] input_string wczytany napis
 * @return enum z typem zadania
 */
enum taskType decodeTask(char* input_string);

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
void taskZero(Stack *stack);

/**
 * Obsługa zadania IS_COEFF.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void taskIsCoeff(Stack *stack, size_t row_number);

/**
 * Obsługa zadania IS_ZERO.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void taskIsZero(Stack *stack, size_t row_number);

/**
 * Obsługa zadania CLONE.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void taskClone(Stack *stack, size_t row_number);

/**
 * Obsługa zadania ADD.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void taskAdd(Stack *stack, size_t row_number);

/**
 * Obsługa zadania MUL.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void taskMul(Stack *stack, size_t row_number);

/**
 * Obsługa zadania NEG.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void taskNeg(Stack *stack, size_t row_number);

/**
 * Obsługa zadania SUB.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void taskSub(Stack *stack, size_t row_number);

/**
 * Obsługa zadania IS_EQ.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void taskIsEq(Stack *stack, size_t row_number);

/**
 * Obsługa zadania DEG.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void taskDeg(Stack *stack, size_t row_number);

/**
 * Obsługa zadania DEG_BY.
 * @param[in] stack stos z wielomianami
 * @param[in] var_idx wartość dla polecenia DEG_BY
 * @param[in] row_number numer aktualnego wiersza
 */
void taskDegBy(Stack *stack, size_t var_idx, size_t row_number);

/**
 * Obsługa zadania AT.
 * @param[in] stack stos z wielomianami
 * @param[in] poly_coeff_t wartość dla polecenia AT
 * @param[in] row_number numer aktualnego wiersza
 */
void taskAt(Stack *stack, poly_coeff_t x, size_t row_number);

/**
 * Obsługa zadania PRINT.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void taskPrint(Stack *stack, size_t row_number);

/**
 * Obsługa zadania POP.
 * @param[in] stack stos z wielomianami
 * @param[in] row_number numer aktualnego wiersza
 */
void taskPop(Stack *stack, size_t row_number);

/**
 * Wypisuje wielomian
 * @param[in] p wielomian
 */
void printPoly(Poly *p);

/**
 * Wypisuje jednomian
 * @param[in] m jednomian
 */
void printMono(Mono *m);

