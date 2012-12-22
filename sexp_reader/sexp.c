#include "sexp.h"
#include <assert.h>
#include <string.h>

#include "mem.h"
#include "utils.h"

#define T SExp_T

struct SExpCons {
  T car;
  T cdr;
};

union SExpValue {
  long i;
  float f;
  char* str;
  const char* sym;
  struct  SExpCons* cons;
};

struct SExp_t {
  enum SExpType type;
  union SExpValue value;
};

static struct SExp_t g_sexp_none = {.type=SEXP_INVALID};
T sexp_none = &g_sexp_none;

static struct SExp_t g_sexp_end_list= {SEXP_INVALID};
T sexp_end_list = &g_sexp_end_list;

T SExp_new_int(Token_T t)
{
  T sexp;
  assert(t.type == TOKEN_INTEGER);
  NEW(sexp);
  sexp->type = SEXP_INTEGER;
  sexp->value.i = t.value.i;
  return sexp;
}

T SExp_new_string(Token_T t)
{
  T sexp;
  assert(t.type == TOKEN_STRING);
  assert(t.value.str != NULL);
  NEW(sexp);
  sexp->type = SEXP_STRING;
  if ((sexp->value.str = strdup(t.value.str)) == NULL)
    READ_ERR("out of memory");
  return sexp;
}
      
T SExp_new_float(Token_T  t)
{
  T sexp;
  assert(t.type == TOKEN_FLOAT);
  NEW(sexp);
  sexp->type = SEXP_FLOAT;
  sexp->value.f = t.value.f;
  return sexp;
}

T SExp_new_symbol(Token_T  t)
{
  T sexp;
  assert(t.type == TOKEN_SYMBOL);
  NEW(sexp);
  sexp->type = SEXP_SYMBOL;
  sexp->value.sym = t.value.sym;
  return sexp;
}

/* make an empty sexp list */
T SExp_new_list()
{
  T sexp;
  NEW(sexp);
  sexp->type = SEXP_NULL;
  return sexp;
}

/* return (cons x y) */
T SExp_cons(T x, T y)
{
  T sexp;
  struct SExpCons* cons;
  
  NEW(cons);
  cons->car = x;
  cons->cdr = y;
  
  NEW(sexp);  
  sexp->type = SEXP_CONS;
  sexp->value.cons = cons;

  return sexp;
}

T SExp_car(T sexp)
{
  assert(SExp_is_cons(sexp));
  return sexp->value.cons->car;
}


T SExp_cdr(T sexp)
{
  assert(SExp_is_cons(sexp));
  return sexp->value.cons->cdr;
}

/* first element of list */
T SExp_first(T list)
{
  assert(SExp_is_list(list));

  if (SExp_is_null(list))
    return list;
  else
    return SExp_car(list);
}

/* rest elements of list */
T SExp_rest(T list)
{
  assert(SExp_is_list(list));

  if (SExp_is_null(list))
    return list;
  else
    return SExp_cdr(list);
}

/* return the reverse of list
   the original one will be destroyed
*/
T SExp_reverse_list (T list)
{
  T new_list;
  T new_list_tail;
  
  assert(SExp_is_list(list));

  if (SExp_is_null(list))
    return list;

  new_list = new_list_tail = list;
  list = SExp_rest(list);

  while (!SExp_is_null(list)) {
    T tmp = list;
    list = SExp_rest(list);
    assert(SExp_is_list(list));
    tmp->value.cons->cdr = new_list;
    new_list = tmp;
  }

  new_list_tail->value.cons->cdr = list;

  return new_list;
}


void SExp_free(T* sexp)
{
  if (SExp_is_string(*sexp)) {
    FREE((*sexp)->value.str);
  } else if (SExp_is_cons(*sexp)) {
    SExp_free(&((*sexp)->value.cons->car));
    SExp_free(&((*sexp)->value.cons->cdr));
  }
  FREE(*sexp);
}

int SExp_is_string(T sexp)
{
  return sexp->type == SEXP_STRING;
}

int SExp_is_symbol(T sexp)
{
  return sexp->type == SEXP_SYMBOL;
}

int SExp_is_number(T sexp)
{
  return SExp_is_integer(sexp) || SExp_is_float(sexp);
}

int SExp_is_integer(T sexp)
{
  return sexp->type == SEXP_INTEGER;
}
int SExp_is_float(T sexp)
{
  return sexp->type == SEXP_FLOAT;
}

int SExp_is_atom(T sexp)
{
  return (SExp_is_string(sexp) || SExp_is_symbol(sexp) ||
          SExp_is_number(sexp) || SExp_is_null(sexp));
}

int SExp_is_null(T sexp)
{
  return sexp->type == SEXP_NULL;
}

int SExp_is_cons(T sexp)
{
  return sexp->type == SEXP_CONS;
}

int SExp_is_list(T sexp)
{
  return SExp_is_null(sexp) || SExp_is_cons(sexp);
}

void SExp_assert_valid(T sexp)
{
  assert(sexp && sexp->type >=0 && sexp->type < SEXP_INVALID);
}


char* SExp_value_string(T sexp)
{
  assert(sexp && sexp->type == SEXP_STRING);
  return sexp->value.str;
}

int SExp_value_integer(T sexp)
{
  assert(sexp && sexp->type == SEXP_INTEGER);
  return sexp->value.i;
}

float SExp_value_float(T sexp)
{
  assert(sexp && sexp->type == SEXP_FLOAT);
  return sexp->value.f;
}

const char* SExp_value_symbol(T sexp)
{
  assert(sexp && sexp->type == SEXP_SYMBOL);
  return sexp->value.sym;
}

T SExp_nth(T list, int n)
{
  while(--n > 0)
    list = SExp_rest(list);
  return SExp_first(list);
}
