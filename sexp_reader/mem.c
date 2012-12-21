#include "mem.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "utils.h"


void* Mem_alloc(long nbytes, const char *file, int line)
{
  void* ptr;
  assert(nbytes > 0);
  ptr = malloc(nbytes);
  if (ptr == NULL) {
    printf("%s,%d:",file,line);
    ERR("out of memory");
  }
  return ptr;
}
  
void* Mem_calloc(long count, long nbytes, const char* file, int line)
{
  void* ptr;
  assert(count > 0);
  assert(nbytes > 0);
  ptr = calloc(count, nbytes);
  if (ptr == NULL) {
    printf("%s,%d:",file,line);
    ERR("out of memory");
  }
  return ptr;
}
    
void* Mem_resize(void* ptr, long nbytes, const char* file, int line)
{
  assert(ptr);
  assert(nbytes > 0);
  ptr = realloc(ptr, nbytes);
  if (ptr == NULL) {
    printf("%s,%d:",file,line);
    ERR("out of memory");
  }
  return ptr;
}

void  Mem_free(void* ptr, const char* file, int line)
{
  if (ptr)
    free(ptr);
}
