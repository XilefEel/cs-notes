# Types of Binary Trees

Not all binary trees are the same. There are several types of binary trees, each with different properties.

## Full Binary Tree

A **full binary tree** is a tree where every node has either **0 or 2 children**, no node has only one child.

```
        [1]
       /   \
     [2]   [3]
    /   \
  [4]   [5]

✓ Full, every node has 0 or 2 children


        [1]
       /   \
     [2]   [3]
    /
  [4]

✗ Not full, [2] has only one child
```

## Complete Binary Tree

A **complete binary tree** is a tree where every level is fully filled except possibly the last, and the last level is filled from **left to right**.

```
        [1]
       /   \
     [2]   [3]
    /   \  /
  [4]  [5][6]

✓ Complete, last level filled from the left


        [1]
       /   \
     [2]   [3]
    /   \     \
  [4]   [5]   [6]

✗ Not complete, last level not filled from the left
```

## Perfect Binary Tree

A **perfect binary tree** is a tree where every level is completely filled. All leaf nodes are at the same depth.

```
        [1]
       /   \
     [2]   [3]
    /   \ /   \
  [4] [5][6]  [7]

✓ Perfect, all levels completely filled
```

A perfect binary tree is both **full** and **complete**!

## Balanced Binary Tree

A **balanced binary tree** is a tree where the height of the left and right subtrees of every node differ by at most **1**.

```
        [1]
       /   \
     [2]   [3]
    /   \
  [4]   [5]

✓ Balanced, height difference is at most 1


        [1]
       /
     [2]
    /
  [3]

✗ Not balanced, left subtree is much taller
```

Balanced trees are important because they keep operations like search, insert, and delete at **O(log n)** instead of degrading to **O(n)** in the worst case.

## Summary

| Type     | Property                                            |
| -------- | --------------------------------------------------- |
| Full     | Every node has 0 or 2 children                      |
| Complete | All levels filled except last, filled left to right |
| Perfect  | All levels completely filled                        |
| Balanced | Height difference between subtrees is at most 1     |
