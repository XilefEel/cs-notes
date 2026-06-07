#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int size;
    int capacity;
} Heap;

Heap create_heap(int capacity) {
    Heap h;
    h.data = (int *)malloc((capacity + 1) * sizeof(int));
    h.size = 0;
    h.capacity = capacity;
    return h;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int getParentIndex(int i) {
    return i / 2;
}

void upheap(Heap *heap, int index) {
    if (index <= 1) return;

    int parentIndex = getParentIndex(index);

    if (heap->data[index] <= heap->data[parentIndex]) return;

    swap(&heap->data[index], &heap->data[parentIndex]);
    upheap(heap, parentIndex);
}

void insert(Heap *heap, int value) {
    heap->size++;
    heap->data[heap->size] = value;

    upheap(heap, heap->size);
}

int main() {
    Heap max_heap = create_heap(10);
    Heap min_heap = create_heap(10);

    insert(&max_heap, 5);   // [_, 5]
    insert(&max_heap, 3);   // [_, 5, 3]
    insert(&max_heap, 8);   // [_, 8, 3, 5]
    insert(&max_heap, 1);   // [_, 8, 3, 5, 1]
    insert(&max_heap, 2);   // [_, 8, 3, 5, 1, 2]
    insert(&max_heap, 9);   // [_, 9, 3, 8, 1, 2, 5]

    for (int i = 1; i <= max_heap.size; i++) {
        printf("%d ", max_heap.data[i]);
    }

    return 0;
}
