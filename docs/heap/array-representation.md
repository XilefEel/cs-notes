# Heap as an Array

Unlike a [linked list](../linked-list/intro.md), [BST](../binary-tree/bst.md), or any other tree we have seen so far, a heap is almost always implemented as a plain **array** instead of a collection of nodes with pointers.

```
        [9]             index: 1
       /   \
     [7]   [8]          index: 2, 3
    /   \  /  \
  [3]  [4][5]  [6]      index: 4, 5, 6, 7

Array: [_, 9, 7, 8, 3, 4, 5, 6]
        0  1  2  3  4  5  6  7
```

We simply read the tree **level by level, left to right**, and place each node into the array at the next index.

## The Index Trick

Once the tree is stored as an array, we can navigate it using simple math. For any node at index `i`:

```
Parent:      i / 2
Left child:  2 * i
Right child: 2 * i + 1
```

For the example above:

```
Array: [_, 9, 7, 8, 3, 4, 5, 6]
        0  1  2  3  4  5  6  7

Node [7] is at index 2:
  Parent:      2 / 2 = 1      -> [9] ✓
  Left child:  2 * 2 = 4      -> [3] ✓
  Right child: 2 * 2 + 1 = 5  -> [4] ✓

Node [8] is at index 3:
  Parent:      3 / 2 = 1      -> [9] ✓
  Left child:  2 * 3 = 6      -> [5] ✓
  Right child: 2 * 3 + 1 = 7  -> [6] ✓
```

::: details 0-based indexing
Some textbooks and real-world implementations (including Rust's standard library `BinaryHeap`) use **0-based indexing** instead, where the root starts at index `0`. This shifts the formulas slightly:

```
Parent:      (i - 1) / 2
Left child:  (2 * i) + 1
Right child: (2 * i) + 2

Array: [9, 7, 8, 3, 4, 5, 6]
        0  1  2  3  4  5  6

Node [7] is at index 1:
  Parent:      (1 - 1) / 2 = 0  -> [9] ✓
  Left child:  (2 * 1) + 1 = 3  -> [3] ✓
  Right child: (2 * 1) + 2 = 4  -> [4] ✓
```

The algorithm is identical, it's just offset by 1. The 1-based approach wastes index `0` but the math is slightly cleaner.
:::

## Why This Works

This works because a heap is a **complete binary tree** and since every level is filled left to right with no gaps, we can map every node to an index in an array predictably and efficiently. If there were gaps in the tree, the index mapping would break.

```
Complete (heap):            Not complete (not a heap):
        [9]                         [9]
       /   \                       /   \
     [7]   [8]                   [7]   [8]
    /   \                            \
  [3]   [4]                          [4]

Array: [_, 9, 7, 8, 3, 4]      Array: [_, 9, 7, 8, ?, 4]
        0  1  2  3  4  5               0  1  2  3  4  5
```

In the right tree, index `4` has no node but index `5` does, which breaks the mapping. This is why heaps must always be complete binary trees.

## Why Not Use Pointers?

We could use `left` and `right` pointers like we did for the BST, but the array approach has many clear advantages:

|                  | Array                        | Nodes with pointers          |
| ---------------- | ---------------------------- | ---------------------------- |
| Memory           | No pointer overhead          | Extra memory for 2 pointers  |
| Cache efficiency | Elements stored contiguously | Scattered across memory      |
| Navigation       | Simple index math            | Follow pointers              |
| Implementation   | Simpler                      | More complex                 |

::: tip
Since array elements are stored **contiguously in memory**, the CPU can cache them efficiently. Pointer-based trees scatter nodes across memory, which can lead to more cache misses and slower performance in practice.
:::

## The Structure

### In C

```c
typedef struct {
    int *data;      // dynamic array to store elements
    int size;       // current number of elements
    int capacity;   // maximum number of elements
} Heap;
```

`data` is a pointer to a dynamically allocated array that stores the heap elements.

`size` tracks how many elements are currently in the heap.

`capacity` tracks how large the array is before we need to resize it.

### In Rust

```rust
struct Heap<T> {
    data: Vec<T>,   // dynamic array to store elements
}
```

`data` is a `Vec<T>` which handles sizing and capacity automatically, so we don't need to track them manually like in C.

::: info Why no size or capacity in Rust?
In C we manage the array manually, so we need to track both `size` and `capacity` ourselves. In Rust, `Vec<T>` handles this internally. We can use `data.len()` for size and `data.capacity()` for capacity whenever we need them.
:::
