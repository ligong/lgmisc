#include "orderlist.h"
#include "stdlib.h"
#include "stdio.h"
#include "assert.h"

extern void OrderList_sanity_check(OrderList_T ol);

#define NELEM(x) (sizeof(x) / sizeof(x[0]))

/* Generate a random number between 0 and n-1(inclusive)
   with equal probability */
int rand_int(int n)
{
  int limit = RAND_MAX - RAND_MAX % n;
  int k;

  do {
    k = rand();
  } while (k >= limit);

  return k % n;
}

/* Fisher–Yates shuffle */
void shuffle(int array[], int n)
{
  int i,j,tmp;

  for(i = n - 1; i > 0; i--) {
    j = rand_int(i + 1);
    tmp = array[j];
    array[j] = array[i];
    array[i] = tmp;
  }

}

int intcmp(const void *key1, const void *key2)
{
  int k1 = *(int*)key1;
  int k2 = *(int*)key2;

  if (k1 == k2)
    return 0;
  else if (k1 < k2)
    return -1;
  else
    return 1;
}

void test1()
{
  int a[] = {0,1,2,3,4,5,6,7,8,9};
  int i;
  OrderList_T ol;

  ol = OrderList_new(0,intcmp);
  
  for(i = 0; i < NELEM(a); i++)
    OrderList_put(ol,a+i,a+i);
  
  OrderList_sanity_check(ol);
  
  for(i = 0; i < NELEM(a); i++)
    assert(a+i == OrderList_get(ol,a+i));
  
  OrderList_remove(ol,a+2);
  assert(OrderList_length(ol) == NELEM(a)-1);
  assert(OrderList_get(ol,a+2) == NULL);
  assert(OrderList_get(ol,a) == a);

  OrderList_sanity_check(ol);
  
  OrderList_free(&ol);

  printf("Pass... test1\n");

}

void test2()
{
  int n = 50000;
  int *array = malloc(n * sizeof *array);
  OrderList_T ol;
  int i;
  
  ol = OrderList_new(n,intcmp);
  OrderList_sanity_check(ol);
  
  for(i = 0; i < n; i++)
    array[i] = i;

  shuffle(array,n);

  for(i = 0; i < n; i++)
    OrderList_put(ol,array+i,array+i);

  OrderList_sanity_check(ol);

  OrderList_put(ol,array+5,array+6);
  assert(array[6] == *(int*)OrderList_get(ol,array+5));
  assert(n == OrderList_length(ol));
  OrderList_sanity_check(ol);
  OrderList_put(ol,array+5,array+5);
  
  for(i = 0; i < n; i++) {
    assert(array[i] == *(int*)OrderList_get(ol,array+i));
    assert(array[i] == *(int*)OrderList_remove(ol,array+i));
  }

  OrderList_sanity_check(ol);
    
  for(i = 0; i < n; i++) 
    assert(NULL == OrderList_get(ol,array+i));

  printf("Pass... test2\n");
}

void test3()
{
  int a[] = {0,1,2,3,4,5,6,7,8,9};
  OrderList_T ol;
  OrderListIter_T it;
  int i,j = 6;

  
  ol = OrderList_new(0,intcmp);
  shuffle(a,NELEM(a));

  for(i = 0; i < NELEM(a); i++)
    OrderList_put(ol,a+i,a+i);

  for(i = 0, it = OrderListIter_new(ol,NULL); OrderListIter_isend(it); \
      i++,it = OrderListIter_next(it))
    assert(*(int*)OrderListIter_value(it) == i);

  OrderListIter_free(&it);

  for(i = 6, it = OrderListIter_new(ol,&j); OrderListIter_isend(it); \
      i++,it = OrderListIter_next(it))
    assert(*(int*)OrderListIter_value(it) == i);

  printf("Pass... test3\n");
}
    
  
  
int main()
{
  test1();
  test2();
  test3();
  return EXIT_SUCCESS;
}
	   
