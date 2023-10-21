#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct trie trie;
struct trie {
    char *value;
    char ch;
    trie *sibling;
    trie *child;
};

trie *create();
trie *insert(trie *root, char *key, char *value);
trie *lookup(trie *root, char *key);
trie *delete(trie *root, char *key);
void trie_print(trie *root, int level);


trie *create() {
    trie *node = (trie *)malloc(sizeof(trie));
    if (node == NULL) {
        return NULL;
    }
    node->ch = '\0';
    node->value = NULL;
    node->sibling = NULL;
    node->child = NULL;

    return node;
}

trie *get_child(trie *root, char key){
    root = root->child;
    while(root){
        if(root->ch == key){
            return root;
        }
        root = root->sibling;
    }
    return NULL;
}

void set_child(trie *root, char key, trie *node){
    node->ch = key;
    if(!root->child) {
        root->child = node;
        return;
    }
    root = root->child;

    while(root){
        if(!root->sibling){
            root->sibling = node;
            return;
        }
        root = root->sibling;
    }
}

trie *insert(trie *root, char *key, char *value){
    if(!root || !key )
        return root;
    trie *node = root;
    for(int i = 0; key[i] != '\0'; i++){
        trie * child = get_child(node, key[i]);
        if(!child){
            child = create();
            set_child(node, key[i], child);
        }
        node = child;
    }
    node->value = value;
    return root;
}

trie *lookup(trie *root, char *key){
    trie* node = root;
    for(int i = 0; key[i] != '\0'; i++){
        trie *child = get_child(node, key[i]);
        if(!child){
            return NULL;
        }
        node = child;
    }
    if(node->value == NULL) return NULL;
    return node;
}

void trie_print(trie *root, int level) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < level; i++) {
        printf("|"); 
    }
    printf("%c\n", root->ch);
    trie_print(root->child, level + 1);
    trie_print(root->sibling, level);
}

int is_children(trie *root) {
    return (root != NULL && root->child != NULL);
}

void delete_recursive(trie *parent, trie *current, char *key, int depth) {
    if (current == NULL || key[depth] == '\0') {
        return;
    }

    // Find the child corresponding to the current character in the key
    trie *child = get_child(current, key[depth]);

    if (child == NULL) {
        // Key not found in the trie
        return;
    }

    // Recursively delete the node associated with the next character in the key
    delete_recursive(current, child, key, depth + 1);

    if (key[depth + 1] == '\0') {
        // We have reached the last character of the key
        // Delete the value associated with this node
        child->value = NULL;
    }

    if (!child->value && !is_children(child)) {
        // If the child has no value and no children, it can be safely deleted
        if (current->child == child) {
            current->child = child->sibling;
        } else {
            trie *sibling = current->child;
            while (sibling->sibling != child) {
                sibling = sibling->sibling;
            }
            sibling->sibling = child->sibling;
        }
        free(child);
    }

    // If the current node has no value and no children, it can also be safely deleted
    if (current != parent && !current->value && !is_children(current)) {
        if (parent->child == current) {
            parent->child = current->sibling;
        } else {
            trie *sibling = parent->child;
            while (sibling->sibling != current) {
                sibling = sibling->sibling;
            }
            sibling->sibling = current->sibling;
        }
        free(current);
    }
}

trie *delete(trie *root, char *key) {
    if (root == NULL || key == NULL) {
        return root;
    }

    delete_recursive(NULL, root, key, 0);

    return root;
}
