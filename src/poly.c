#include "poly.h"
#include <stdlib.h>


Poly PolyAdd(const Poly *p, const Poly *q)
{

  // współczynnik liczbą całkowitą w obydwu, wtedy zwracamy wielomian z tym samym wykładnikiem i współczynnikiem będącym sumą tych dwóch
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
      if (p->arr[0].exp == 0)
      {
        if(p->arr[0].p.arr != NULL)
        {
          Poly temp = PolyAdd(&p->arr[0].p, q);
          if (PolyIsZero(&temp))
          {
            new_monos_buffer_size = p->size - 1;
            new_monos_buffer = (Mono *)malloc(sizeof(Mono) * new_monos_buffer_size);
          }
          else
          {
            new_monos_buffer_size = p->size;
            new_monos_buffer = (Mono *)malloc(sizeof(Mono) * new_monos_buffer_size);
            new_monos_buffer[0].exp = 0;
            new_monos_buffer[0].p = temp;
            zero_index_used = 1;
          }
        }
        else
        {
          poly_coeff_t c = p->arr[0].p.coeff + q->coeff;
          if (c == 0)
          {
            new_monos_buffer_size = p->size - 1;
            new_monos_buffer = (Mono *)malloc(sizeof(Mono) * new_monos_buffer_size);
          }
          else
          {
            new_monos_buffer_size = p->size;
            new_monos_buffer = (Mono *)malloc(sizeof(Mono) * new_monos_buffer_size);
            new_monos_buffer[0].exp = 0;
            new_monos_buffer[0].p.coeff = c;
            new_monos_buffer[0].p.arr = NULL;
            zero_index_used = 1;
          }
        }
        first_p_mono_used = 1;
      }
      else
      {
        new_monos_buffer_size = p->size + 1;
        new_monos_buffer = (Mono *)malloc(sizeof(Mono) * new_monos_buffer_size);
        new_monos_buffer[0].exp = 0;
        new_monos_buffer[0].p.coeff = q->coeff;
        new_monos_buffer[0].p.arr = NULL;
        zero_index_used = 1;

      }
      for(size_t i = 0; i < p->size - first_p_mono_used; i++)
        new_monos_buffer[zero_index_used + i] = p->arr[i + first_p_mono_used];
      return (Poly) {.size = new_monos_buffer_size, .arr = new_monos_buffer};

    }
    else
      return PolyAdd(q, p);
  }

  size_t p_size = p->size, p_iter = 0;
  size_t q_size = q->size, q_iter = 0;
  size_t new_monos_added_count = 0;
  Mono *new_monos_buffer = (Mono *)malloc(sizeof(Mono) * (p_size + q_size));

  while(p_iter < p_size || q_iter < q_size) {
    // czy koniec jedej z tablic
    if (p_iter == p_size)
    {
      new_monos_buffer[new_monos_added_count++] = q->arr[q_iter++];
      continue;
    }
    if (q_iter == q_size)
    {
      new_monos_buffer[new_monos_added_count++] = p->arr[p_iter++];
      continue;
    }
    // ----
    // stopień
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
        new_monos_buffer[new_monos_added_count++] = p->arr[p_iter++];
      else
        new_monos_buffer[new_monos_added_count++] = q->arr[q_iter++];
    }
  }

  if (new_monos_added_count == 0)
  {
    free(new_monos_buffer);
    return PolyZero();
  }
  if(new_monos_added_count == 1 && new_monos_buffer[0].exp == 0)
  {
    poly_coeff_t c = new_monos_buffer[0].p.coeff;
    free(new_monos_buffer);
    return PolyFromCoeff(c);
  }

  // zmniejszamy rozmiar tablicy jeśli nie potrzeba aż tyle miejsca
  if (new_monos_added_count < p_size + q_size)
    new_monos_buffer = (Mono *)realloc(new_monos_buffer, (sizeof(Mono) * new_monos_added_count));

  return (Poly) {.size = new_monos_added_count, .arr = new_monos_buffer};

}

int compareMonosByExp(const Mono* lhs, const Mono* rhs)
{
    if (lhs->exp < rhs->exp)
        return -1;
    if (lhs->exp > rhs->exp)
        return 1;
    return 0;
}

Poly PolyAddMonos(size_t count, Mono monos[])
{
    qsort(monos, count, sizeof(Mono), compareMonosByExp);
    Mono* new_monos_buffer = (Mono*)malloc(count * sizeof(Mono));
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
            MonoDestroy(&monos[i]);
            MonoDestroy(&new_monos_buffer[new_monos_added_count - 1]);
            if (PolyIsZero(&newMaybeZeroCoeff))
                new_monos_added_count--;
            else
                new_monos_buffer[new_monos_added_count - 1] = MonoFromPoly(&newMaybeZeroCoeff, current_exp);
        }

    }

    if (new_monos_added_count < count)
        new_monos_buffer = (Mono *)realloc(new_monos_buffer, (sizeof(Mono) * new_monos_added_count));

    if (new_monos_added_count == 1 && new_monos_buffer[0].exp == 0)
    {
      Poly result = new_monos_buffer[0].p;
      free(new_monos_buffer);
      return result;
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
  if (!p->arr && !q->arr)
    return (p->coeff == q->coeff);
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
  if (!p->arr)
    return PolyFromCoeff(p->coeff);
  
  size_t p_size = p->size;
  Mono* copied_monos = (Mono *)malloc(p_size * sizeof(Mono));
  for(size_t i = 0; i < p_size; i++)
    copied_monos[i] = MonoClone(&p->arr[i]);

  return (Poly) {.size = p_size, .arr = copied_monos};
}

