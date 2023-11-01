#include "header2.h"
int main(){
    fibHeap *heap = buildHeap();
    fibHeap *heap2 = buildHeap();

    heap = fibHeap_insert(heap, 5, "Value 5");
    heap = fibHeap_insert(heap, 10, "Value 10");
    heap = fibHeap_insert(heap, 20, "Value 20");
    heap2 = fibHeap_insert(heap2, 30, "Value 30");
    heap2 = fibHeap_insert(heap2, 40, "Value 30");

    heap = fibHeap_union(heap, heap2);

    printf("> Initial heap:\n");
    print_heap(heap->min, 0);

    Node *min = fibHeap_min(heap);
    if (min) {
        printf(" {Minimum key: %d}\n", min->key);
    }

    fibHeap_delete_min(heap);
    fibHeap_delete(heap, 30);
    fibHeap_delete(heap, 40);

    printf("> Heap after deleting minimum:\n");
    print_heap(heap->min, 0);

    min = fibHeap_min(heap);
    if (min) {
        printf(" {Minimum key: %d}\n", min->key);
    }

    return 0;
}



