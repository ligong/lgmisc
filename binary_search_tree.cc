// #面试题#写一个二叉树中序遍历的c++ class iterator。
// http://weibo.com/1915548291/z9cVqACOB

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

// binary search tree, require keys are distinct
template<class K,class D>
class BST
{
 public:
  
  BST(): timestamp_(0), root_(NULL) { }
  ~BST() {Free();}
  
  // add key,data pair into binary tree,
  // if key already exists, replace it
  // return true on success, false on failure
  bool Add(const K& key, const D& data);

  // delete key from binary tree, if parameter data is not
  // NULL, assign it to delete data
  // return true on success, false if key does not exit
  bool Del(const K& key, D* data=NULL);

  // lookup key in binary tree, assign result to
  // parameter data if it is not NULL and key exists
  // return true if found, false if key does not exist
  bool Find(const K& key, D* data=NULL);

  // Is an empty tree?
  bool IsEmpty() { return root_ == NULL;}

  struct Node;
  class Iterator
  {
   public:
    Iterator(): timestamp_(0),p_(NULL),tree_(NULL) { }
    
    // prefix ++i
    Iterator& operator++() {
      assert(timestamp_ == tree_->timestamp_);
      p_ = tree_->Successor(p_);
      return *this;
    }
    // postfix i++
    Iterator operator++(int) {
      assert(timestamp_ == tree_->timestamp_);
      Iterator it = *this;
      ++*this;
      return it;
    }

    // prefix --i
    Iterator& operator--() {
      assert(timestamp_ == tree_->timestamp_);
      p_ = tree_->Prodecessor(p_);
      return *this;
    }

    // postfix i--
    Iterator operator--(int) {
      assert(timestamp_ == tree_->timestamp_);
      Iterator it = *this;
      ++*this;
      return it;
    }
    
    D& operator*() {
      assert(p_);
      return p_->data;
    }

    bool operator==(const Iterator &that)
    {
      return p_ == that.p_;
    }

    bool operator!=(const Iterator &that)
    {
      return !(*this == that);
    }

   private:
    unsigned long timestamp_;  // prevent tree is changed during iter
    Node* p_;
    BST* tree_;
    Iterator(unsigned long timestamp, Node* p,BST* tree):
        timestamp_(timestamp),p_(p),tree_(tree) { }
    friend class BST;
  };

  typedef Iterator iterator;

  iterator begin();

  iterator end();

 private:

  unsigned long timestamp_;
  struct Node {
    Node* parent;
    Node* left;
    Node* right;
    K key;
    D data;
    Node(): parent(NULL),left(NULL),right(NULL) { }
    Node(Node* p, Node* l, Node* r, K k, D d):
        parent(p),left(l),right(r),key(k),data(d) { }
  }* root_;

  static iterator end_it_;
  
  // return the left most child of p
  Node* Max(Node* p);

  // return the right most child of p
  Node* Min(Node* p);
  
  // return successor of p, or return NULL if no successor
  Node* Successor(Node* p);

  // return prodecessor of p, or return NULL if no predecessor
  Node* Predecessor(Node* p);

  // free memory
  void Free() { while(root_) Del(root_->data);}
  
  // disallow copy & assignment constructor
  BST(const BST&);
  void operator=(const BST&);
};


template<class K, class D>
typename BST<K,D>::iterator BST<K,D>::end_it_;

template<class K, class D>
bool BST<K,D>::Add(const K& key, const D& data)
{
  Node** pp = &root_;
  Node* parent = NULL;

  timestamp_++;
  
  while(*pp) {
    if ((*pp)->key == key) {
      (*pp)->data = data; 
      return true;
    } else if ((*pp)->key < key) {
      parent = *pp;
      pp = &(*pp)->right;
    } else {
      parent = *pp;
      pp = &(*pp)->left;
    }
  }

  *pp = new Node(parent,NULL,NULL,key,data);
  
  return *pp != NULL;
    
}

template<class K, class D>
bool BST<K,D>::Find(const K& key, D* data)
{
  Node* p = root_;
  while (p) {
    if (p->key == key) {
      if (data)
        *data = p->data;
      return true;
    } else if (p->key < key)
      p = p->right;
    else
      p = p->left;
  }
  return false;
}


template<class K, class D>
bool BST<K,D>::Del(const K& key, D* data)
{
  Node** pp = &root_;

  timestamp_++;
  
  while (*pp) {
    if ((*pp)->key == key)
      break;
    else if ((*pp)->key < key) 
      pp = &(*pp)->right;
    else
      pp = &(*pp)->left;
  }

  Node* p = *pp;
  if (p == NULL) return false;

  if (data)
    *data = p->data;

  if (p->left == NULL && p->right == NULL) {
    *pp = NULL;
    delete p;
  } else if (p->left == NULL || p->right == NULL) {
    Node* q = p->left == NULL? p->right:p->left;
    q->parent = p->parent;
    *pp = q;
    delete p;
  } else {
    Node* q = Successor(p);
    assert(q);

    // copy data to p
    p->key = q->key;
    p->data = q->data;

    // delete q
    if (q->right)
      q->right->parent = q->parent;
    if (q->parent->left == q)
      q->parent->left = q->right;
    else
      q->parent->right = q->right;

    delete q;
  }
  return true;
}

template<class K, class D>
typename BST<K,D>::Node* BST<K,D>::Successor(typename
                                             BST<K,D>::Node* p)
{
  if (p == NULL) return NULL;
  
  if (p->right)
    return Min(p->right);

  Node* q = p->parent;

  while(q && q->right == p) {
    p = q;
    q = q->parent;
  }
  return q;
}

template<class K, class D>
typename BST<K,D>::Node*  BST<K,D>::Predecessor(typename
                                                BST<K,D>::Node* p)
{
  if (p == NULL) return NULL;
  
  if (p->left)
    return Max(p->left);

  Node* q = p->parent;

  while(q && q->left == p) {
    p = q;
    q = q->parent;
  }
  return q;
}

template<class K, class D>
typename BST<K,D>::Node* BST<K,D>::Min(typename BST<K,D>::Node* p)
{
  if (p == NULL) return NULL;
  while(p->left)
    p = p->left;
  return p;
}

template<class K, class D>
typename BST<K,D>::Node* BST<K,D>::Max(BST<K,D>::Node* p)
{
  if (p == NULL) return NULL;
  while(p->right)
    p = p->right;
  return p;
}

template<class K, class D>
typename BST<K,D>::iterator BST<K,D>::begin()
{
  return BST<K,D>::iterator(timestamp_,Min(root_),this);
}

template<class K, class D>
typename BST<K,D>::iterator BST<K,D>::end()
{
  return end_it_;
}


// test

#define NELEMS(x) (int(sizeof(x)/sizeof(x[0])))

int main()
{

  int d[] = {0,1,2,3,4,5,6,7,8,9};
  std::vector<int> v(d,d+NELEMS(d));

  int k = 0;
  do {
    if(++k % 50000 == 0) {
      fprintf(stderr,".");
      k = 0;
    }
    BST<int,int> tree;
    int i;
    for(i = 0; i < (int)v.size(); i++)
      tree.Add(v[i],v[i]);
    BST<int,int>::iterator it;
    for(i = 0, it = tree.begin(); it != tree.end(); ++it,++i) {
      assert(d[i] == *it);
    }
    for(i = 0; i < NELEMS(d); i++) {
      int x;
      assert(tree.Find(d[i],&x));
      assert(x == d[i]);
      assert(tree.Del(d[i]));
    }
    assert(tree.IsEmpty());
  } while(std::next_permutation(v.begin(),v.end()));

  printf("ok\n");

  return 0;
}
