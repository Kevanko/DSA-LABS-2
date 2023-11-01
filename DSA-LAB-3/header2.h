#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct fibHeap fibHeap;
typedef struct Node Node;

struct Node {
    Node *parent;
    Node *child;
    Node *left;
    Node *right;
    int key;
    int mark;
    int degree;
    char *value;
};

struct fibHeap {
    int nNodes;
    Node *min;
};

fibHeap *fibHeap_insert(fibHeap *heap, int key, char *value);
Node *fibHeap_min(fibHeap *heap);
fibHeap *fibHeap_union(fibHeap *heap1, fibHeap *heap2);
Node *fibHeap_delete_min(fibHeap *heap);
fibHeap *fibHeap_decrease_key(fibHeap *heap, Node *node, int newkey);
fibHeap *fibHeap_delete(fibHeap *heap, int key);

fibHeap *buildHeap() {
    fibHeap *heap = (fibHeap *)malloc(sizeof(fibHeap));
    heap->min = NULL;
    heap->nNodes = 0;
    return heap;
}

void fibHeap_add_to_root(Node *node, Node *heap) {
    if (heap == NULL) return;
    if (heap->left == heap) {
        heap->left = node;
        heap->right = node;
        node->right = heap;
        node->left = heap;
    } else {
        Node *lNode = heap->left;
        heap->left = node;
        node->right = heap;
        node->left = lNode;
        lNode->right = node;
    }
}

void fibHeap_add_to_root_list(Node *node, Node *heap) {
    if (heap == NULL) return;
    if (heap->child == NULL) 
    {
        heap->child = node;
    } 
    if(heap->child->left == heap->child)
    {
        heap->child->left = node;
        heap->child->right = node;
        node->left = heap->child;
        node->right = heap->child;
    }else {
        Node *lNode = heap->child->left;
        heap->child->left = node;
        node->right = heap->child;
        node->left = lNode;
        lNode->right = node;
    }
}

fibHeap *fibHeap_insert(fibHeap *heap, int key, char *value) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->degree = 0;
    node->parent = NULL;
    node->child = NULL;
    node->right = node;
    node->left = node;
    node->mark = 0;
    fibHeap_add_to_root(node, heap->min);
    if (heap->min == NULL || node->key < heap->min->key) {
        heap->min = node;
    }
    heap->nNodes += 1;
    return heap;
}

Node *fibHeap_min(fibHeap *heap) {
    return heap->min;
}

void fibHeap_link_lists(Node *first, Node *second) {
    if (first == NULL || second == NULL) return;

    Node *Left1 = first->left;
    Node *Left2 = second->left;

    Left1->right = second;
    second->left = Left1;

    Left2->right = first;
    first->left = Left2;
}


fibHeap *fibHeap_union(fibHeap *heap1, fibHeap *heap2) {
    fibHeap *heap = buildHeap();
    heap->min = heap1->min;
    fibHeap_link_lists(heap1->min, heap2->min);
    if (heap1->min == NULL || (heap2->min != NULL && heap2->min->key < heap->min->key)) {
        heap->min = heap2->min;
    }
    heap->nNodes = heap1->nNodes + heap2->nNodes;
    free(heap1);
    free(heap2);
    return heap;
}

void fibHeap_remove_from_root(Node *z, fibHeap *heap) {
    z->left->right = z->right;
    z->right->left = z->left;
    if (z == heap->min) 
    {
        if (z->right == z) {
            heap->min = NULL;
        } else {
            heap->min = z->right;
        }
    }
}

int D(int n) {
    return floor(log2(n));
}

void fibHeap_swap(Node *x, Node *y) {
    // Обмен детей между x и y
    Node *xChild = x->child;
    Node *yChild = y->child;
    
    if (xChild) {
        xChild->parent = y;
    }
    
    if (yChild) {
        yChild->parent = x;
    }

    x->child = yChild;
    y->child = xChild;
}


void fibHeap_link(fibHeap *heap, Node *x, Node *y) {
    fibHeap_remove_from_root(x, heap);
    x->left = x->right = x;
    x->parent = y;
    if (y->child) {
        fibHeap_link_lists(y->child, x);
    } else {
        y->child = x;
    }
    y->degree++;
    x->mark = 0;
}


void consolidate(fibHeap *heap) {
    int count = D(heap->nNodes);
    Node *arr[count + 1];
    for (int i = 0; i <= count; i++) {
        arr[i] = NULL;
    }
    Node *x = heap->min;
    do {
        int d = x->degree;
        while (arr[d] != NULL) {
            Node * y = arr[d];
            if (x->key > y->key) {
                Node * tmp = x;
                x = y;
                y = tmp;
            }
            if (y == heap->min) {
                heap->min = x;
            }
            fibHeap_link(heap, y, x);
            if (x->right == x) {
                heap->min = x;
            }
            arr[d] = NULL;
            d++;
        }
        arr[d] = x;
        x = x->right;
    } while (x != heap->min);
   /* Находим минимальный узел */
    heap->min = NULL;
    for(int i = 0; i <= count; i++){
        if (arr[i] != NULL)
        {
            //fibHeap_add_to_root(arr[i], heap->min);
            if(heap->min == NULL || arr[i]->key < heap->min->key)
            {
                heap->min = arr[i];
            }
        }
    }
}

// void fibheap_cut(fibHeap *heap, Node *x, Node *y) {
//     fibHeap_remove_from_root(x, heap);
//     fibHeap_link_lists(heap->min, x);
//     y->child = x->right;
//     x->left = x->right = x;
//     x->parent = NULL;
//     x->mark = 0;
// }
void fibheap_cut(fibHeap *heap, Node *x, Node *y) {
    // Убираем x из списка корней y
    if (x->right == x) {
        y->child = NULL;
    } else {
        y->child = x->right;
        x->right->left = x->left;
        x->left->right = x->right;
    }
    
    // Добавляем x в список корней кучи heap
    x->left = heap->min;
    x->right = heap->min->right;
    heap->min->right = x;
    x->right->left = x;
    
    x->parent = NULL;
    x->mark = 0;
}

void fibheap_cascading_cut(fibHeap *heap, Node *y) {
    Node *z = y->parent;
    if (z) {
        if (!y->mark) {
            y->mark = 1;
        } else {
            fibheap_cut(heap, y, z);
            fibheap_cascading_cut(heap, z);
        }
    }
}

fibHeap *fibHeap_decrease_key(fibHeap *heap, Node *x, int newkey) {
    if (newkey > x->key) {
        return heap;
    }
    x->key = newkey;
    Node *y = x->parent;
    if (y != NULL && x->key < y->key) {
        /* Нарушены свойства min-heap: ключ родителя больше */
        /* Вырезаем x и переносим его в список корней */
        fibheap_cut(heap, x, y);
        fibheap_cascading_cut(heap, y);
    }
    /* Корректируем указатель на минимальный узел */
    if (x->key < heap->min->key) {
        heap->min = x;
    }
    return heap;
}


Node *fibHeap_delete_min(fibHeap *heap) {
    Node *z = heap->min;
    if (z == NULL) {
        return NULL;
    }
    Node *x = z->child;
    while (x) {
        fibHeap_add_to_root(x, heap->min);
        x->parent = NULL;
        x = x->right;
        if (x == z->child) {
            x = NULL;
        }
    }
    fibHeap_remove_from_root(z, heap);
    if (z == z->right) {
        heap->min = NULL;
    } else {
        heap->min = z->right;
        consolidate(heap);
    }
    heap->nNodes -= 1;
    return z;
}

Node *fibheap_find_node_recursive(Node *node, int key) {
    if (node == NULL) {
        return NULL;
    }
    
    if (node->key == key) {
        return node;
    }
    
    Node *childNode = node->child;
    if (childNode != NULL) {
        do {
            Node *found = fibheap_find_node_recursive(childNode, key);
            if (found) {
                return found;
            }
            childNode = childNode->right;
        } while (childNode != node->child);
    }
    
    return NULL;
}


Node* fibheap_find_node(fibHeap *heap, int key) {
    if (heap == NULL || heap->min == NULL) {
        return NULL; // Куча пуста или не инициализирована
    }
    
    Node *minNode = heap->min;
    Node *node = minNode;
    
    do {
        if (node->key == key) {
            return node; // Найден узел с заданным ключом
        }
        
        if (node->child != NULL) {
            Node *childNode = node->child;
            do {
                Node *found = fibheap_find_node_recursive(childNode, key);
                if (found) {
                    return found; // Найден узел с заданным ключом в детях
                }
                childNode = childNode->right;
            } while (childNode != node->child);
        }
        
        node = node->right;
    } while (node != minNode);
    
    return NULL; // Узел с заданным ключом не найден
}


fibHeap *fibHeap_delete(fibHeap *heap, int key) {
    Node * node = fibheap_find_node(heap, key);
    fibHeap_decrease_key(heap, node, -2147483647);  // Decrease to negative infinity
    fibHeap_delete_min(heap);
    return heap;
}

void print_heap(Node *n, int level) {
    if (n == NULL) {
        printf("Heap is NULL\n");
        return;
    }

    Node *x = n;
    do {
        for(int i = 0; i < level; i++)
        {
            if(i == level - 1)
                printf("^---");
            else 
                printf("    ");
        }
        if (x->child == NULL) 
        {
            printf("Node with no child (%d)\n", x->key);
        } else {
            printf("Node(%d) with child (%d)\n", x->key, x->child->key);
            print_heap(x->child, level+1);
        }
        x = x->right;
    } while (x != n);
}
