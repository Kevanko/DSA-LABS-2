#include"header.h"
#include <windows.h>
#define DELAY 10

int main() {
  struct avltree *root = NULL;
  double start, end;
  char* value = "go";

  root = avltree_add(root, 10, value);
   root = avltree_add(root, 5, value);
  root = avltree_add(root, 20, value);
  root = avltree_add(root, 4, value);
  root = avltree_add(root, 7, value);
  root = avltree_add(root, 8, value);
  root = avltree_add(root, 2, value);
  
  root = avltree_delete(&root, 2);
  root = avltree_delete(&root, 4);

  root = avltree_delete(&root, 5);
  root = avltree_add(root, 5, value);

  root = avltree_delete(&root, 7);
  //root = avltree_delete(&root, 20);

  printTree(root, 0, 1);

  printf("min: %d\n", avltree_min(root)->key);
  printf("max: %d\n", avltree_max(root)->key);
  printf("del: %d\n", get_deleted(root));
  printf("count: %d\n", get_count(root));

  for(int i = 0; i < 0; i++){
    root = avltree_add(root, i, value);
    if(i % 1000 == 0 && i > 0){
      start = clock();
      avltree_lookup(root, i);
      Sleep(DELAY);
      end = clock();
      printf("%d %d %.3f \n", i, height(root), (end - start - DELAY) / CLOCKS_PER_SEC);
    }
  }
  avltree_free(root);

  return 0;
}