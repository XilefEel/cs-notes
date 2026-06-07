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

int getLeftChildIndex(int i) {
    return 2 * i;
}

int getRightChildIndex(int i) {
    return 2 * i + 1;
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

void downheap(Heap *heap, int index) {
    if (getLeftChildIndex(index) > heap->size) return;

    int leftChildIndex = getLeftChildIndex(index);
    int rightChildIndex = getRightChildIndex(index);

    int largestIndex = index;

    if (heap->data[leftChildIndex] > heap->data[largestIndex]) {
        largestIndex = leftChildIndex;
    }

    if (
        rightChildIndex <= heap->size &&
        heap->data[rightChildIndex] > heap->data[largestIndex]
    ) {
        largestIndex = rightChildIndex;
    }

    if (largestIndex == index) return;

    swap(&heap->data[index], &heap->data[largestIndex]);
    downheap(heap, largestIndex);
}

int pop(Heap *heap) {
    if (heap->size == 0) return -1;

    int removed = heap->data[1];

    heap->data[1] = heap->data[heap->size];
    heap->size--;

    downheap(heap, 1);

    return removed;
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

    printf("\n");

    pop(&max_heap);     // returns 9, heap: [_, 8, 3, 5, 1, 2]
    pop(&max_heap);     // returns 8, heap: [_, 5, 3, 2, 1]
    pop(&max_heap);     // returns 5, heap: [_, 3, 1, 2]

    for (int i = 1; i <= max_heap.size; i++) {
        printf("%d ", max_heap.data[i]);
    }

    return 0;
}
