#ifndef ORDERLIST_INCLUDED
#define ORDERLIST_INCLUDED

#define T OrderList_T
typedef struct T *T;

typedef struct OrderListIter_T *OrderListIter_T;

/*
  Order list is a container for objects as (key,value).
  The objects are organized by key's order, from small to big
  
  except Mem_fail may be raised in OrderList_new,OrderList_put.
  It is a checked runtime error to pass NULL orderlist parameter
  to following functions.
*/

/* Create order list
   hint is the estimation of the number of data,
   or provides 0 to use default value.
   cmp is a function to compare object's key,
   returns -1 if key1 < key2, 0 if key1 = key2,
   1 if key1 > key2
*/
extern T OrderList_new(int hint,
		       int cmp(const void *key1, const void *key2));

/* free order list */
extern void OrderList_free(T *ol);

/* put object (key,val) into orderlist,
   If key exists, overwrite the value
   If overwrite occurs, return old val, or return NULL */
extern void *OrderList_put(T ol,const void *key,void *val);

/* return value if search key success or return NULL */
extern void *OrderList_get(T ol,const void *key);

/* remove the object */
extern void *OrderList_remove(T ol,const void *key);

extern int OrderList_length(T ol);

/*
   It is a checked runtime error to use OrderListIter
   if its OrderList is changed after iter is created.
   i.e. create iter, change order list, use iter is an
   invalid use case.

   It is checked run time error to pass NULL iter to
   functions of OrderListIter interface
*/


/* Find the item whose key is the smallest one
   greater than key parameter and return iter point to it.
   e.g.
   order list is (0, 8, 15, 20), OrderListIter_iter(ol,10) returns
   iter for 15, OrderListIter_iter(ol,15) returns iter for 15
   if key is NULL,return iter points to first object.
   if no object is found, return NULL;   
*/
extern OrderListIter_T OrderListIter_new(T ol, void *key);

extern void OrderListIter_free(OrderListIter_T *iter);

/* return the value of object pointed to by iter */
extern void *OrderListIter_value(OrderListIter_T iter);

/* return the key of object pointed to by iter */
extern const void *OrderListIter_key(OrderListIter_T iter);

/* update object with new value, return old value */
extern void *OrderListIter_put(OrderListIter_T iter, void *value);

/* Move iter to point to next object and return it.
   If there is no next object, do not change iter and return NULL
*/
extern OrderListIter_T OrderListIter_next(OrderListIter_T iter);

#undef T
#endif
