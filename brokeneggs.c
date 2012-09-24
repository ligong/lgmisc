/* Problem: http://weibo.com/1915548291/yB3lXe4ec
   两个鸡蛋：两个软硬程度一样但未知的鸡蛋，它们有可能都在一楼就摔碎，也可能从一百层楼摔下来没事。
   有座100层的建筑，要你用这两个鸡蛋以最少的次数确定哪一层是鸡蛋可以安全落下的最高位置。可以摔碎两个鸡蛋。
*/

#include <stdio.h>
#include <limits.h>
#include <assert.h>

int min(int x, int y) {return x<y?x:y;}
int max(int x, int y) {return x<y?y:x;}

/* min_trials uses dynamic programming to solve above problem
   of m floor, n eggs.
   It returns the minimum number of
   trials to determine the highest floor number from which
   egg can be dropped safely.
   Decision result is stored in array a. 
   a[i][j] is floor number of the
   first trial when solving the i floor,j eggs problem.
*/
int min_trials(int m, int n, int a[m+1][n+1])
{
  int i,j,k,c,cost;
  int f[m+1][n+1]; // C99's dynamic array feature

  assert(m >= 1 && n >= 1);

  for(i = 1; i <= m; i++)
    f[i][1] = i;
  for(j = 1; j <= n; j++) {
    f[0][j] = 0;
    f[1][j] = 1;
  }

  for(i = 2; i <= m; i++)       // i floor
    for(j = 2; j <= n; j++) {   // j eggs
      cost = INT_MAX;           // min trial number
      for(k = 1; k <= i; k++) { // first test on kth floor,
	// if egg is broken, examine 1..k-1 floor with j-1 eggs
	// if not broken, examine k+1..n floor with j eggs
	c = 1 + max(f[k-1][j-1],f[i-k][j]);
	if (c < cost) {
	  cost = c;
	  a[i][j] = k;
	}
      }
      f[i][j] = cost;
    }
  return f[m][n];
}

void print_depth(int n)
{
  while(n-- > 0) putchar(' ');
}

/* Based on decision data stored in array a, print the decision tree
   of m floor, n eggs.
   k is first floor's number,i.e. k..m-k+1 floor.
   d is the recur depth, used to print prefix blank.
   t is the total trials done before
*/
void print_decision(int m, int n, int a[m+1][n+1], int k, int d, int t)
{
  
  print_depth(d);
  if (n == 1)
    printf("Drop egg from floor %d to %d until egg is broken. Done,maximum %d trials\n", k, m+k-1,t+m);
  else if (m == 1)
    printf("Drop egg from floor %d, Done,maximum %d trials\n",k,t+1);
  else {
    int i = a[m][n];
    printf("Drop egg from %d floor\n", k+i-1);
    print_depth(d);
    printf("If broken:\n");
    print_decision(i-1,n-1,a,k,d+2,t+1);
    print_depth(d);
    printf("If not broken:\n");
    print_decision(m-i,n,a,k+i,d+2,t+1);
  }
}

int main()
{
  int m = 100, n = 2;
  int a[m+1][n+1]; // C99' dynamic array
  printf("%d floor, %d eggs: %d trials\n", m,n,min_trials(m,n,a));
  print_decision(m,n,a,1,0,0);
  return 0;
}

