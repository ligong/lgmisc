#include "token_stream.h"

#include <limits.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "utils.h"

#define T TokenStream_T

struct TokenStream_t {
  FILE* file;
};

T TokenStream_new(FILE* file)
{
  T ts;
  NEW(ts);
  ts->file = file;
  return ts;
}

// parse integer from file and return it
// WARN: call READ_ERR if number is too big to
// fit in long
static long parse_int(T ts,int c)
{
  unsigned long m;
  int sign = 1;
  
  if (c == '+')
    c = fgetc(ts->file);
  else if (c == '-') {
    c = fgetc(ts->file);
    sign = -1;
  }

  m = 0;
  for(;c != EOF && isdigit(c); c = fgetc(ts->file)) {
    int d = c - '0';
    if (((sign > 0) && (m > (LONG_MAX - d) / 10UL)) ||
        ((sign < 0) && (m > (LONG_MAX + 1UL - d) / 10UL)))
      READ_ERR("do not support big number yet");
    m = m*10 + d;
  }

  if (c != EOF)
    ungetc(c,ts->file);

  return sign > 0? m:-m;
}

// parse number from file and return it
// parameter c must contain the first digit
static Token_T next_number(T ts,int c)
{
  long m = parse_int(ts,c);

  if ((c = fgetc(ts->file)) == '.') {
    float f,g;
    c = fgetc(ts->file);
    f = (float)parse_int(ts,c);
    long power = 10;
    g = f;
    while ((g/=10) > 1)
      power *= 10;
    return (Token_T){.type=TOKEN_FLOAT,.value.f=m+f/power};
  } else if (c != EOF) {
    ungetc(c,ts->file);
  }
    
  return (Token_T){.type=TOKEN_INTEGER,.value.i=m};
}

// parse string as format "..." and return it
// assume the first '"' has already been parsed
static Token_T next_string(T ts)
{
  long nbuf = 50;
  int i = 0;
  char* buf = ALLOC(nbuf);
  int c;
  
  while((c = fgetc(ts->file)) != EOF && c != '"') {
    if (c == '\\') { // support escape char '\'
      if ((c = fgetc(ts->file)) == EOF)
        READ_ERR("'\' in string must follow a char");
    }

    if (i >= nbuf-1 ) { // ensure buffer large enough
      nbuf <<= 1;
      RESIZE(buf,nbuf);
    }

    buf[i++] = c;
  }
  
  if (c == EOF)
    READ_ERR("Unbalanced '\"' in string");
  
  buf[i++] = '\0';

  RESIZE(buf,i);
  return (Token_T){.type=TOKEN_STRING,.value.str=buf};
}

static int is_symbol(int c)
{
  return isalnum(c) || strchr("?+-*/_",c);
}

// parse symbol and return it
// the first char is in c
static Token_T next_symbol(T ts,int c)
{
  long nbuf = 50;
  char* buf = ALLOC(nbuf);
  int i = 0;


  assert(is_symbol(c));
  
  do {
    if (i >= nbuf-1 ) { // ensure buffer large enough
      nbuf <<= 1;
      RESIZE(buf,nbuf);
    }
    buf[i++] = c;
  } while(is_symbol(c = fgetc(ts->file)));
  
  if (c != EOF)
    ungetc(c,ts->file);

  const char* sym = Symbol_new(buf,i);
  FREE(buf);
  
  return (Token_T){.type=TOKEN_SYMBOL, .value.sym=sym};
}

Token_T TokenStream_next(T ts)
{
  int c;

  // skip blanks
  while(isblank(c = fgetc(ts->file)) || (c=='\r') || (c=='\n'))
    ;
  if (c == EOF)
    return (Token_T){TOKEN_NONE};
  else if (isdigit(c) || (c == '-'))
    return next_number(ts,c);
  else if (c == '\"')
    return next_string(ts);
  else if (c == '(')
    return (Token_T){TOKEN_LPAREN};
  else if (c == ')')
    return (Token_T){TOKEN_RPAREN};
  else if (c == '.')
    return (Token_T){TOKEN_DOT};
  else
    return next_symbol(ts,c);
}

void TokenStream_free_token(Token_T t)
{
  if (t.type == TOKEN_STRING)
    FREE(t.value.str);
}

void TokenStream_free(T* ts)
{
  FREE(*ts);
}
