# What is an AVL Tree?

An **AVL tree** is a [BST](../binary-tree/bst.md) that **automatically keeps itself balanced** after every insertion and deletion.

Unlike a regular BST where the tree can become unbalanced and degrade to O(n) operations, an AVL tree guarantees **O(log n)** for search, insertion, and deletion at all times.

```
Regular BST (unbalanced):      AVL Tree (balanced):
      [1]                             [4]
        \                            /   \
        [2]                        [2]   [5]
          \                       /   \
          [3]                   [1]   [3]
            \
            [4]
              \
              [5]
```

Both trees contain the same values, but the regular BST has degraded into a straight line while the AVL tree stays balanced.

## The Problem with Regular BSTs

Recall from the [BST](../binary-tree/bst.md) section that search, insertion, and deletion are all **O(log n) on average** but **O(n) in the worst case** when the tree becomes unbalanced.

```
Insert 1, 2, 3, 4, 5 in order into a regular BST:
      [1]
        \
        [2]
          \
          [3]
            \
            [4]
              \
              [5]
```

This is no better than a linked list and every operation now costs O(n) since we have to traverse the entire tree.

## How AVL Trees Solve This

After every insertion or deletion, an AVL tree checks if the tree has become unbalanced. If it has, it performs a **rotation** to restore balance.

```
Insert 1, 2, 3 into an AVL tree:

Insert 1:      Insert 2:      Insert 3:      After rotation:
  [1]            [1]            [1]              [2]
                   \              \             /   \
                   [2]            [2]         [1]   [3]
                                    \
                                    [3]
                          unbalanced! rotate left
```

The AVL tree detected that inserting `[3]` made the tree unbalanced and automatically rotated it to restore balance.

## The Height Property

Every node in an AVL tree stores its **height**, which is the number of edges from that node to its deepest leaf. This is how the tree detects imbalance.

```
        [4]         <- height 2
       /   \
     [2]   [5]      <- height 1, height 0
    /   \
  [1]   [3]         <- height 0, height 0
```

::: info Why store height?
Without storing height at each node, we'd have to recompute it by traversing the subtree every time we need it, which is expensive. Storing it directly means we can check balance in O(1) at any node.
:::

## AVL Tree vs BST

|               | BST                  | AVL Tree              |
| ------------- | -------------------- | --------------------- |
| Balance       | Not guaranteed       | Always balanced       |
| Search        | O(log n) average     | O(log n) guaranteed   |
| Insert        | O(log n) average     | O(log n) guaranteed   |
| Delete        | O(log n) average     | O(log n) guaranteed   |
| Extra storage | None                 | Height per node       |
| Complexity    | Simpler              | More complex          |
