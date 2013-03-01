#ifndef MEM_INCLUDED
#define MEM_INCLUDED

#include "stdlib.h"
#include "except.h"

extern const Except_T Mem_Failed;

extern void *Mem_alloc(long nbytes,const char *file, int line);
extern void *Mem_calloc(int count,long nbytes, const char *file, int line);
extern void *Mem_resize(void *ptr,long nbytes,const char *file, int line);
extern void Mem_free(void *ptr,const char *file, int line);

#define ALLOC(nbytes) Mem_alloc((nbytes),__FILE__,__LINE__)
#define CALLOC(count,nbytes) Mem_alloc((count)*(nbytes),__FILE__,__LINE__)

#define FREE(p) ((void)(Mem_free((p),__FILE__,__LINE__),(p) = 0))
#define NEW(p) ((p) = ALLOC(sizeof *(p)))
#define NEW0(p) ((p) = CALLOC(1,sizeof *(p)))
#define RESIZE(ptr,nbytes) ((ptr) = Mem_resize((ptr),(nbytes),__FILE__,__LINE__))


#endif
