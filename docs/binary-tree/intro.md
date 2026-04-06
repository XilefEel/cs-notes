# What is a Binary Tree?

A **binary tree** is a data structure where each node has at most **two children**, a **left child** and a **right child**.

Unlike a [linked list](../linked-list/intro.md), [stack](../stack/intro.md), or [queue](../queue/intro.md), where each node points to only one next node, a binary tree branches in **two directions**, forming a tree-like structure.

```
            [1]
           /   \
         [2]   [3]
        /   \
      [4]   [5]
```

## Terminology

```
            [1]        <- Root (the topmost node)
           /   \
         [2]   [3]     <- Children of [1]
        /   \
      [4]   [5]        <- Leaves (no children)

[1] is the root of the tree
[1] is the parent of [2] and [3]
[2] is the parent of [4] and [5]
[2] and [3] are siblings (they are both children of [1])
[3], [4], and [5] are leaves
```

Some important terms to know about binary trees:

- **Root**: the topmost node of the tree, it has no parent
- **Parent**: a node that has one or more children
- **Child**: a node that has a parent
- **Leaf**: a node with no children
- **Siblings**: nodes that share the same parent
- **Height**: the number of edges from a node to its deepest leaf
- **Depth**: the number of edges from the root to a given node

## Height and Depth

```
            [1]         <- depth 0 or height 2
           /   \
         [2]   [3]      <- depth 1 or height 1 (0 for [3] since it's a leaf)
        /   \
      [4]   [5]         <- depth 2 or height 0

Height of the tree = 2 (root to the deepest leaf)

Height of [2] = 1 (from [2] to its deepest leaf)
Depth of [2] = 1 (from root [1] to [2])

Height of [4] = 0 (from [4] to its deepest leaf)
Depth of [4] = 2 (from root [1] to [4])

```

All leaves have a height of `0`, and the root has a depth of `0`.

## Structure

Each node in a binary tree has three things:

```
[ left | data | right ]
   ^              ^
   |              |
points to      points to
left child    right child
```

Unlike a linked list node which only has one `next` pointer, a binary tree node has **two pointers**, which are `left` and `right`.

## When to use a Binary Tree

- You need to represent hierarchical data (file systems, organization charts)
- You need fast search, insert, and delete operations (via BST)
- You need to process data in a sorted order
- You are implementing traversal algorithms like BFS and DFS
