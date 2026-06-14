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

void upheap_max(Heap *heap, int index) {
    if (index <= 1) return;

    int parentIndex = getParentIndex(index);

    if (heap->data[index] <= heap->data[parentIndex]) return;

    swap(&heap->data[index], &heap->data[parentIndex]);
    upheap_max(heap, parentIndex);
}

void insert_max(Heap *heap, int value) {
    heap->size++;
    heap->data[heap->size] = value;

    upheap_max(heap, heap->size);
}

void downheap_max(Heap *heap, int index) {
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
    downheap_max(heap, largestIndex);
}

int pop_max(Heap *heap) {
    if (heap->size == 0) return -1;

    int removed = heap->data[1];

    heap->data[1] = heap->data[heap->size];
    heap->size--;

    downheap_max(heap, 1);

    return removed;
}

void upheap_min(Heap *heap, int index) {
    if (index <= 1) return;

    int parentIndex = getParentIndex(index);

    if (heap->data[index] >= heap->data[parentIndex]) return;

    swap(&heap->data[index], &heap->data[parentIndex]);
    upheap_min(heap, parentIndex);
}

void insert_min(Heap *heap, int value) {
    heap->size++;
    heap->data[heap->size] = value;

    upheap_min(heap, heap->size);
}

void downheap_min(Heap *heap, int index) {
    if (getLeftChildIndex(index) > heap->size) return;

    int leftChildIndex = getLeftChildIndex(index);
    int rightChildIndex = getRightChildIndex(index);

    int smallestIndex = index;

    if (heap->data[leftChildIndex] < heap->data[smallestIndex]) {
        smallestIndex = leftChildIndex;
    }

    if (
        rightChildIndex <= heap->size &&
        heap->data[rightChildIndex] < heap->data[smallestIndex]
    ) {
        smallestIndex = rightChildIndex;
    }

    if (smallestIndex == index) return;

    swap(&heap->data[index], &heap->data[smallestIndex]);
    downheap_min(heap, smallestIndex);
}

int pop_min(Heap *heap) {
    if (heap->size == 0) return -1;

    int removed = heap->data[1];

    heap->data[1] = heap->data[heap->size];
    heap->size--;

    downheap_min(heap, 1);

    return removed;
}

int peek(Heap *heap) {
    if (heap->size == 0) {
        printf("Heap is empty\n");
        return -1;
    }

    return heap->data[1];
}

int is_empty(Heap *heap) {
    return heap->size == 0;
}

int kth_largest(int *arr, int n, int k) {
    Heap min_heap = create_heap(k);

    for (int i = 0; i < n; i++) {
        insert_min(&min_heap, arr[i]);

        if (min_heap.size > k) {
            pop_min(&min_heap);
        }
    }

    return peek(&min_heap);
}

int *heap_sort(int *arr, int n) {
    Heap min_heap = create_heap(n);

    for (int i = 0; i < n; i++) {
        insert_min(&min_heap, arr[i]);
    }

    int *result = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        result[i] = pop_min(&min_heap);
    }

    return result;
}

int *sort_nearly_sorted(int *arr, int n, int k, int *result_size) {
    Heap min_heap = create_heap(k + 1);

    int i;
    for (i = 0; i <= k && i < n; i++) {
        insert_min(&min_heap, arr[i]);
    }

    *result_size = n;
    int *result = (int *)malloc(n * sizeof(int));
    int idx = 0;

    while (i < n) {
        result[idx++] = pop_min(&min_heap);
        insert_min(&min_heap, arr[i++]);
    }

    while (min_heap.size > 0) {
        result[idx++] = pop_min(&min_heap);
    }

    return result;
}

int main() {
    Heap max_heap = create_heap(10);
    Heap min_heap = create_heap(10);

    insert_max(&max_heap, 5);   // [_, 5]
    insert_max(&max_heap, 3);   // [_, 5, 3]
    insert_max(&max_heap, 8);   // [_, 8, 3, 5]
    insert_max(&max_heap, 1);   // [_, 8, 3, 5, 1]
    insert_max(&max_heap, 2);   // [_, 8, 3, 5, 1, 2]
    insert_max(&max_heap, 9);   // [_, 9, 3, 8, 1, 2, 5]

    pop_max(&max_heap);     // returns 9, heap: [_, 8, 3, 5, 1, 2]
    pop_max(&max_heap);     // returns 8, heap: [_, 5, 3, 2, 1]
    pop_max(&max_heap);     // returns 5, heap: [_, 3, 1, 2]

    int arr[] = {3, 1, 5, 12, 2, 8};
    int n = 6;
    int k = 2;

    int result = kth_largest(arr, n, k);
    printf("%d\n", result); // 8

    int unsorted[] = {3, 1, 5, 2, 8};
    n = 5;

    int *sorted = heap_sort(unsorted, n);

    // result: [1, 2, 3, 5, 8]
    for (int i = 0; i < n; i++) {
        printf("%d ", sorted[i]);
    }

    int nearly_sorted[] = {3, 2, 1, 5, 4, 7, 6};
    n = 7;
    k = 2;

    int result_size;
    sorted = sort_nearly_sorted(nearly_sorted, n, k, &result_size);

    // result: [1, 2, 3, 4, 5, 6, 7]
    for (int i = 0; i < result_size; i++) {
        printf("%d ", sorted[i]);
    }

    return 0;
}
