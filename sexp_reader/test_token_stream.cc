#include "gtest/gtest.h"

#include <stdio.h>


extern "C" {
#include "token_stream.h"
#include "symbol.h"
#include "sexp.h"  
}

TEST(TokenStream, next) {

  FILE* file = fopen("test_token_stream.txt","r");

  TokenStream_T ts = TokenStream_new(file);

  Token_T t = TokenStream_next(ts);
  EXPECT_EQ(t.type, TOKEN_INTEGER);
  EXPECT_EQ(t.value.i,12345);
  TokenStream_free_token(t);

  t = TokenStream_next(ts);
  EXPECT_EQ(t.type,TOKEN_SYMBOL);
  EXPECT_EQ(t.value.sym,Symbol_new("foo",-1));
  TokenStream_free_token(t);

  t = TokenStream_next(ts);
  EXPECT_EQ(t.type,TOKEN_STRING);
  EXPECT_TRUE(strcmp(t.value.str,"test test") == 0); 
  TokenStream_free_token(t);

  t = TokenStream_next(ts);
  EXPECT_EQ(t.type,TOKEN_STRING);
  EXPECT_TRUE(strcmp(t.value.str,"") == 0);   // test empty string
  TokenStream_free_token(t);

  t = TokenStream_next(ts);
  EXPECT_EQ(t.type,TOKEN_STRING);
  EXPECT_TRUE(strcmp(t.value.str,"foo\\bar") == 0); // test escape char
  TokenStream_free_token(t);

  t = TokenStream_next(ts);
  EXPECT_EQ(t.type,TOKEN_FLOAT);
  //[bug] float parser's precision needs improvement
  EXPECT_TRUE(abs(t.value.f - 3.14159) < 0.00001);
  TokenStream_free_token(t);

  t = TokenStream_next(ts);
  EXPECT_TRUE(t.type == TOKEN_NONE);
  
  TokenStream_free(&ts);
  fclose(file);
  
}



