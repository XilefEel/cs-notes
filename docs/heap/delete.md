# Deleting an Element

Deleting (or popping) from a heap is more complex than inserting because we have to maintain the complete binary tree structure and the heap property after removal. Deleting from a heap always removes the **root**, but we can't just remove it and leave a gap, so we replace it with the **last element** in the heap and then perform a **downheap** operation to restore the heap property.

```
Delete root from a max heap:

Step 1: save root, move last element to root
        [9]                       [5]
       /   \                     /   \
     [3]   [8]      ->         [3]   [8]
    /   \  /                  /   \
  [1]  [2][5]               [1]   [2]

Save [9], move [5] to root, shrink size

Step 2: downheap(1)           Step 3: downheap(3)
        [8]                           [8]
       /   \                         /   \
     [3]   [5]          ->         [3]   [5]
    /   \                         /   \
  [1]   [2]                     [1]   [2]

5 < 8, swap with largest        no children, stop!
child [8]
```

## The Approach

- Save the **root** element to return it later
- Move the **last element** to the root and shrink the size by 1
- Compare the new root with its **children**
- Swap with the **largest child** (max heap) or **smallest child** (min heap) if it violates the heap property
- Repeat until we reach a **leaf** or the heap property is satisfied

## In C

```c
int getLeftChildIndex(int i) {
    return 2 * i;
}

int getRightChildIndex(int i) {
    return 2 * i + 1;
}

void downheap(Heap *heap, int index) {
    // Base case: no left child means we are at a leaf
    if (getLeftChildIndex(index) > heap->size) return;

    int leftChildIndex = getLeftChildIndex(index);
    int rightChildIndex = getRightChildIndex(index);

    // Assume current node is the largest
    int largestIndex = index;

    // Compare with left child
    if (heap->data[leftChildIndex] > heap->data[largestIndex]) {
        largestIndex = leftChildIndex;
    }

    // Compare with right child
    if (
        rightChildIndex <= heap->size &&
        heap->data[rightChildIndex] > heap->data[largestIndex]
    ) {
        largestIndex = rightChildIndex;
    }

    // Base case: heap property is satisfied
    if (largestIndex == index) return;

    swap(&heap->data[index], &heap->data[largestIndex]);
    downheap(heap, largestIndex);
}

int pop(Heap *heap) {
    // Base case: empty heap
    if (heap->size == 0) return -1;

    int removed = heap->data[1];

    heap->data[1] = heap->data[heap->size];
    heap->size--;

    downheap(heap, 1);

    return removed;
}

// Usage
Heap max_heap = create_heap(10);
insert(&max_heap, 5);
insert(&max_heap, 3);
insert(&max_heap, 8);
insert(&max_heap, 1);
insert(&max_heap, 2);
insert(&max_heap, 9);   // [_, 9, 3, 8, 1, 2, 5]

pop(&max_heap);     // returns 9, heap: [_, 8, 3, 5, 1, 2]
pop(&max_heap);     // returns 8, heap: [_, 5, 3, 2, 1]
pop(&max_heap);     // returns 5, heap: [_, 3, 1, 2]
```

`int removed = heap->data[1]` saves the root element so that we can return it at the end.

`heap->data[1] = heap->data[heap->size]` moves the last element to the root.

`downheap(heap, 1)` starts the downheap process from the root.

`getLeftChildIndex(index) > heap->size` is the base case when there are no children, meaning we are at a leaf.

`int largestIndex = index` assumes the current node is the largest before challenging with children.

`leftChildIndex <= heap->size && heap->data[leftChildIndex] > heap->data[largestIndex]` checks if the left child exists and is larger than the current largest. Same thing for the right child.

`if (largestIndex == index) return` is the base case when the heap property is already satisfied, so we stop.

`downheap(heap, largestIndex)` recurses downward with the largest child index.

::: warning
Same with our insertion implementation, this is for a **max heap**. For a min heap, we flip the comparisons to find the smallest child instead.
:::

::: tip
A leaf will always have no left child, because of the **complete binary tree** property. If a node has a right child but no left child, that means the tree is not complete, which is not allowed in a heap. This is why we only check for the left child to determine if we are at a leaf.
:::

::: info Why didn't we remove the last element?
In C, we didn't explicitly remove the last element from the array. Instead, we just decrease the `size` of the heap, which effectively ignores the last element in future operations. The next time we insert a new element, it will overwrite that position. This is a common pattern in C to avoid unnecessary array resizing or shifting.
:::

::: tip
Returning `-1` for an empty heap is a common C pattern but it's not ideal since `-1` could be a valid value in some cases. In production code you might want to use a separate `is_empty()` check before calling `pop()`.
:::

## In Rust

```rust
impl<T: Default + PartialOrd, F: Fn(&T, &T) -> bool> Heap<T, F> {
    fn left_child_index(i: usize) -> usize {
        2 * i
    }

    fn right_child_index(i: usize) -> usize {
        2 * i + 1
    }

    fn downheap(&mut self, index: usize) {
        // Base case: no left child means we are at a leaf
        if Self::left_child_index(index) >= self.data.len() {
            return;
        }

        let left_child_index = Self::left_child_index(index);
        let right_child_index = Self::right_child_index(index);

        // Assume current node is the largest
        let mut priority_index = index;

        // Compare with left child
        if (self.comparator)(&self.data[left_child_index], &self.data[priority_index]) {
            priority_index = left_child_index;
        }

        // Compare with right child
        if right_child_index < self.data.len() &&
            (self.comparator)(&self.data[right_child_index], &self.data[priority_index])
        {
            priority_index = right_child_index;
        }

        // Base case: heap property is satisfied
        if priority_index == index {
            return;
        }

        self.data.swap(index, priority_index);
        self.downheap(priority_index);
    }

    fn pop(&mut self) -> Option<T> {
        // Base case: empty heap
        if self.data.len() <= 1 {
            return None;
        }

        let last = self.data.len() - 1;
        self.data.swap(1, last);
        let removed = self.data.pop();

        self.downheap(1);

        removed
    }
}

// Usage
let mut max_heap = Heap::new(|a: &i32, b: &i32| a > b);
max_heap.insert(5);
max_heap.insert(3);
max_heap.insert(8);
max_heap.insert(1);
max_heap.insert(2);
max_heap.insert(9);
// [_, 9, 3, 8, 1, 2, 5]

max_heap.pop();   // returns Some(9), heap: [_, 8, 3, 5, 1, 2]
max_heap.pop();   // returns Some(8), heap: [_, 5, 3, 2, 1]
max_heap.pop();   // returns Some(5), heap: [_, 3, 1, 2]

let mut min_heap = Heap::new(|a: &i32, b: &i32| a < b);
min_heap.insert(5);
min_heap.insert(3);
min_heap.insert(8);
min_heap.insert(1);
min_heap.insert(2);
min_heap.insert(9);
// [_, 1, 2, 8, 5, 3, 9]

min_heap.pop();   // returns Some(1), heap: [_, 2, 3, 8, 5, 9]
min_heap.pop();   // returns Some(2), heap: [_, 3, 5, 8, 9]
min_heap.pop();   // returns Some(3), heap: [_, 5, 9, 8]
```

`if self.data.len() <= 1` is the base case for an empty heap since index `0` is always occupied by the dummy value.

`self.data.pop()` removes and returns the last element as `Option<T>`, which is the old root.

Unlike in C, we use `priority_index` instead of `largestIndex` because the same logic applies for both max and min heaps, so we just call it "priority" to be more generic.

`Self::left_child_index(index) >= self.data.len()` is the base case when there are no children.

`(self.comparator)(&self.data[left_child_index], &self.data[priority_index])` checks if the left child has higher priority than the current largest. Same thing for the right child.

::: info Why swap before pop?
We can't remove the root directly without breaking the array structure. Instead we swap the root with the last element so the old root is now at the end, then `pop()` removes it cleanly from the back of the `Vec`.
:::

## Complexity

| Operation | Average  | Worst case |
| --------- | -------- | ---------- |
| Delete    | O(log n) | O(log n)   |

Every deletion starts at the root and bubbles down at most `O(log n)` levels since the height of a complete binary tree is always `O(log n)`.

## Key Difference

|                    | C                                   | Rust                                       |
| ------------------ | ----------------------------------- | ------------------------------------------ |
| Return type        | `int`, `-1` for empty               | `Option<T>`, `None` for empty              |
| Remove root        | Manual index replacement            | `swap()` then `self.data.pop()`            |
| Left child index   | `getLeftChildIndex(i)`              | `Self::left_child_index(i)`                |
| Right child index  | `getRightChildIndex(i)`             | `Self::right_child_index(i)`               |
| Comparison         | Hardcoded `>` for max heap          | `(self.comparator)(&data[i], &data[j])`   |
| Heap type          | Separate logic for max and min      | Comparator closure handles both            |
| Empty check        | `heap->size == 0`                   | `self.data.len() <= 1`                     |
