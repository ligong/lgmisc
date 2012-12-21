#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "reader.h"
#include "sexp.h"
#include "token_stream.h"

// well, an empty evaluator looks better than nothing
//typedef SExp_T Value
SExp_T evaluate(SExp_T sexp)
{
  return sexp;
}

void repl(char* name, FILE* file)
{
  TokenStream_T ts;
  SExp_T sexp;
  
  assert(file);

  ts = TokenStream_new(file);

  for(;;) {
    printf("> ");
    if ((sexp = read_sexp(ts)) == sexp_none)
      break;
    printf("value:\n");
    print(evaluate(sexp));
    printf("\n");
    SExp_free(&sexp);
  }
  TokenStream_free(&ts);
}

int main(int argc, char* argv[])
{
  int i;
  
  for(i = 1; i < argc; i++) {
    FILE* file = fopen(argv[i],"r");
    if (file == NULL) {
      printf("%s fail to open %s (%s)\n",argv[0],argv[i],strerror(errno));
      return EXIT_FAILURE;
    } else {
      repl(argv[i],file);
      fclose(file);
    }
  }

  if (argc == 1)
    repl(NULL,stdin);
  
  return EXIT_SUCCESS;
}

