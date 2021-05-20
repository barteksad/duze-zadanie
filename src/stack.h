/** @file
  Interfejs struktury danych stos

  @author Bartłomiej Sadlej
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#pragma once
#include "poly.h"

/**
 * Deklaracja typu stosu Stack.
 */
typedef struct Stack Stack;

/**
 * Tworzy nowy pusty stos
 * @return wskaźnik na nowy stos
 */
Stack* createNewStack();

/**
 * Usuwa i zwraca wielomian z wierzchu stosu
 * @param[in] stack : wskaźnik na stos
 * @return wielomian z wierzchu stosu
 */
Poly StackPop(Stack *stack);

/**
 * Dodaje wielomian na stos
 * @param[in] stack : wskaźnik na stos
 * @param[in] p : wielomian
 */
void StackAdd(Stack *stack, Poly p);

/**
 * Zwraca rozmiar stosu
 * @param[in] stack : wskaźnik na stos
 * @return rozmiar stosu
 */
size_t StackSize(Stack *stack);

/**
 * Zwraca wielomian z wierzchołku stosu ale go nie usuwa
 * @param[in] stack : wskaźnik na stos
 * @return wielomian z wierzchołku stosu
 */
Poly* StackHead(Stack *stack);

/**
 * Zwraca drugi w kolejności wielomian z wierzchołku stosu ale go nie usuwa
 * @param[in] stack : wskaźnik na stos
 * @return drugi w kolejności wielomian z wierzchołku stosu
 */
Poly* StackHead2(Stack *stack);

/**
 * Usuwa stos
 * @param[in] stack : wskaźnik na stos
 */
void StackFree(Stack *stack);


