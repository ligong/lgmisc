#include "orderlist.h"
#include "mem.h"
#include "assert.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Skip list implmentation of order list interface */

#define T OrderList_T

/* probability of generating new level */
#define P 0.25

#define min(x,y) ((x) < (y) ? (x):(y))

/*
  head is array head[0..maxlevel-1];
  update[0..maxlevel-1] is temp array which holds 
  pointer to pointer of node and is used in put,remove.
  This implementation is NOT thread safe.
*/
struct OrderList_T {
  int maxlevel;       
  int level;
  int length;
  unsigned int timestamp;
  int (*cmp)(const void *key1, const void *key2);
  struct node {
    const void *key;
    void *value;
    int level;
    struct node **links;
  } ***update,**head;
};

struct OrderListIter_T {
  unsigned int timestamp;
  T ol;
  struct node *p;
};
  
int OrderList_length(T ol)
{
  assert(ol);
  return ol->length;
}

static int atomcmp(const void *k1, const void *k2)
{
  const char *s1 = k1;
  const char *s2 = k2;
  return strcmp(s1,s2);
}

static int rand_int(int n)
{
  int limit = RAND_MAX - RAND_MAX % n;
  int k;

  do {
    k = rand();
  } while (k >= limit);

  return k % n;
}

static int choose_level(int maxlevel)
{
  static int p = P * 100;
    
  int level = 1;

  while(rand_int(100) < p && level < maxlevel)
    level++;

  return level;
}

T OrderList_new(int hint, int cmp(const void *key1,const void *key2))
{
  T ol;
  int maxlevel;
  struct node **p;
  int n;

  assert(hint >= 0);

  if (hint == 0)
    maxlevel = 16;
  else
    maxlevel =  (int)(log(hint) / log(1/P)) + 1;
  
  ol = ALLOC(sizeof(*ol) + maxlevel * sizeof(*ol->head) \
	     + maxlevel * sizeof(*ol->update));
  ol->maxlevel = maxlevel;
  ol->level = 1;
  ol->length = 0;
  if (cmp == NULL)
    ol->cmp = atomcmp;
  else
    ol->cmp = cmp;
  ol->head = (struct node **)(ol+1);
  for(p = ol->head, n = ol->maxlevel; n-- > 0; )
    *p++ = NULL;
  ol->update = (struct node ***)(ol->head + maxlevel);
  ol->timestamp = 0;

  return ol;
}

void OrderList_free(T *ol)
{
  struct node *p,*q;

  assert(ol && *ol);
  for(p = (*ol)->head[0]; p != NULL; p = q) {
    q = p->links[0];
    FREE(p);
  }
  FREE(*ol);
}

void *OrderList_get(T ol, const void *key)
{

  struct node **pp;
  int i;
  
  assert(ol);

  /* pp's node's key < key <= pp[i+1]->key */
  pp = ol->head;
  for (i=ol->level-1; i>=0; i--)
    for(; pp[i] && ol->cmp(pp[i]->key,key) < 0; pp = pp[i]->links)
      ;

  if (pp[0] && ol->cmp(key,pp[0]->key) == 0)
    return pp[0]->value;
  else
    return NULL;
}

void *OrderList_put(T ol, const void *key, void *value)
{

  struct node **pp;
  struct node *p;
  assert(ol);
  int i;

  pp = ol->head;
  for(i = ol->level-1; i>=0; i--) {
    for(; pp[i] && ol->cmp(pp[i]->key,key) < 0; pp = pp[i]->links)
      ;
    ol->update[i] = &pp[i];
  }

  ol->timestamp++;
      
  if (pp[0] && ol->cmp(key,pp[0]->key) == 0) {
    void *prev;
    prev = pp[0]->value;
    pp[0]->value = value;
    return prev;
  } else {
    int level = choose_level(ol->maxlevel);
    p = ALLOC(sizeof(*p) + level * sizeof(*p->links));
    p->key = key;
    p->value = value;
    p->level = level;
    p->links = (struct node **)(p + 1);

    for(i = 0; i < min(level,ol->level); i++) {
      p->links[i] = *ol->update[i];
      *ol->update[i] = p;
    }
	
    if (level > ol->level) {
      for(i = ol->level; i < level; i++) {
	ol->head[i] = p;
	p->links[i] = NULL;
      }
      ol->level = level;
    }

    ol->length++;
    return NULL;
  }
}
    
void *OrderList_remove(T ol, const void *key)
{

   struct node **pp;
   int i;
   
   assert(ol);

   pp = ol->head;
   for(i = ol->level-1; i>=0; i--) {
     for(; pp[i] && ol->cmp(pp[i]->key,key) < 0; pp = pp[i]->links)
       ;
     ol->update[i] = &pp[i];
   }

   if (pp[0] && ol->cmp(pp[0]->key,key) == 0) {
     struct node *p = pp[0];
     void *prev = p->value;
     for(i = 0; i < ol->level; i++) {
       if (*ol->update[i] && *ol->update[i] == p)
	 *ol->update[i] = p->links[i];
     }
     FREE(p);
     for(i = ol->level-1; i> 0 && ol->head[i] == NULL; i--)
       ;
     ol->level = i+1;
     ol->length--;
     ol->timestamp++;
     return prev;
   } else
     return NULL;
}
       
void OrderList_sanity_check(T ol)
{
  int i;
  int n;
  struct node *p;
  
  assert(ol->level <= ol->maxlevel);
  
  for(i = 0; i < ol->level; i++)
    assert(ol->head[i] || (i == 0 && ol->length == 0));

  if (ol->length == 0) 
    assert(ol->level == 1 && ol->head[0] == NULL);

  for(n = 0,p = ol->head[0]; p; n++,p = p->links[0]) {
    assert(p->links[0] == NULL || \
	   ol->cmp(p->key,p->links[0]->key) <= 0);
  }
  assert(n == ol->length);

  for(i = 0; i < ol->level; i++) {
    for(n = 0,p = ol->head[i]; p; n++,p = p->links[i])
      ;
    assert(n <= ol->length);
    if (ol->length > 1000) {
      int m = pow(P,i) * ol->length; /*expected length at level i*/
      if ((float)abs(m - n) / ol->length > 0.1) {
	printf("Warn: level %d length: %d, expected: %d\n", i, n, m);
      }
    }
  }
}

OrderListIter_T OrderListIter_new(T ol, void *key)
{
  struct node **pp,*p=NULL;
  OrderListIter_T iter;
  int i;
  
  assert(ol);

  if (key == NULL && ol->length > 0)
    p = ol->head[0];
  else if (key != NULL) {
    /* pp's node's key < key <= pp[i+1]->key */
    pp = ol->head;
    for (i=ol->level-1; i>=0; i--)
      for(; pp[i] && ol->cmp(pp[i]->key,key) < 0; pp = pp[i]->links)
	;
    p = pp[0];
  }

  NEW(iter);
  iter->timestamp = ol->timestamp;
  iter->p = p;
  iter->ol = ol;
  return iter;
 
}

void OrderListIter_free(OrderListIter_T *iter)
{
  assert(iter && *iter);
  FREE(*iter);
}

int OrderListIter_isend(OrderListIter_T iter)
{
  assert(iter);
  assert((uintptr_t)iter % sizeof (void*) == 0);
  assert(iter->timestamp == iter->ol->timestamp);

  return iter->p == NULL;
}

void *OrderListIter_value(OrderListIter_T iter)
{
  assert(iter);
  assert((uintptr_t)iter % sizeof(void*) == 0);
  assert(iter->timestamp == iter->ol->timestamp);
  assert(iter->p);

  return iter->p->value;
}

const void *OrderListIter_key(OrderListIter_T iter)
{
  assert(iter);
  assert((uintptr_t)iter % sizeof (void*) == 0);
  assert(iter->timestamp == iter->ol->timestamp);
  assert(iter->p);

  return iter->p->key;
}

void *OrderListIter_put(OrderListIter_T iter,void *value)
{
  assert(iter);
  assert((uintptr_t)iter % (sizeof (void*)) == 0);
  assert(iter->timestamp == iter->ol->timestamp);
  assert(iter->p);

  return iter->p->value = value;
}

OrderListIter_T OrderListIter_next(OrderListIter_T iter)
{
  assert(iter);
  assert((uintptr_t)iter % (sizeof (void*)) == 0);
  assert(iter->timestamp == iter->ol->timestamp);
  assert(iter->p);

  iter->p = iter->p->links[0];
  return iter;
}


