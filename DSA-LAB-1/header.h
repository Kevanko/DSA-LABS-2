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
struct avltree *avltree_add(struct avltree *tree, int key, char* value);
void avltree_copy(struct avltree *tree, struct avltree **new_tree);
struct avltree *avltree_delete(struct avltree **tree, int key);
struct avltree *avltree_lookup(struct avltree *tree, int key);
void printTree(struct avltree *tree, int times, int last);
struct avltree *avltree_create(int key, char* value);
struct avltree *avltree_min(struct avltree *tree);
struct avltree *avltree_max(struct avltree *tree);
struct avltree *rightRotate(struct avltree *y);
struct avltree *leftRotate(struct avltree *x);
void printPreOrder(struct avltree *tree);
void avltree_free(struct avltree *tree);
int get_deleted(struct avltree *tree);
int get_count(struct avltree *tree);
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

struct avltree *avltree_min(struct avltree *tree){
  if(!tree) return NULL;
  struct avltree * min = avltree_min(tree->left);
  if (min && !min->is_deleted)
    return min;

  if (!tree->is_deleted)
    return tree;

  return avltree_min(tree->right);
 }

struct avltree *avltree_max(struct avltree *tree){
  if(!tree) return NULL;
  struct avltree * max = avltree_max(tree->right);
  if (max && !max->is_deleted)
    return max;

  if (!tree->is_deleted)
    return tree;

  return avltree_min(tree->left);
}

// Insert node
struct avltree *avltree_add(struct avltree *tree, int key, char* value) {
  // Find the correct position to avltree_add the node and avltree_add it
  if (tree == NULL)
    return (avltree_create(key, value));

  if (key < tree->key)
    tree->left = avltree_add(tree->left, key, value);
  else if (key > tree->key)
    tree->right = avltree_add(tree->right, key, value);
  else{
    tree->is_deleted = 0;
    tree->value = value;
    return tree;
  }

  // Update the balance factor of each node and
  // Balance the tree
  tree->height = 1 + max(height(tree->left),
               height(tree->right));

  int balance = getBalance(tree);
  if (balance > 1 && key < tree->left->key)
    return rightRotate(tree);

  if (balance < -1 && key > tree->right->key)
    return leftRotate(tree);

  if (balance > 1 && key > tree->left->key) {
    tree->left = leftRotate(tree->left);
    return rightRotate(tree);
  }

  if (balance < -1 && key < tree->right->key) {
    tree->right = rightRotate(tree->right);
    return leftRotate(tree);
  }

  return tree;
}

// avltree_delete ленивое удаление
struct avltree *avltree_delete(struct avltree **tree, int key) {
  // Найти узел и пометить его как удаленный
  struct avltree *node = avltree_lookup(*tree, key);
  if (node != NULL) {
    node->is_deleted = 1; // Пометить узел как удаленный
  }
  int deleted = get_deleted(*tree);
  int all = deleted + get_count(*tree);
  if((all / 2 + 1) <= deleted){
    struct avltree *new_tree = NULL;
    avltree_copy(*tree, &new_tree);
    avltree_free(*tree);
    *tree = new_tree;
  }
  return *tree;
}

// Print the tree
void printPreOrder(struct avltree *tree) {
  if (tree != NULL) {
    printf("%d ", tree->key);
    printPreOrder(tree->left);
    printPreOrder(tree->right);
  }
}
void printTree(struct avltree *tree, int times, int last) {
  if (tree != NULL) {
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
    if(tree->is_deleted)
      printf("(%d)\n", tree->key);
    else
      printf("%d\n", tree->key);
    printTree(tree->left, times, 0);
    printTree(tree->right, times, 1);
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

int get_deleted(struct avltree *tree){
  int result = 0;
  if(tree == NULL)
    return 0;
  if(tree->is_deleted == 1)
    result = 1;
  return result + get_deleted(tree->left) + get_deleted(tree->right);
}

int get_count(struct avltree *tree){
  int result = 0;
  if(tree == NULL)
    return 0;
  if(tree->is_deleted == 0)
    result = 1;
  return result + get_count(tree->left) + get_count(tree->right);
}

void avltree_copy(struct avltree *tree, struct avltree **new_tree) {
  if (tree == NULL)
    return;
  // Если узел не удаленный, добавляем его в новое дерево
  if (tree->is_deleted == 0) {
    *new_tree = avltree_add(*new_tree, tree->key, tree->value);
  }
  // Рекурсивно копируем левое и правое поддерево
  avltree_copy(tree->left, new_tree);
  avltree_copy(tree->right, new_tree);
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