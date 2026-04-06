# What is a Queue?

A queue is a data structure that also stores a sequence of elements, but unlike a [stack](../stack/intro.md), it follows **FIFO** (First In, First Out). The first element added is the first one removed, like a queue of people waiting in line.

## Structure

```
FRONT                        BACK
  |                           |
  v                           v
 [1] -> [2] -> [3] -> [4] -> [5]

```

Elements are added to the **back** and removed from the **front**. Just like with stacks, we **cannot** access elements in the **middle** directly.

## Core Operations

| Operation    | Description                               | Time |
| ------------ | ----------------------------------------- | ---- |
| `enqueue(x)` | Add an element to the back                | O(1) |
| `dequeue()`  | Remove and return the front element       | O(1) |
| `peek()`     | Return the front element without removing | O(1) |
| `isEmpty()`  | Check if the queue is empty               | O(1) |

## Queue vs Stack

|         | Queue        | Stack           |
| ------- | ------------ | --------------- |
| Order   | FIFO         | LIFO            |
| Add     | Back         | Top             |
| Remove  | Front        | Top             |
| Example | Waiting line | Stack of plates |

## When to use a Queue

- You need to process elements in the order they arrived
- You are implementing [breadth-first search (BFS)](../binary-tree/traversal/level-order.md)
- You need to buffer or schedule tasks
- You are simulating real world lines or queues
