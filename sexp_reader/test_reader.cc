#include "gtest/gtest.h"

extern "C" {
#include "reader.h"
#include "sexp.h"
#include "token_stream.h"
}

#define INT(x) SExp_value_integer(x)
#define STR(x) SExp_value_string(x)
#define FLT(x) SExp_value_float(x)
#define SYM(x) SExp_value_symbol(x)
#define SYMBOL(x) Symbol_new(x,-1)
#define NTH(x,n) SExp_nth(x,n)


TEST(Reader, read) {

  FILE* file = fopen("test_reader.txt","r");
  TokenStream_T ts = TokenStream_new(file);

  SExp_T x = read_sexp(ts);
  EXPECT_TRUE(SExp_is_cons(x));
  EXPECT_TRUE(SExp_is_integer(CAR(x)));
  EXPECT_TRUE(SExp_value_integer(CAR(x)) == 1);

  EXPECT_TRUE(SExp_is_cons(CDR(x)));
  EXPECT_TRUE(SExp_is_integer(CAADR(x)));
  EXPECT_EQ(2,SExp_value_integer(CAADR(x)));
  EXPECT_TRUE(SExp_is_integer(CADR(CADR(x))));
  EXPECT_EQ(3,SExp_value_integer(CADR(CADR(x))));

  SExp_free(&x);

  x = read_sexp(ts);

  EXPECT_EQ(SYMBOL("a"),SYM(NTH(x,1)));
  EXPECT_EQ(123,INT(NTH(x,2)));

  SExp_T y = NTH(x,3);
  EXPECT_TRUE(abs(3.14 - FLT(NTH(y,1))) < 0.000001);
  EXPECT_EQ(SYMBOL("pi"),SYM(NTH(y,2)));
  EXPECT_TRUE(SExp_is_null(NTH(y,3)));

  EXPECT_EQ(SYMBOL("d"),SYM(NTH(x,4)));
  EXPECT_EQ(SYMBOL("e"),SYM(NTH(x,5)));
  EXPECT_EQ(12,INT(NTH(x,6)));
  EXPECT_TRUE(abs(5.6 - FLT(NTH(x,7))) < 0.000001);

  

  SExp_free(&x);
}


