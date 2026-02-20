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

`struct Node *next` is a raw **pointer**, a 64-bit integer representing a memory address, that either points to the **next node** or is `NULL` if there is **no next node**.

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

`Option<Box<Node>>` is Rust's way of representing `struct Node *` in C, but safer.

- **`Box<T>`** is a heap-allocated pointer (a type of smart pointer). Unlike `malloc`, the memory is **automatically freed** when the `Box` goes out of scope. No manual cleanup needed!
- **`Option<T>`** is an enum that's either `Some(value)` or `None`. This forces you to explicitly **handle** both cases.

::: info What is T?
The `T` in `Box<T>` and `Option<T>` is a **generic type placeholder**. It means these types can hold any type, like integers, structs, strings, and etc. Here, `T` is the struct `Node`.
:::

::: tip
Unlike in C, the Rust compiler forces you to handle `None` before you can access the value inside. This **prevents null pointer crashes** by catching the error at compile time instead of runtime.
:::

## Key Difference

|               | C                    | Rust                |
| ------------- | -------------------- | ------------------- |
| Pointer Type  | `struct Node *next`  | `Option<Box<Node>>` |
| Valid Pointer | always valid pointer | `Some(Box<Node>)`   |
| Nullability   | `NULL`               | `None`              |
| Deallocation  | `free()`             | Automatic           |
