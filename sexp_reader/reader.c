#include "reader.h"

#include <assert.h>

#include "token_stream.h"
#include "sexp.h"
#include "utils.h"

static SExp_T read_list(TokenStream_T ts);

/*
  return a sexp from token steram
  if syntax is wrong, call READ_ERR
*/
SExp_T read_sexp(TokenStream_T ts)
{
  Token_T t = TokenStream_next(ts);
  SExp_T sexp;

  if (t.type == TOKEN_NONE) 
    sexp = sexp_none;
  else if (t.type == TOKEN_INTEGER) 
    sexp = SExp_new_int(t);
  else if (t.type == TOKEN_STRING) 
    sexp = SExp_new_string(t);
  else if (t.type == TOKEN_FLOAT) 
    sexp = SExp_new_float(t);
  else if (t.type == TOKEN_SYMBOL) 
    sexp = SExp_new_symbol(t);
  else if (t.type == TOKEN_LPAREN) 
    sexp = read_list(ts);
  else if (t.type == TOKEN_RPAREN) 
    sexp = sexp_end_list;
  else {
    READ_ERR("Invalid s-exp");
    sexp = sexp_none;
  }
  
  TokenStream_free_token(t);
  return sexp;
}

/*
  read and return sexp list embeded in parences (..)
  assume '(' has already been analyzed
  if syntax is wrong, call READ_ERR
*/
static SExp_T read_list(TokenStream_T ts)
{
  SExp_T sexp = SExp_new_list();
  SExp_T sub_sexp;
  
  while ((sub_sexp = read_sexp(ts)) != sexp_none &&
         sub_sexp != sexp_end_list) {
    sexp = SExp_cons(sub_sexp,sexp);
  }
  
  if (sub_sexp == sexp_none)
    READ_ERR("Unbalanced parence");

  sexp = SExp_reverse_list(sexp);
  return sexp;
}

void print_atom(SExp_T sexp)
{
  SExp_assert_valid(sexp);
  
  if (SExp_is_integer(sexp))
    printf("%d",SExp_value_integer(sexp));
  else if (SExp_is_float(sexp))
    printf("%f",SExp_value_float(sexp));
  else if (SExp_is_string(sexp))
    printf("%s",SExp_value_string(sexp));
  else if (SExp_is_symbol(sexp))
    printf("%s",Symbol_string(SExp_value_symbol(sexp)));
  else if (SExp_is_null(sexp))
    printf("NIL");
  else
    assert(0);
}

void print_rest(SExp_T sexp);
void print(SExp_T sexp)
{
  if (SExp_is_atom(sexp)) {
    print_atom(sexp);
  } else {
    printf("(");
    print(SExp_first(sexp));
    print_rest(SExp_rest(sexp));
    printf(")");
  }
}

void print_rest(SExp_T sexp)
{
  if (SExp_is_null(sexp))
    return;
  else if (SExp_is_atom(sexp)) {
    printf(" . ");
    print(sexp);
  } else {
    printf(" ");
    print(SExp_first(sexp));
    // let's wish compiler is smart enough to
    // recognize tail recursion
    print_rest(SExp_rest(sexp)); 
  }
}

