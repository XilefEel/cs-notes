# What is a Binary Search Tree?

A **Binary Search Tree (BST)** is a binary tree with one extra rule, which is that for every node:

- All nodes in the **left subtree** have values **less than** the node.
- All nodes in the **right subtree** have values **greater than** the node.

This rule applies to **every node** in the tree, not just the root.

## Structure

```
        [8]
       /   \
     [3]   [10]
    /   \     \
  [1]   [6]   [14]
       /   \
     [4]   [7]
```

`[8]` is the root.

Everything to the left of `[8]` is less than 8 — `[3]`, `[1]`, `[6]`, `[4]`, `[7]`.

Everything to the right of `[8]` is greater than 8 — `[10]`, `[14]`.

Everything to the left of `[3]` is less than 3 — `[1]`.

Everything to the right of `[3]` is greater than 3 — `[4]`, `[6]`, `[7]`.

## Valid vs Invalid BST

```
Valid BST:              Invalid BST:
      [8]                  [8]
     /   \                /   \
   [3]   [10]           [3]   [10]
  /   \                /   \
[1]  [6]             [1]   [9]
    /   \                 /   \
  [4]   [7]             [4]   [7]
```
The right tree is invalid since `[9]` is greater than its parent `[3]`, but it's also greater than the root `[8]`, so it should be in the right subtree of `[8]`, not the left.

::: tip
A common mistake is only checking if the left child is less than the parent and the right child is greater. But a valid BST must satisfy this for the **entire subtree**, not just the immediate children. 
:::

## Why is this useful?

Because the left subtree contains only **smaller** values and the right subtree contains only **larger** values, we can perform **efficient** search, insertion, and deletion operations.

For example, when we want to search for a value in the tree, we can compare it with the current node and decide whether to go **left** or **right**. This allows us eliminate half the tree at each step, and lead to an average time complexity of **O(log n)**.

```
Search for 4:

        [8]         4 < 8, go left
       /   \
     [3]   [10]     4 > 3, go right
    /   \
  [1]   [6]         4 < 6, go left
       /
     [4]            found!
```

## BST vs Regular Binary Tree

|               | Binary Tree | BST                 |
| ------------- | ----------- | ------------------- |
| Node ordering | No rule     | Left < Root < Right |
| Search        | O(n)        | O(log n) average    |
| Insert        | O(n)        | O(log n) average    |
| Delete        | O(n)        | O(log n) average    |

::: warning
These time complexities assume the tree is **balanced**. If the tree becomes unbalanced, operations can degrade to O(n).
:::
