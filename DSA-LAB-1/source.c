#include"header.h"
#include <windows.h>

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
  
  root = avltree_delete(root, 2);
  root = avltree_delete(root, 4);
  root = avltree_delete(root, 5);
  root = avltree_delete(root, 10);
  root = avltree_delete(root, 20);

  root = avltree_add(root, 5, value);

  printTree(root, 0, 1);

  printf("min: %d\n", avltree_min(root)->key);
  printf("max: %d\n", avltree_max(root)->key);

  for(int i = 0; i < 50001; i++){
    root = avltree_add(root, i, value);
    if(i % 1000 == 0 && i > 0){
      start = clock();
      avltree_lookup(root, i);
      Sleep(10);
      end = clock();
      printf("%d %d %.3f \n", i, height(root), (end - start - 10) / CLOCKS_PER_SEC);
    }
  }


  avltree_free(root);

  return 0;
}