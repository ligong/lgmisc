/*
  #面试题# 左“{”，右”}"括号各N个，请打印出所有正确的组合，比如当N=3，{}{}{}，{{{}}}，
  等为正确的组合。如果写的代码是recursive，能否用iterative再写一个；反之亦然。
  http://www.weibo.com/1915548291/z5J3dsL8M
*/

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

// unambigious BKNF definition of balanced parence P:
// P ::= empty
// P ::= {P}P

// generate balanced parence
void gen_balanced_paren(int n,vector<string>* result)
{
  if (n == 0) {
    result->push_back(string(""));
    return;
  }
  
  // generate {left-subexp}right-subexp
  for(int nleft = 0; nleft <= n-1 ; nleft++) {
    int nright = n-1-nleft;
    vector<string> left,right;
    gen_balanced_paren(nleft,&left);
    gen_balanced_paren(nright,&right);
    vector<string>::const_iterator lit,rit;
    for(lit = left.begin(); lit != left.end(); ++lit)
      for(rit = right.begin(); rit != right.end(); ++rit) 
        result->push_back("{" + *lit + "}" + *rit);
  }
}

int main()
{
  vector<string> result;
  gen_balanced_paren(4,&result);
  vector<string>::const_iterator it;
  for(it = result.begin(); it != result.end(); ++it)
    cout << *it << endl;
  return 0;
}
