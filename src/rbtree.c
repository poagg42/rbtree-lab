#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *NIL = (node_t *)calloc(1, sizeof(node_t));
  NIL->color = RBTREE_BLACK;
  p->nil = NIL;
  p->root = NIL;
  return p;
}

void delete_postOrder(node_t *root, rbtree *t){
  if (root == t->nil)
  {
    return;
  }
  delete_postOrder(root->left, t);
  delete_postOrder(root->right, t);
  free(root);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t == NULL)
  {
    return;
  }
  delete_postOrder(t->root, t);
  free(t->nil);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *x = t->nil;
  node_t *y;

  y = t->nil;
  x = t->root;

  while (x != t->nil) {
    y = x;
    if (key < x->key) {
      x = x->left;
    }
    else {
      x = x->right;
    }
  }
  
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  if (new_node == NULL) {
    return NULL;
  }
  new_node->parent = y;

  if (y == t->nil){
    t->root = new_node;
    new_node->color = RBTREE_BLACK;
  }

  else if (key < y->key) {
    y->left = new_node;
    new_node->color = RBTREE_RED;
  }
  
  else {
    y->right = new_node;
    new_node->color = RBTREE_RED;
  }

  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->key = key;

  rbtree_insert_fixup(t, new_node);
  return t->root;
}

void rbtree_insert_fixup(rbtree *t, node_t *z){
  node_t *uncle;
  while (z->parent->color == RBTREE_RED) {
    if (z->parent == z->parent->parent->left){
      uncle = z->parent->parent->right;

      if (uncle->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }

      else {
        if (z == z->parent->right){
          z = z->parent;
          left_rotate(t, z);
        }

        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }

    else
    {
      uncle = z->parent->parent->left;

      if (uncle->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else {
        if (z == z->parent->left){
          z = z->parent;
          right_rotate(t, z);
        }

        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

void left_rotate(rbtree *t, node_t *x){
  node_t *y;
  y = x->right;
  x->right = y->left;
  if (y->left != t->nil){
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil){
    t->root = y;
  }

  else if (x == x->parent->left){
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x){
  node_t *y;
  y = x->left;
  x->left = y->right;
  if (y->right != t->nil){
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil){
    t->root = y;
  }
  else if (x == x->parent->right){
    x->parent->right = y;
  }
  else {
    x->parent->left = y;
  }
  
  y->right = x;
  x->parent = y;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *r = t->root;
  while (r != t->nil){
    if (r->key == key)
      return r;
    else if(r->key < key)
      r = r->right;
    else
      r = r->left;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
