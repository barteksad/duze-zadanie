/** @file
  Implementacja interfejsu klasy wielomianów rzadkich wielu zmiennych

  @author Bartłomiej Sadlej
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include "poly.h"
#include <stdlib.h> // qsort

#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define CHECK_PTR(p)  \
  do {                \
    if (p == NULL) {  \
      exit(1);        \
    }                 \
  } while (0)


Poly PolyAdd(const Poly *p, const Poly *q)
{
  /*
    Funkcja rozpatruje dwa osobne przypadki:
    I. choć jeden z wielomianów jest współczynnikiem
    II. obydwa są wielomianami nie będącymi współczynnikiem
  */

  /*
    I.  Jeżeli p lub q są współczynnikami to należy zsumować współczynniki przy zerowej potędze oraz
        skopiować ewentualne pozostałe jednomiany z drugiego wielomianu
  */
  if (PolyIsCoeff(p) || PolyIsCoeff(q))
  {
    if (PolyIsCoeff(q))
    {
      if (PolyIsZero(q))
        return PolyClone(p); 
      if (PolyIsCoeff(p))
        return PolyFromCoeff(p->coeff + q->coeff);

      Mono *new_monos_buffer;
      size_t zero_index_used = 0;
      size_t first_p_mono_used = 0;
      size_t new_monos_buffer_size;

      // jak drugi wielomian ma niezerowy współczynnik przy zerowej potędze trzeba ten współczynnik dodać
      // i sprawdzić czy nie jest zerowy po dodaniu, od tego zależy długość (Mono arr) w nowym wielomianie
      if (p->arr[0].exp == 0)
      {
        // przypadek q = c_1, p = x_0^0(c_2 + ... ) + 
        if(!PolyIsCoeff(&p->arr[0].p))
        {  
          Poly temp = PolyAdd(&p->arr[0].p, q);
          if (PolyIsZero(&temp))
          {
            if (p->size == 1)
              return PolyZero();
            new_monos_buffer_size = p->size - 1;
            new_monos_buffer = (Mono *)malloc(sizeof(Mono) * new_monos_buffer_size);
            CHECK_PTR(new_monos_buffer);
          }
          else
          {
            new_monos_buffer_size = p->size;
            new_monos_buffer = (Mono *)malloc(sizeof(Mono) * new_monos_buffer_size);
            CHECK_PTR(new_monos_buffer);
            new_monos_buffer[0] = MonoFromPoly(&temp, 0);
            zero_index_used = 1;
          }
        }
        // przypadek q = c_1, p = x_0^0 * c_2
        else
        {
          poly_coeff_t c = p->arr[0].p.coeff + q->coeff;
          if (c == 0)
          {
            if (p->size == 1)
              return PolyZero();
            new_monos_buffer_size = p->size - 1;
            new_monos_buffer = (Mono *)malloc(sizeof(Mono) * new_monos_buffer_size);
            CHECK_PTR(new_monos_buffer);
          }
          else
          {
            new_monos_buffer_size = p->size;
            new_monos_buffer = (Mono *)malloc(sizeof(Mono) * new_monos_buffer_size);
            CHECK_PTR(new_monos_buffer);
            Poly temp = PolyFromCoeff(c);
            new_monos_buffer[0] = MonoFromPoly(&temp, 0);
            zero_index_used = 1;
          }
        }
        first_p_mono_used = 1;
      }
      // przypadek q = c_1, p = x_0^(nie zero) * ( ... )
      else
      {
        new_monos_buffer_size = p->size + 1;
        new_monos_buffer = (Mono *)malloc(sizeof(Mono) * new_monos_buffer_size);
        CHECK_PTR(new_monos_buffer);
        Poly temp = PolyFromCoeff(q->coeff);
        new_monos_buffer[0] = MonoFromPoly(&temp, 0);
        zero_index_used = 1;

      }
      // potem przepisujemy kopiujemy reszte z drugiego wielomianu
      for(size_t i = 0; i < p->size - first_p_mono_used; i++)
        new_monos_buffer[zero_index_used + i] = MonoClone(&p->arr[i + first_p_mono_used]);
      return (Poly) {.size = new_monos_buffer_size, .arr = new_monos_buffer};

    }
    else
      return PolyAdd(q, p);
  }

  // jak dwa różne to po kolei względem rosnących wykładników dodajemy
  size_t p_size = p->size, p_iter = 0;
  size_t q_size = q->size, q_iter = 0;
  size_t new_monos_added_count = 0;
  Mono *new_monos_buffer = (Mono *)malloc(sizeof(Mono) * (p_size + q_size));
  CHECK_PTR(new_monos_buffer);

  while(p_iter < p_size || q_iter < q_size) {
    if (p_iter == p_size)
    {
      new_monos_buffer[new_monos_added_count++] = MonoClone(&q->arr[q_iter++]);
      continue;
    }
    if (q_iter == q_size)
    {
      new_monos_buffer[new_monos_added_count++] = MonoClone(&p->arr[p_iter++]);
      continue;
    }
    
    if(p->arr[p_iter].exp == q->arr[q_iter].exp)
    {
      // jeśli taki sam dodajemy wielomiany będące współczynnikami obudwu, jeśli dostaniemy zero, nic nie wstawiamy do tablicy
      Poly newMaybeZeroCoeff = PolyAdd(&p->arr[p_iter].p, &q->arr[q_iter].p);
      if (!PolyIsZero(&newMaybeZeroCoeff))
      {
        new_monos_buffer[new_monos_added_count].exp = p->arr[p_iter].exp;
        new_monos_buffer[new_monos_added_count].p = newMaybeZeroCoeff;
        new_monos_added_count++;
      }
      p_iter++;
      q_iter++;
    }
    else
    {
      if(p->arr[p_iter].exp < q->arr[q_iter].exp)
        new_monos_buffer[new_monos_added_count++] = MonoClone(&p->arr[p_iter++]);
      else
        new_monos_buffer[new_monos_added_count++] = MonoClone(&q->arr[q_iter++]);
    }
  }

  if (new_monos_added_count == 0)
  {
    free(new_monos_buffer);
    return PolyZero();
  }
  // zmniejszamy rozmiar tablicy jeśli nie potrzeba aż tyle miejsca
  if (new_monos_added_count < p_size + q_size)
  {
    new_monos_buffer = (Mono *)realloc(new_monos_buffer, sizeof(Mono) * new_monos_added_count);
    CHECK_PTR(new_monos_buffer);
  }

  // przypadek gdy po dodaniu mamy wielomian złożony z jednomiany stopnia zero to zwracamy jako wielomian stały
  if(new_monos_added_count == 1 && new_monos_buffer[0].exp == 0 && PolyIsCoeff(&new_monos_buffer[0].p))
  {
    poly_coeff_t coeff = new_monos_buffer[0].p.coeff;
    free(new_monos_buffer);
    return PolyFromCoeff(coeff);
  }

  return (Poly) {.size = new_monos_added_count, .arr = new_monos_buffer};

}

int compareMonosByExp(const void * lhs, const void * rhs)
{
  if (((Mono *)lhs)->exp < ((Mono *)rhs)->exp)
      return -1;
  if (((Mono *)lhs)->exp > ((Mono *)rhs)->exp)
      return 1;
  return 0;
}

Poly PolyAddMonos(size_t count, const Mono monos[])
{
    if(count == 0)
      return PolyZero();
    /*
      sortuje tablice monos i po kolei dodaje do nowej tablicy
      uwzględnia powtarzające się potęgi oraz zerujące współczynniki i zlicza adekwatnie rozmiar
    */
    Mono* new_monos_buffer = (Mono*)malloc(count * sizeof(Mono));
    qsort((Mono *)monos, count, sizeof(Mono), compareMonosByExp);
    CHECK_PTR(new_monos_buffer);
    size_t new_monos_added_count = 0;
    new_monos_buffer[new_monos_added_count++] = monos[0];

    for(size_t i = 1;i < count; i++)
    {
        if (new_monos_added_count == 0 || monos[i].exp != new_monos_buffer[new_monos_added_count - 1].exp)
        {
            new_monos_buffer[new_monos_added_count++] = monos[i];
            continue;
        }
        else
        {
            Poly newMaybeZeroCoeff = PolyAdd(&monos[i].p, &new_monos_buffer[new_monos_added_count - 1].p);
            poly_exp_t current_exp = monos[i].exp;
            PolyDestroy((Poly *)&monos[i].p);
            MonoDestroy(&new_monos_buffer[new_monos_added_count - 1]);
            if (PolyIsZero(&newMaybeZeroCoeff))
                new_monos_added_count--;
            else
            {
                new_monos_buffer[new_monos_added_count - 1].exp = current_exp;
                new_monos_buffer[new_monos_added_count - 1].p = newMaybeZeroCoeff;
            }
        }

    }

    // zmniejszamy rozmiar tablicy jeśli nie potrzeba aż tyle miejsca
    if (new_monos_added_count < count)
    {
      if (new_monos_added_count == 0)
      {
        free(new_monos_buffer);
        return PolyZero();
      }
      new_monos_buffer = (Mono *)realloc(new_monos_buffer, sizeof(Mono) * new_monos_added_count);
      CHECK_PTR(new_monos_buffer);
    }

    // przypadek gdy po dodaniu mamy wielomian złożony z jednomiany stopnia zero to zwracamy jako wielomian stały
    if (new_monos_added_count == 1 && new_monos_buffer[0].exp == 0 && PolyIsCoeff(&new_monos_buffer[0].p))
    {
      poly_coeff_t coeff = new_monos_buffer[0].p.coeff;
      MonoDestroy(&new_monos_buffer[0]);
      free(new_monos_buffer);
      return PolyFromCoeff(coeff);
    }
    else
      return (Poly) {.size = new_monos_added_count, .arr = new_monos_buffer};
}

void PolyDestroy(Poly *p)
{
  if (p->arr)
  {
    for(size_t i = 0;i < p->size; i++)
      MonoDestroy(&p->arr[i]);
    free(p->arr);
  }
} 

bool PolyIsEq(const Poly *p, const Poly *q)
{
  /*
    porównuje wykładniki wielomianów a potem jednomiany składowe obydwu
  */

  bool p_is_coeff = PolyIsCoeff(p);
  bool q_is_coeff = PolyIsCoeff(q);

  if (p_is_coeff && q_is_coeff)
    return (p->coeff == q->coeff);
  // przypadek gdy p = c_1, q = x_0^0 * c_1 i na odwrót
  // if (p_is_coeff && !q_is_coeff && q->arr[0].exp == 0 && PolyIsCoeff(&q->arr[0].p))
  //   return (PolyIsEq(p, &q->arr[0].p));
  // if (q_is_coeff && !p_is_coeff && p->arr[0].exp == 0 && PolyIsCoeff(&p->arr[0].p))
  //   return (PolyIsEq(q, &p->arr[0].p));  
  
  if (PolyIsCoeff(p) ^ PolyIsCoeff(q))
    return false;
  if (p->size != q->size)
    return false;

  for(size_t i = 0; i < p->size; i++)
  {
    if (p->arr[i].exp != q->arr[i].exp)
      return false;
    if(!PolyIsEq(&p->arr[i].p, &q->arr[i].p))
      return false;
  }
  return true;
}

Poly PolyClone(const Poly *p)
{
  if (PolyIsCoeff(p))
    return PolyFromCoeff(p->coeff);
  /*
    po kolei klonuje składowe jednomiany
  */
  size_t p_size = p->size;
  Mono* copied_monos = (Mono *)malloc(p_size * sizeof(Mono));
  CHECK_PTR(copied_monos);
  for(size_t i = 0; i < p_size; i++)
    copied_monos[i] = MonoClone(&p->arr[i]);

  return (Poly) {.size = p_size, .arr = copied_monos};
}

Poly PolyMul(const Poly *p, const Poly *q)
{
  /*
    sumuje wykładniki i rekurencyjnie mnoży każdy składowy jednomian z jednego z każdym składowym jednomianem z drugiego
    tak uzyskane jednomiany sumuje funckją PolyAddMonos()
  */

  bool p_is_coeff = PolyIsCoeff(p);
  bool q_is_coeff = PolyIsCoeff(q);
  
  if (p_is_coeff || q_is_coeff)
  {
    if (PolyIsZero(p) || PolyIsZero(q))
      return PolyZero();
    if (p_is_coeff && q_is_coeff)
      return PolyFromCoeff(p->coeff * q->coeff);

    if (q_is_coeff)
    {
      // jeśli q to współczynnik to mnożymy każdy z jednomianów składowych p przez niego i zapisujemy to w nowej tablicy
      size_t new_monos_num = p->size;
      size_t not_used_slots = 0;
      Mono* multiplied_monos_array = (Mono*)malloc(new_monos_num * sizeof(Mono));
      CHECK_PTR(multiplied_monos_array);


      for (size_t i = 0; i < new_monos_num; i++)
      {
        multiplied_monos_array[i - not_used_slots].p = PolyMul(&p->arr[i].p, q);
        if (PolyIsZero(&multiplied_monos_array[i - not_used_slots].p))
        {
          MonoDestroy(&multiplied_monos_array[i - not_used_slots]);
          not_used_slots++;
        }
        else
          multiplied_monos_array[i - not_used_slots].exp = p->arr[i].exp;
      }
      
      // zmniejszamy rozmiar tablicy jeśli nie potrzeba aż tyle miejsca
      if (not_used_slots > 0)
      {
        new_monos_num -= not_used_slots;
        // jeśli wszystkie wysumowane po mnożeniu się wyzerowały to zwracamy zero    
        if(new_monos_num == 0)
        {
          free(multiplied_monos_array);
          return PolyZero();
        }
        multiplied_monos_array = (Mono*)realloc(multiplied_monos_array, new_monos_num * sizeof(Mono));
        CHECK_PTR(multiplied_monos_array);
      }
      return (Poly) {.size = new_monos_num, .arr = multiplied_monos_array};
    }
    else
      return PolyMul(q, p);
  }

  // mnożymy każdy z każdym więc potrzeba tyle miejsca
  size_t new_monos_num = p->size * q->size;
  // potrzebne do zliczania wielomianów zerowych dostanych z mnożenia
  size_t not_used_slots = 0;
  Mono* multiplied_monos_array = (Mono*)malloc(new_monos_num * sizeof(Mono));
  for (size_t p_i = 0; p_i < p->size; p_i++)
    for (size_t q_i = 0; q_i < q->size; q_i++)
    {
      // mnożenie polega na zsumowaniu wykładników i wymnożeniu rekurencyjnie wielomianów będących współczynnikamy przy zmiennej danej potęgi
      multiplied_monos_array[p_i * q->size + q_i - not_used_slots].exp = p->arr[p_i].exp + q->arr[q_i].exp;
      multiplied_monos_array[p_i * q->size + q_i - not_used_slots].p = PolyMul(&p->arr[p_i].p, &q->arr[q_i].p);
      // być może jako wynik mnożenia dostaniemy zero
      if (PolyIsZero(&multiplied_monos_array[p_i * q->size + q_i - not_used_slots].p))
      {
        MonoDestroy(&multiplied_monos_array[p_i * q->size + q_i - not_used_slots]);
        not_used_slots++;
      }
    }

  // zmniejszamy rozmiar tablicy jeśli nie potrzeba aż tyle miejsca
  if (not_used_slots > 0)
  {
    new_monos_num -= not_used_slots;
    // jeśli wszystkie wysumowane po mnożeniu się wyzerowały to zwracamy zero    
    if (new_monos_num == 0)
    {
      free(multiplied_monos_array);
      return PolyZero();
    }
    
    multiplied_monos_array = (Mono *)realloc(multiplied_monos_array, new_monos_num * sizeof(Mono));
    CHECK_PTR(multiplied_monos_array);
  }
  
  // używamy funkcji PolyAddMonos żeby poprawnie zsumować nowe jednomiany
  Poly result = PolyAddMonos(new_monos_num, multiplied_monos_array);
  free(multiplied_monos_array);
  return result;
}

Poly PolyNeg(const Poly *p)
{
  /*
    mnoży wielomian p przez wielomian stały = -1
  */
  Poly result, temp = PolyFromCoeff(-1);
  result = PolyMul(p, &temp);
  PolyDestroy(&temp);
  return result;
}

Poly PolySub(const Poly *p, const Poly *q)
{
  /*
    mnoży wielomian q przez wielomian stały = -1 i sumuje go z p
  */
  Poly result, temp = PolyNeg(q);
  result = PolyAdd(p, &temp);
  PolyDestroy(&temp);
  return result;
}

poly_exp_t PolyDeg(const Poly *p)
{
  /*
    zwraca największy ze stopni z jednomianów składowych
  */
  if (PolyIsCoeff(p))
    return (p->coeff == 0 ? -1 : 0);
  
  poly_exp_t max_monos_exp = -1;
  for(size_t i = 0; i < p->size; i++)
  {
    // stopień przy zmiennej x_j + maksymalny stopnień wielomianu będącego jej współczynnikiem
    poly_exp_t i_mono_exp = p->arr[i].exp + PolyDeg(&p->arr[i].p);
    max_monos_exp = MAX(max_monos_exp, i_mono_exp);
  }
  return max_monos_exp;
}

poly_exp_t PolyDegBy(const Poly *p, size_t var_idx)
{
  if (PolyIsCoeff(p))
  {
    if (var_idx == 0)
      return PolyIsZero(p) ? -1 : 0;
    else
      return -1;
  }
  /*
    rekurencyjnie sprawdza stopnie przy zmiennej var_idx w jednomianach składowych i zwraca maximum z nich
  */
  poly_exp_t max_monos_by_exp = -1;
  if (var_idx > 0)
  {
    for(size_t i = 0; i < p->size; i++)
      max_monos_by_exp = MAX(max_monos_by_exp, PolyDegBy(&p->arr[i].p, var_idx - 1));
  }
  else
  {
    for(size_t i = 0; i < p->size; i++)
      max_monos_by_exp = MAX(max_monos_by_exp, p->arr[i].exp);
  }

  return max_monos_by_exp;
}



poly_coeff_t intiger_pow(poly_coeff_t base, poly_exp_t exp)
{
    /*
      implementacja algorytmu potęgowania w złożoności logarytmicznej względem wykładnika
    */
    poly_coeff_t result = 1;
    while(true)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

Poly PolyAt(const Poly *p, poly_coeff_t x)
{
  /*
    wartość wielomianu stałego jest stała, niezależna od punktu ewaluacji
  */
  if (PolyIsCoeff(p))
    return PolyFromCoeff(p->coeff);

  /*
    pokolei ewaluujemy jednomiany składowe p w punkcie x i sumujemy je w nowym, początkowo równym zero wielomianie 
  */
  Poly acc = PolyZero();
  for (size_t i = 0; i < p->size; i++)
  {
    poly_coeff_t value = intiger_pow(x, p->arr[i].exp);
    Poly temp1 = PolyFromCoeff(value);
    Poly temp2 = PolyMul(&temp1, &p->arr[i].p);
    PolyDestroy(&temp1);
    temp1 = PolyAdd(&acc, &temp2);
    PolyDestroy(&acc);
    acc = temp1;
    PolyDestroy(&temp2);
  } 
  return acc;
}