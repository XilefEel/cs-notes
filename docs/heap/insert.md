# Inserting an Element

Inserting an element into a heap is done in two steps. First, we add the new element to the **end** of the array. Then we restore the heap property by **bubbling it up** until it's in the right position. This process is called **upheap** or **heapify up**.

```
Before:
        [5]
       /   \
     [3]   [8]
    /   \
  [1]  [2]

Insert 9 into an existing max heap:

Step 1: add to end        Step 2: upheap(6)         Step 3: upheap(3)
        [5]                       [5]                       [9]
       /   \                     /   \                     /   \
     [3]   [8]                 [3]   [9]                 [3]   [8]
    /   \  /                  /   \  /                  /   \  /
  [1]  [2][9]               [1]  [2][8]               [1]  [2][5]

Add [9] at index 6        9 > 8, swap with parent    9 > 5, swap with parent

Step 4: upheap(1)
        [9]
       /   \
     [3]   [8]
    /   \  /
  [1]  [2][5]

index <= 1, stop!
```

## The Approach

- Add the new element at the **end** of the array, incrementing the size
- Compare the element with its **parent** using the formula `parentIndex = index / 2`
- If the element violates the heap property, **swap** it with its parent
- Repeat until we reach the **root** or the heap property is satisfied

## In C

```c
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int getParentIndex(int i) {
    return i / 2;
}

void upheap(Heap *heap, int index) {
    // Base case: reached the root
    if (index <= 1) return;

    int parentIndex = getParentIndex(index);

    // Base case: heap property is satisfied
    if (heap->data[index] <= heap->data[parentIndex]) return;

    swap(&heap->data[index], &heap->data[parentIndex]);

    upheap(heap, parentIndex);
}

void insert(Heap *heap, int value) {
    heap->size++;
    heap->data[heap->size] = value;

    upheap(heap, heap->size);
}

// Usage
Heap max_heap = create_heap(10);

insert(&max_heap, 5);   // [_, 5]
insert(&max_heap, 3);   // [_, 5, 3]
insert(&max_heap, 8);   // [_, 8, 3, 5]
insert(&max_heap, 1);   // [_, 8, 3, 5, 1]
insert(&max_heap, 2);   // [_, 8, 3, 5, 1, 2]
insert(&max_heap, 9);   // [_, 9, 3, 8, 1, 2, 5]
```

`heap->size++` increments the size before inserting so the new element lands at the next available index.

`heap->data[heap->size] = value` places the new element at the end of the array.

`upheap(heap, heap->size)` starts the upheap process from the newly inserted element, which is always at index `size`.

`if (index <= 1)` is the base case when we have reached the root, so we stop.

`if (heap->data[index] <= heap->data[parentIndex])` is the base case when the heap property is already satisfied, so we also stop.

`swap(&heap->data[index], &heap->data[parentIndex])` swaps the element with its parent.

`upheap(heap, parentIndex)` recurses upward with the parent index, now starting from the new position of that element.

::: warning
This implementation is for a **max heap**. For a min heap, we flip the comparison to `heap->data[index] >= heap->data[parentIndex]`. In Rust this is handled automatically by the comparator closure.
:::

## In Rust

```rust
impl<T: Default + PartialOrd, F: Fn(&T, &T) -> bool> Heap<T, F> {
    fn parent_index(i: usize) -> usize {
        i / 2
    }

    fn upheap(&mut self, index: usize) {
        // Base case: reached the root
        if index <= 1 {
            return;
        }

        let parent_index = Self::parent_index(index);

        // Base case: heap property is satisfied
        if !(self.comparator)(&self.data[index], &self.data[parent_index]) {
            return;
        }

        self.data.swap(index, parent_index);
        self.upheap(parent_index);
    }

    fn insert(&mut self, value: T) {
        self.data.push(value);
        let last = self.data.len() - 1;
        self.upheap(last);
    }
}

// Usage
let mut max_heap = Heap::new(|a: &i32, b: &i32| a > b);

max_heap.insert(5);     // [_, 5]
max_heap.insert(3);     // [_, 5, 3]
max_heap.insert(8);     // [_, 8, 3, 5]
max_heap.insert(1);     // [_, 8, 3, 5, 1]
max_heap.insert(2);     // [_, 8, 3, 5, 1, 2]
max_heap.insert(9);     // [_, 9, 3, 8, 1, 2, 5]

let mut min_heap = Heap::new(|a: &i32, b: &i32| a < b);

min_heap.insert(5);     // [_, 5]
min_heap.insert(3);     // [_, 3, 5]
min_heap.insert(8);     // [_, 3, 5, 8]
min_heap.insert(1);     // [_, 1, 3, 8, 5]
min_heap.insert(2);     // [_, 1, 2, 8, 5, 3]
min_heap.insert(9);     // [_, 1, 2, 8, 5, 3, 9]
```

The `PartialOrd` trait bound in `T: Default + PartialOrd` means that the type `T` must support comparison operations, which is necessary because we need to compare elements.

`self.data.push(value)` adds the new element to the end of the array, automatically incrementing the length.

`let last = self.data.len() - 1` gets the index of the newly inserted element.

`self.upheap(last)` starts the upheap process from the newly inserted element.

`!(self.comparator)(&self.data[index], &self.data[parent_index])` checks if the heap property is satisfied. If the comparator returns `false`, we stop.

`self.data.swap(index, parent_index)` swaps the element with its parent using `Vec`'s built in swap method, so we don't need a separate helper function.

`self.upheap(parent_index)` recurses upward with the parent index.

::: info Why !(self.comparator)?
In C, we hardcode the comparison to `<=` for a max heap, stopping when the child is less than or equal to the parent. In Rust, the comparator returns `true` when the child should be above the parent, so we stop when it returns `false`, which is why we negate it with `!`.
:::

## Complexity

| Operation | Average  | Worst case |
| --------- | -------- | ---------- |
| Insert    | O(log n) | O(log n)   |

Every insert starts at the bottom of the heap and bubbles up at most `O(log n)` levels since the height of a complete binary tree is always `O(log n)`.

## Key Difference

|                  | C                              | Rust                                     |
| ---------------- | ------------------------------ | ---------------------------------------- |
| Add to end       | `heap->data[++size] = value`   | `self.data.push(value)`                  |
| Swap             | Manual `swap()` helper         | `self.data.swap(index, parent_index)`    |
| Comparison       | Hardcoded `<=` for max heap    | `!(self.comparator)(&data[i], &data[p])` |
| Heap type        | Separate logic for max and min | Comparator closure handles both          |
