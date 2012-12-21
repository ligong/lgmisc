#ifndef SEXP_READER_TOKEN_STREAM_H_
#define SEXP_READER_TOKEN_STREAM_H_

#include <stdio.h>

#include "symbol.h"

enum TokenType {TOKEN_INTEGER,TOKEN_FLOAT,TOKEN_STRING,
                TOKEN_SYMBOL,TOKEN_LPAREN,TOKEN_RPAREN,
                TOKEN_DOT,TOKEN_NONE};

typedef struct 
{
  enum TokenType type;
  union {
    long i;
    float f;
    char* str;
    const char* sym;
  } value;
} Token_T;

#define T TokenStream_T
typedef struct TokenStream_t* T;

T TokenStream_new(FILE* file);

Token_T TokenStream_next(T);

void TokenStream_free(T*);

void TokenStream_free_token(Token_T t);


#undef T
#endif
