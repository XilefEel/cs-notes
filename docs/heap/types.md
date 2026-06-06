# Types of Heaps

There are two types of heaps, a **max heap** and a **min heap**. Both follow the same rules, the only difference is the direction of the heap property.

## Max Heap

In a **max heap**, every parent is **greater than or equal to** its children. The largest value is always at the root.

```
        [9]           <- root, largest element
       /   \
     [7]   [8]        <- children of [9]
    /   \  /  \
  [3]  [4][5]  [6]    <- leaves
```

`[9]` is the root and the largest element.
`[7]` is less than its parent `[9]`.
`[8]` is less than its parent `[9]`.
`[3]`, `[4]`, `[5]`, `[6]` are all less than their parent.

## Min Heap

In a **min heap**, every parent is **less than or equal to** its children. The smallest value is always at the root.

```
        [1]           <- root, smallest element
       /   \
     [3]   [2]        <- children of [1]
    /   \  /  \
  [7]  [4][6]  [5]    <- leaves
```

`[1]` is the root and the smallest element.
`[3]` is greater than its parent `[1]`.
`[2]` is greater than its parent `[1]`.
`[7]`, `[4]`, `[6]`, `[5]` are all greater than their parents.

## Valid vs Invalid

The heap property must be satisfied at **every node**, not just the root.

```
Valid Max Heap:          Invalid Max Heap:
        [9]                      [9]
       /   \                    /   \
     [7]   [8]                [7]   [8]
    /   \                    /   \
  [3]   [4]                [3]   [10]
```

The right tree is invalid because `[10]` is greater than its parent `[7]`, which violates the max heap property.

::: warning
A common mistake is only checking the root. The heap property must hold for **every parent-child pair** in the tree, not just at the top.
:::

## Max Heap vs Min Heap

|                  | Max Heap                       | Min Heap                       |
| ---------------- | ------------------------------ | ------------------------------ |
| Root             | Largest element                | Smallest element               |
| Heap property    | Parent >= Children             | Parent <= Children             |
| Extract          | Removes the largest            | Removes the smallest           |
| Use case         | Find max, descending sort      | Find min, priority queues      |

## Which one to use?

- Use a **max heap** when you need quick access to the **largest** element, like finding the top scores in a leaderboard
- Use a **min heap** when you need quick access to the **smallest** element, like finding the shortest path in a graph or scheduling tasks by priority

::: info
In practice, a **min heap** is more commonly used because priority queues typically process the **lowest priority value first**, such as the shortest distance in Dijkstra's algorithm.
:::
