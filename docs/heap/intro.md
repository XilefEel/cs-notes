# What is a Heap?

A **heap** is a binary tree that satisfies the **heap property**: every node must be greater than or equal to (or less than or equal to) all of its children.

Unlike a [BST](../binary-tree/bst.md), where the left child is smaller and the right child is larger, a heap only cares about the **parent-child relationship**.

```
        [9]
       /   \
     [7]   [8]
    /   \  /
  [3]  [4][5]
```

## The Heap Property

There are two types of heaps based on the heap property:

- **Max Heap**: every parent is **greater than or equal to** its children. The root of a max heap is always the **largest** element.
- **Min Heap**: every parent is **less than or equal to** its children. The root of a min heap is always the **smallest** element.

```
Max Heap:               Min Heap:
        [9]                     [1]
       /   \                   /   \
     [7]   [8]               [3]   [2]
    /   \  /                /   \  /
  [3]  [4][5]             [7]  [4][6]
```

## Heap vs BST

```
BST:                    Max Heap:
        [8]                     [9]
       /   \                   /   \
     [3]   [10]              [7]   [8]
    /   \                   /   \
  [1]   [6]               [3]   [4]
```

In the BST above, `[3]` is to the left of `[8]` because `3 < 8`. In the max heap above, `[7]` is to the left of `[9]` simply because it's a child, there is no left vs right ordering rule. If we had inserted `[8]` before `[7]`, then `[8]` would be the left child and `[7]` would be the right child, but the heap property would still hold since both children are less than the parent `[9]`.

|               | BST                 | Heap                        |
| ------------- | ------------------- | --------------------------- |
| Ordering rule | Left < Root < Right | Parent >= Children (max)    |
| Find min/max  | O(log n)            | O(1), always at the root    |
| Search        | O(log n) average    | O(n)                        |
| Use case      | Searching, sorting  | Priority queues, scheduling |


## Complete Binary Tree

A heap must also be a [complete binary tree](../binary-tree/types.md). All levels of the tree must be fully filled from left to right, except possibly the last level.

```
Complete (valid heap shape):    Not complete (invalid heap shape):
        [9]                             [9]
       /   \                           /   \
     [7]   [8]                       [7]   [8]
    /   \                               \
  [3]   [4]                             [4]
```

The right tree is invalid because the last level is not filled left to right (`[4]` is a right child with no left sibling).

::: info Why does the shape matter?
The complete binary tree property is what allows us to store a heap efficiently as a plain array, which we will cover in the next section.
:::

## When to use a Heap

- You need fast access to the **minimum or maximum** element
- You are implementing a **priority queue**
- You need to efficiently sort data with **heap sort**
- You need to solve **top K** problems (K largest, K most frequent)
