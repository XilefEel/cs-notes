# Accessing a Node

Since a linked list is not stored in contiguous memory, you **cannot access a node by index directly** like an array. Instead, you have to start at the head and follow the pointers one by one until you reach the node you want.

This makes accessing a node **O(n)** in the worst case.

## In C

```c
// Function that returns a pointer to the node at the given index, or NULL if out of bounds
Node *get_node(Node *head, int index) {
    Node *current = head;
    int i = 0;

    // Traverse the linked list from node to node
    while (current != NULL) {
        if (i == index) return current;
        current = current->next;
        i++;
    }

    return NULL; // Index out of bounds
}

// Usage
Node *node = get_node(head, 2); // Get the third node
if (node != NULL) {
    printf("%d\n", node->data);
}
```

`current = current->next` follows the pointer to the next node, moving one step forward.<br>
We return `NULL` if the index is out of bounds, so the caller must always check before using the result.

::: warning
If you forget to check for `NULL` before using the returned pointer, dereferencing it will **crash** your program with a segfault.
:::

## In Rust

```rust
impl Node {
    fn get(head: &Option<Box<Node>>, index: usize) -> Option<&Node> {
        let mut current = head;
        let mut i = 0;

        // Traverse the linked list from node to node
        while let Some(node) = current {
            if i == index return Some(node);
            current = &node.next;
            i += 1;
        }

        None // Index out of bounds
    }
}

// Usage
if let Some(node) = Node::get(&head, 2) {
    println!("{}", node.data);
}
```

`while let Some(node) = current` is Rust's way of writing `while (current != NULL)` in C.
Once it hits `None` (the end of the list), the loop stops automatically.<br>
We return `None` if the index is out of bounds, but unlike C, the caller **cannot** use the result without checking it first.

::: tip
In C, forgetting to check for `NULL` is a easy mistake that crashes your program at runtime. In Rust, `Option` forces you to handle the empty case, since the compiler won't let you ignore it.
:::

## Key Difference

|                  | C                            | Rust                   |
| ---------------- | ---------------------------- | ---------------------- |
| Traverse         | `current = current->next`    | `current = &node.next` |
| Out of bounds    | Returns `NULL`               | Returns `None`         |
| Caller check     | Manual `if (node != NULL)`   | Forced by `Option`     |
| Null dereference | Possible, crashes at runtime | Impossible, no crashes |
| Complexity       | O(n)                         | O(n)                   |
