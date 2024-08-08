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
  return;
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
  return;
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
  if (t == NULL || t->root == NULL){
    return NULL;
  }

  node_t *x = t->root;

  while (x->left != t->nil)
  {
    x = x->left;
  }

  return x;
}

node_t *tree_minimum(const rbtree *t, node_t *sub_root){
  node_t * r = sub_root;
  if (r == t->nil)
    return r;
  while (r->left != t->nil)
  {
    r = r->left;
  }
  return r;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  if (t == NULL || t->root == NULL){
    return NULL;
  }

  node_t *x = t->root;

  while (x -> right != t->nil)
  {
    x = x->right;
  }
  return x;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil) {
    t->root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent;
  return;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y = p;
  color_t y_original_color = y->color;
  node_t *x;

  if (p->left == t->nil) {
    x = p->right;
    rbtree_transplant(t, p, p->right);
  } else if (p->right == t->nil) {
    x = p->left;
    rbtree_transplant(t, p, p->left);
  } else {
    y = tree_minimum(t, p->right);
    y_original_color = y->color;
    // x가 12일 때 y의 오른쪽이면 null이 된다.
    x = y->right;

    if (y->parent == p) {
      x->parent = y;
    } else {
      rbtree_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    // 이거 다시 확인 필요
    // x->parent = y;
    // 여기서 확인하자.
    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  // fixup 전에 이미 배치가 이상하다.
  if (y_original_color == RBTREE_BLACK) {
    rbtree_delete_fixup(t, x);
  }

  free(p);
  p = NULL;
  return 0;
}

void rbtree_delete_fixup(rbtree *t, node_t *x){
  node_t *w;

  while (x != t->root && (x == t->nil || x->color == RBTREE_BLACK)) {
    if (x == x->parent->left){
      w = x->parent->right;
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->right->color == RBTREE_BLACK){
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    } else {
      w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  if (x != t->nil) {
      x->color = RBTREE_BLACK;
  }
}

void inorder_tree(const rbtree *t, node_t *x, key_t *arr, const size_t n, int *index){
  if (x != t->nil && *index < n) {
    inorder_tree(t, x->left, arr, n, index);
    if (*index < n) {
      arr[*index] = x->key;
      (*index)++;
    }
    inorder_tree(t, x->right, arr, n, index);
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  int index = 0;
  node_t *x = t->root;
  inorder_tree(t, x, arr, n, &index);
  return 0;
}
