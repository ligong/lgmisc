#ifndef SEXP_READER_UTILS_H_
#define SEXP_READER_UTILS_H_

#define NELEMS(array) ((int)(sizeof((array)) / sizeof((array[0]))))

#define ERR(msg) ((void)(printf("%s\n",msg),assert(0),0))

#define READ_ERR(msg) ERR(msg)

#endif
