#include "gtest/gtest.h"

#include <stdio.h>

#include "utils.h"

extern "C" {
#include "sexp.h"
#include "token_stream.h"
#include "symbol.h"
}

TEST(SExp, test_atom) {

  Token_T t;
  t.type = TOKEN_INTEGER;
  t.value.i = 12345;
  SExp_T sexp = SExp_new_int(t);
  EXPECT_TRUE(SExp_is_integer(sexp));
  EXPECT_EQ(SExp_value_integer(sexp),12345);
  SExp_free(&sexp);

  t.type = TOKEN_FLOAT;
  t.value.f = 2.71828;
  sexp = SExp_new_float(t);
  EXPECT_TRUE(SExp_is_float(sexp));
  EXPECT_EQ(SExp_value_float(sexp),t.value.f);
  SExp_free(&sexp);

  t.type = TOKEN_SYMBOL;
  t.value.sym = Symbol_new("test",-1);
  sexp = SExp_new_symbol(t);
  EXPECT_TRUE(SExp_is_symbol(sexp));
  EXPECT_EQ(SExp_value_symbol(sexp),Symbol_new("test",-1));
  SExp_free(&sexp);
  
  SExp_T y = SExp_new_list();
  EXPECT_TRUE(SExp_is_null(y));

  t.type = TOKEN_INTEGER;
  t.value.i = 12345;
  SExp_T x = SExp_new_int(t);
  SExp_T z = SExp_cons(x,y);

  EXPECT_TRUE(SExp_is_cons(z));
  EXPECT_TRUE(SExp_is_list(z));
  EXPECT_EQ(SExp_car(z),x);
  EXPECT_EQ(SExp_cdr(z),y);

  // free x and y also
  SExp_free(&z);
  x = y = NULL;
  EXPECT_EQ(NULL,z);

  x = SExp_new_list();
  int a[] = {0,1,2,3,4,5,6,7,8,9,10};
  int i;
  for(i=0; i < NELEMS(a); i++) {
    t.type = TOKEN_INTEGER;
    t.value.i = a[i];
    x = SExp_cons(SExp_new_int(t),x);
  }
  y = x;
  i = NELEMS(a)-1;
  while(!SExp_is_null(y)) {
    z = SExp_first(y);
    EXPECT_TRUE(SExp_is_integer(z));
    EXPECT_EQ(SExp_value_integer(z),a[i]);
    i--;
    y = SExp_rest(y);
  }
  EXPECT_TRUE(i == -1);

  x = SExp_reverse_list(x);
  y = x;
  i = 0;
  while(!SExp_is_null(y)) {
    z = SExp_first(y);
    EXPECT_TRUE(SExp_is_integer(z));
    EXPECT_EQ(SExp_value_integer(z),a[i]);
    i++;
    y = SExp_rest(y);
  }
  EXPECT_EQ(i,NELEMS(a));

  SExp_free(&x);
  EXPECT_EQ(NULL,x);
  y = z = NULL;
  
}
