#pragma once

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Node
struct avltree {
  int key;
  char* value;
  int is_deleted;
  int height;
  struct avltree *left;
  struct avltree *right;
};
struct avltree *avltree_add(struct avltree *root, int key, char* value);
struct avltree *avltree_lookup(struct avltree *tree, int key);
void printTree(struct avltree *root, int times, int last);
struct avltree *avltree_delete(struct avltree *root, int key);
struct avltree *avltree_create(int key, char* value);
struct avltree *avltree_min(struct avltree *root);
struct avltree *avltree_max(struct avltree *root);
struct avltree *rightRotate(struct avltree *y);
struct avltree *leftRotate(struct avltree *x);
void printPreOrder(struct avltree *root);
void avltree_free(struct avltree *tree);
int getBalance(struct avltree *N);
int height(struct avltree *N);
int max(int a, int b);

// Calculate height
int height(struct avltree *N) {
  if (N == NULL)
    return -1;
  return N->height;
}

int max(int a, int b) {
  return (a > b) ? a : b;
}

// Create a node
struct avltree *avltree_create(int key, char* value) {
  struct avltree *node = (struct avltree *)
    malloc(sizeof(struct avltree));
  node->key = key;
  node->value = value;
  node->left = NULL;
  node->right = NULL;
  node->height = 0;
  node->is_deleted = 0;
  return (node);
}

// Right rotate
struct avltree *rightRotate(struct avltree *y) {
  struct avltree *x = y->left;
  struct avltree *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->height = max(height(y->left), height(y->right)) + 1;
  x->height = max(height(x->left), height(x->right)) + 1;

  return x;
}

// Left rotate
struct avltree *leftRotate(struct avltree *x) {
  struct avltree *y = x->right;
  struct avltree *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(height(x->left), height(x->right)) + 1;
  y->height = max(height(y->left), height(y->right)) + 1;

  return y;
}

// Get the balance factor
int getBalance(struct avltree *N) {
  if (N == NULL)
    return 0;
  return height(N->left) - height(N->right);
}

struct avltree *avltree_min(struct avltree *root){
  if(!root) return NULL;
  struct avltree * min = avltree_min(root->left);
  if (min && !min->is_deleted)
    return min;

  if (!root->is_deleted)
    return root;

  return avltree_min(root->right);
 }

struct avltree *avltree_max(struct avltree *root){
  if(!root) return NULL;
  struct avltree * max = avltree_max(root->right);
  if (max && !max->is_deleted)
    return max;

  if (!root->is_deleted)
    return root;

  return avltree_min(root->left);
}

// Insert node
struct avltree *avltree_add(struct avltree *root, int key, char* value) {
  // Find the correct position to avltree_add the node and avltree_add it
  if (root == NULL)
    return (avltree_create(key, value));

  if (key < root->key)
    root->left = avltree_add(root->left, key, value);
  else if (key > root->key)
    root->right = avltree_add(root->right, key, value);
  else{
    root->is_deleted = 0;
    root->value = value;
    return root;
  }

  // Update the balance factor of each node and
  // Balance the tree
  root->height = 1 + max(height(root->left),
               height(root->right));

  int balance = getBalance(root);
  if (balance > 1 && key < root->left->key)
    return rightRotate(root);

  if (balance < -1 && key > root->right->key)
    return leftRotate(root);

  if (balance > 1 && key > root->left->key) {
    root->left = leftRotate(root->left);
    return rightRotate(root);
  }

  if (balance < -1 && key < root->right->key) {
    root->right = rightRotate(root->right);
    return leftRotate(root);
  }

  return root;
}

// avltree_delete ленивое удаление
struct avltree *avltree_delete(struct avltree *root, int key) {
  // Найти узел и пометить его как удаленный
  struct avltree *node = avltree_lookup(root, key);
  if (node != NULL) {
    node->is_deleted = 1; // Пометить узел как удаленный
  }
  return root;
}

// Print the tree
void printPreOrder(struct avltree *root) {
  if (root != NULL) {
    printf("%d ", root->key);
    printPreOrder(root->left);
    printPreOrder(root->right);
  }
}
void printTree(struct avltree *root, int times, int last) {
  if (root != NULL) {
    for(int i = 0; i < times; i++){
            printf("   ");

    }
    if (last) {
      printf("R----");
      times++;
    } else {
      printf("L----");
      times++;
    }
    if(root->is_deleted)
      printf("(%d)\n", root->key);
    else
      printf("%d\n", root->key);
    printTree(root->left, times, 0);
    printTree(root->right, times, 1);
  }
}

void avltree_free(struct avltree *tree)
{
    if (tree == NULL)
     return;
    avltree_free(tree->left);
    avltree_free(tree->right);
    free(tree);
}

struct avltree *avltree_lookup(struct avltree *tree, int key) {
  while (tree != NULL) {
    if (key == tree->key && !tree->is_deleted) {
      return tree; // Вернуть активный узел с найденным ключом
    } else if (key < tree->key) {
      tree = tree->left;
    } else {
      tree = tree->right;
    }
  }
  return NULL; // Элемент с указанным ключом не найден
}