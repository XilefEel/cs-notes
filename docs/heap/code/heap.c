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

int main() {
    Heap max_heap = create_heap(10);
    Heap min_heap = create_heap(10);

    return 0;
}
