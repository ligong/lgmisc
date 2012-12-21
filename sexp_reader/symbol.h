#ifndef SEXP_READER_SYMBOL_H_
#define SEXP_READER_SYMBOL_H_

// use const char* represent symbol for
// effeciency of compare and store


// If symbol already exists, return it
// or create a new one
const char* Symbol_new(const char* s,int len);

// convert symbol to string
const char* Symbol_string(const char* sym);

#endif
