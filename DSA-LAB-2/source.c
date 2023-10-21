#include "header.h"

int main() {
    char *apple = "apple";
    char *apples = "apples";
    char *banana = "banana";
    char *banka = "banka";
    char *value = "fruit";

    struct trie *root = create();
    root = insert(root, apple, value);
    root = insert(root, banana, value);
    root = insert(root, apples, value);
    root = insert(root, banka, value);
    root = delete(root, banka); 
    trie_print(root, 0);
    return 0;
}


