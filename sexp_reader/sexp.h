#ifndef SEXP_READER_SEXP_H_
#define SEXP_READER_SEXP_H_

#include "token_stream.h"

#define T SExp_T
typedef struct SExp_t* T;

/* special value used by reader */
extern T sexp_none;     /* no more sexp, similar to eof(end of file) */
extern T sexp_end_list; /* meet ')', end of list */

enum SExpType {SEXP_INTEGER,SEXP_FLOAT,
               SEXP_STRING,SEXP_SYMBOL,
               SEXP_NULL, SEXP_CONS, SEXP_INVALID};

T SExp_new_int(Token_T);
T SExp_new_string(Token_T);
T SExp_new_float(Token_T) ;
T SExp_new_symbol(Token_T);

/* make an empty sexp list */
T SExp_new_list();

/* if z = SExp_cons(x,y), x = SExp_car(z), y =SExp_cdr(z) */
T SExp_cons(T x, T list);
T SExp_car(T cons);
T SExp_cdr(T cons);

/* first element of list */
T SExp_first(T list);

/* rest elements of list */
T SExp_rest(T list);

/* return the nth element */
T SExp_nth(T list, int n);

// shortcut
#define CAR(x) (SExp_first(x))
#define CDR(x) (SExp_rest(x))
#define CADR(x) CAR(CDR(x))
#define CAADR(x) CAR(CADR(x))



/* return the reverse of list
   the original one will be destroyed
*/
T SExp_reverse_list(T list );

// free sexp resource
void SExp_free(T*);

int SExp_is_string(T);
int SExp_is_symbol(T);
int SExp_is_integer(T);
int SExp_is_float(T);
int SExp_is_atom(T);
int SExp_is_null(T);
int SExp_is_list(T);
int SExp_is_cons(T);

char* SExp_value_string(T);
int SExp_value_integer(T);
float SExp_value_float(T);
const char* SExp_value_symbol(T);


void SExp_assert_valid(T);

#undef T
#endif
