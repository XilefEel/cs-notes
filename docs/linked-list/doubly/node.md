# Nodes in Doubly Linked List

A doubly linked node is similar to a singly linked node, but instead of only having one pointer `next`, it has **two** pointers: `next` (a pointer to the next node) and `prev` (a pointer to the previous node).

## In C

```c
typedef struct Node {
    int data;
    struct Node *next;  // Pointer to next node
    struct Node *prev;  // Pointer to previous node
} Node;
```

The `prev` pointer lets us traverse backwards through the list, which is the main advantage of doubly linked lists.

## In Rust?

Yeah imma be honest, I tried to implement this in Rust, but then Ferris (the Rust mascot) broke into my house at 3 AM and told me to stop cuz I was violating every rule known in Rust.

So... for doubly linked lists, no Rust version.

::: tip Why Doubly Linked Lists Break Rust
Each node needs both `next` and `prev` pointers. But if Node A points to Node B's `next`, and Node B points to Node A's `prev`, you have **circular ownership**.

Rust's entire safety model is built on **single ownership**. Doubly linked lists fundamentally violate this. You _can_ make them work with `Rc<RefCell<Node>>` or raw pointers, but it's very painful and un-idiomatic.
:::
