/*
  #面试题# 假如你的朋友信息用hash表来存储，姓名是key，现在想随机挑选一人。朋友表是动态增减的。
  请设计算法和编程实现。
*/


// This implementation require hash table store each bucket's number of elements

// Algorithm: Suppose the hash table has k buckets
//    with n_1,n_2,...n_k number of elements in each one.
//
//    First, we need pick the bucket from which the element will be chosen
//      Go through each bucket and pick it with 
//      probability of n_i / m_i,   m_i = n_1 + n_2 + ... n_i
//      The previous choice will be overwite.
//    After the bucket is determined, randomly pick one element from
//    its list and return as result.

// To show the algorithm works, we need to prove any element e has an equal opportulity
// to be picked
// That is, Pr(any element e is picked) = 1 / m, m is the total number of elements

// Declare Pr(bucket i is finally picked) = n_i / m
// Proof:
//   Pr(bucket i is finally picked) = Pr(bucket i is picked AND
//                                    bucket i+1 is not picked... AND
//                                    bucket k is not picked)
//                                  = n_i/m_i * (1- n_i+1/m_i+1)...*(1 - n_k/m_k)
//                                  = n_i/m_i * m_i/m_i+1 * ... * m_k-1/m_k  , m_i+1 - n_i+1 = m_i
//                                  = n_i / m_k                                     
//                                  = n_i / m                                , m_k = m
//
// It follows that Pr(any element e is picked)
//   = Pr(e's bucket is chosen and e is chosen)
//   = Pr(e's bucket is chosen) * Pr(e is chosen | e's bucket is chosen)
//   = (n_i / n) * (1 / n_i)
//   = 1/n

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define NHASH 50  // hash buckets size

typedef int Key;
typedef int Value;

typedef struct Binding Binding;
struct Binding {
  struct Binding* link;
  Key key;   
  Value value;
};

struct {
  int n;  // number of elements in this bucket
  Binding* link;
} g_hash_table[NHASH];

// hash function, map key to [0..NHASH-1]
unsigned hash(Key key)
{
  // a simple but slow implementation
  double d = (unsigned)key * 0.6180339887;       
  return (unsigned)(NHASH * (d - (int)d)) % NHASH;       
}

// add key,value to hash table, return 1 on success, 0 on failure
int hash_table_add(Key key, Value value)
{
  unsigned h = hash(key);
  Binding* binding = malloc(sizeof *binding);
  if(!binding) return 0;
  binding->key     = key;
  binding->value   = value;
  binding->link    = g_hash_table[h].link;
  g_hash_table[h].link  = binding;
  g_hash_table[h].n++;
  return 1;
}

// delete key from hash table, return 1 on success, 0 on key not exist
int hash_table_del(Key key)
{
  unsigned h = hash(key);
  Binding** pp = &g_hash_table[h].link;
  for(; *pp; pp = &(*pp)->link) {
    if ((*pp)->key == key) {
      Binding* p = *pp;
      *pp = p->link;
      assert(g_hash_table[h].n-- > 0);
      return 1;
    }
  }
  return 0;
}

// lookup hash table, update value and return 1 on success
// return 0 on key not found
int hash_table_lookup(Key key, Value* value)
{
  unsigned h = hash(key);
  Binding* p = g_hash_table[h].link;
  for(; p; p = p->link) {
    if (p->key == key) {
      *value = p->value;
      return 1;
    }
  }
  return 0;
}

// clear hash table, free resource
void hash_table_clear()
{
  Binding *p,*q;
  int i;
  for(i = 0; i < NHASH; i++) {
    for(p = g_hash_table[i].link; p; p = q) {
      q = p->link;
      free(p);
      assert(g_hash_table[i].n-- > 0);
    }
    g_hash_table[i].n = 0;
  }
}

Value pick_one()
{
  int b = -1;      // bucket candidate
  int m = 0;       // the number of elements up to now
  int i;

  // Pick bucket
  for(i = 0; i < NHASH; i++) {
    if (g_hash_table[i].n > 0) {
      int c;
      m += g_hash_table[i].n;
      c = rand() % m;
      if (c < g_hash_table[i].n)
        b = i;
    }
  }
  // Pick element
  if (b >= 0) {
    Binding *q = NULL; // candidate
    Binding *p;
    int n = 0;
    for(p = g_hash_table[b].link; p; p = p->link) {
      n++;
      if (rand() % n == 0)  // 1/n probability
        q = p;
    }
    assert(q);
    return q->value;
  } else
    return -1;
}

#define min(x,y) ((x) < (y)? (x):(y))
#define max(x,y) ((x) > (y)? (x):(y))
int is_close_enough(float a, float b)
{
  return min(a,b) / max(a,b) > 0.9;
}

// add elements[0..n-1], pick element m times
// check the probability of each element being picked
// is indeed about 1/n
int test(int n, int m)
{
  int i;
  long dist[n]; // distribution of picked element

  for(i = 0; i < n; i++) {
    dist[i] = 0;
    hash_table_add(i,i);
  }
  
  for(i = 0; i < m; i++) {
    Value k = pick_one();
    assert(k>=0);
    dist[k]++;
  }
  for(i = 0; i < n; i++) 
    assert(is_close_enough((float)dist[i]/m, 1.0/n));

  hash_table_clear();
  printf("test passed\n");
}
  

int main()
{
  test(NHASH*5, NHASH*5*1000);
  return 0;
}
  
