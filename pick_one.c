/*
  http://www.weibo.com/1915548291/yFsxehWpr
  #Google面试题# 给一个单向链表，只扫描一遍，随机选择一个节点
  
*/

#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>

typedef int T;
typedef struct Node {
  struct Node *next;
  T data;
} Node;

/*
  The elements are numbered as 1..n, 
  replace the candidate node with node i with probability 1/i.
  
  Simple proof:
  P(i): i replaces candidate; ~P(i): i does not replace candidate.
  then the probability element i is picked
  = P(i) && ~P(i+1) && ~P(i+2) ... && ~P(n)
  = 1/i * (1-1/(i+1)) * (1-1/(i+2)) ... * (1- 1/n)
  = 1/i * i/(i+1) * (i+1)/(i+2) ... * (n-1) / n
  = 1/n
*/   
Node *pick_one(Node *list)
{
  int k;
  Node *p;
  for(k = 1,p = list; list != NULL; k++,list = list->next)
    if (rand()%k == 0) // probability is 1/k
      p = list;
  return p;
}


/*test*/

/* build a list [0,1,2,3,4..,n-1] */
Node *make_list(int n)
{
  Node *p = NULL;
  Node *q;

  assert(n > 0);
  while(--n>=0) {
    assert(q = malloc(sizeof(Node)));
    q->data = n;
    q->next = p;
    p = q;
  }
  return p;
}

void free_list(Node *list)
{
  Node *p;
  while(list != NULL) {
    p = list->next;
    free(list);
    list = p;
  }
}

#define min(x,y) ((x) < (y)? (x):(y))
#define max(x,y) ((x) > (y)? (x):(y))
int is_close_enough(float a, float b)
{
  return min(a,b) / max(a,b) > 0.9;
}

/* test list of n elements, m times*/
int test(int n, int m)
{
  Node *list = make_list(n);
  int i;
  int dist[n]; // distribution of picked element

  for(i = 0; i < n; i++)
    dist[i] = 0;
  for(i = 0; i < m; i++) {
    int k = pick_one(list)->data;
    dist[k]++;
  }
  for(i = 1; i < n; i++)
    assert(is_close_enough(dist[i],(float)m/n));
  free_list(list);
}
  

int main()
{
  return test(100,1000000);
}
  
    

  

  

  
