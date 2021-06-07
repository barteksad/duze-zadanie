/** @file
 * Makra używane w programie
  @author Bartłomiej Sadlej
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#pragma once

/**
 *  wczytuje znaki do końca złego wiersza
 */
#define BAD_INPUT(c) \
  do { \
    while(c != '\n' && c != EOF) { \
      c=getc(stdin); \
    } \
    ungetc(c, stdin); \
  } while (0)

/**
 *  sprawdza poprawność alokacji pamięci
 */
#define CHECK_PTR(p)  \
  do {                \
    if (p == NULL) {  \
      exit(1);        \
    }                 \
  } while (0)

/**
 * zwraca większą z liczb
 */
#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
