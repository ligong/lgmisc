/*
  http://weibo.com/1915548291/yDjra5ajO#1348465113389
  一个大小为n的数组，里面的数都属于范围[0, n-1]，有不确定的重复元素，
  找到至少一个重复元素，要求O(1)空间和O(n)时间。
 */

#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define NELEMS(a) (sizeof(a)/sizeof(a[0]))
void swap(int *x, int *y)
{
  int tmp = *x; *x = *y; *y = tmp;
}

/*
  Term: "right place": a[i] = i, "wrong place": a[i] != i
  the while loop executes at most n times, because each execution
  puts an element from the "wrong place" into the "right place",
  and there are at most n elements at "wrong place".
  So, total execution time is for(n) + while(n) = O(n)
*/
int find_duplicate(int a[], int n)
{
  int i;
  for(i = 0; i < n; i++) {
    /* a[0]..a[i-1] are in right place */
    while(a[i] != i) {
      if (a[i] == a[a[i]])
	return a[i];
      else
	swap(&a[i],&a[a[i]]);
    }
  }
  /* a[0]..a[n-1] are in right place
     no duplicate one */
  return -1;
}

void shuffle(int a[],int n)
{
  while(--n > 0) {
    int k = rand() % (n+1);
    swap(&a[k],&a[n]);
  }
}

int main()
{
  /* simple test */
  int i;
  srand(clock());
  for(i = 0; i < 10000000; i++) {
    int a[] = {0,1,2,3,4,5,6,7,8,8};
    shuffle(a,NELEMS(a));
    assert(8 == find_duplicate(a,NELEMS(a)));
  }
  return 0;
}
