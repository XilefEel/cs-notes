# What is a Node?

A node is the building block of a linked list. Every node contains two things:

- The **data** you want to store
- A **pointer** to the next node

## In C

```c
typedef struct Node {
    int data;
    struct Node *next;
} Node;
```

`int data` is the actual data stored in each `Node` (for this example, it's an integer).

`struct Node *next` is a raw pointer, a 64-bit integer representing a memory address. The pointer `next` either points to the next node or is `NULL` if there is no next node.

::: warning
C doesn't know if `next` points to a valid node, uninitialized junk memory, or `NULL`. If you dereference a `NULL` pointer, the program **crashes** (Segfault).
:::

## In Rust

```rust
struct Node {
    data: i32,
    next: Option<Box<Node>>,
}
```

`data: i32`, just like in C, is the actual data stored in each `Node`.

`Option<Box<Node>>` is (basically) the rust way to write `struct Node *` in C.

- `Box<T>` is a heap allocated pointer, like `malloc` but owned. The value is automatically freed when it goes out of scope.
- `Option<T>` is either `Some(value)` or `None`. This forces you to handle both cases explicitly (either there is a node, or there isn't a node).

::: tip
Unlike in C, the rust compiler forces you to handle `None` before you can use the value inside. The prevents runtime errors and moves them to compile time.
:::

## Key Difference

|               | C                    | Rust                |
| ------------- | -------------------- | ------------------- |
| Pointer Type  | `struct Node *next`  | `Option<Box<Node>>` |
| Valid Pointer | always valid pointer | `Some(Box<Node>)`   |
| Nullability   | `NULL`               | `None`              |
| Deallocation  | `free()`             | Automatic           |
