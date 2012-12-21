#include "gtest/gtest.h"

extern "C" {
#include "symbol.h"
}

TEST(Symbol, Symbol_new) {

  const char* a = Symbol_new("foo bar",-1);
  const char* b = Symbol_new("foo bar",-1);
  EXPECT_TRUE(a == b);
  EXPECT_TRUE(strcmp(Symbol_string(a),"foo bar") == 0);
  
  const char* c = Symbol_new("foo bar!",-1);
  EXPECT_TRUE(a != c);

  const char* d = Symbol_new("",-1);
  EXPECT_TRUE(strcmp(Symbol_string(d),"") == 0);
  
}


