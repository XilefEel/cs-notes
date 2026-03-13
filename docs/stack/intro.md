# What is a Stack?

A stack is a data structure that stores a sequence of elements, but unlike an array or linked list, we can only access elements from **one end**, which is the **top**.

This is called **LIFO** (Last In, First Out). The **last element** we put in is the **first one** that gets out, like a stack of pancakes.

## Structure

```
        TOP
         |
         v
        [4]   <-- most recently pushed, first to come out
         |
         v
        [3]
         |
         v
        [2]
         |
         v
        [1]   <-- first pushed, last to come out
```

Elements can only be **pushed** onto the top or **popped** off the top. We cannot access elements in the middle or bottom directly.

## Core Operations

| Operation   | Description                        | Time |
| ----------- | ---------------------------------- | ---- |
| `push(x)`   | Add an element to the top          | O(1) |
| `pop()`     | Remove and return the top element  | O(1) |
| `peek()`    | Return the top element without removing it | O(1) |
| `isEmpty()` | Check if the stack is empty        | O(1) |

## When to use a Stack

- You need to reverse a sequence of elements
- You need to track history and undo operations
- You are parsing nested structures like brackets or HTML tags
- You need to implement recursive algorithms iteratively
