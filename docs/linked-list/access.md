# Accessing and Traversing a Linked List

Since a linked list is not stored in contiguous memory, you **cannot access a node by index directly** like an array. Instead, you have to start at the head and follow the pointers one by one.

There are two common traversal operations:

- **Access by index** — find a specific node at position N (O(n))
- **Traverse and print** — visit every node in the list (O(n))

## Access by Index

Getting a node at a specific index means walking through the list until we reach that position.

### In C

```c
// Returns a pointer to the node at the given index or NULL if out of bounds
Node *get_node(Node *head, int index) {
    Node *current = head;
    int i = 0;

    // Traverse the linked list from node to node
    while (current != NULL) {
        // If we've reached the target index, return the current node
        if (i == index) {
            return current;
        }

        // Move to the next node
        current = current->next;
        i++;
    }

    // Index out of bounds
    return NULL;
}

// Usage
Node *node = get_node(head, 2);  // Get the third node (index 2)
if (node != NULL) {
    printf("%d\n", node->data);
}
```

`current = current->next` reassigns the current node to the node after it. Essentially, we follow the pointer to the next node, moving one step forward through the list. This is how we walk or traverse through the list. <br>

We return `NULL` if the index is out of bounds, so the caller must always check before using the result.

::: warning
If you forget to check for `NULL` before using the returned pointer, dereferencing it will **crash** your program with a segfault.
:::

### In Rust

```rust
impl Node {
    fn get(head: &Option<Box<Node>>, index: usize) -> Option<&Node> {
        let mut current = head;
        let mut i = 0;

        // Traverse the linked list from node to node
        while let Some(node) = current {
            // If we've reached the target index, return the current node
            if i == index {
                return Some(node);
            }

            // Move to the next node
            current = &node.next;
            i += 1;
        }

        // Index out of bounds
        None
    }
}

// Usage
if let Some(node) = Node::get(&head, 2) {
    println!("{}", node.data);
}
```

`while let Some(node) = current` is Rust's way of writing `while (current != NULL)` in C. <br>

Once it hits `None` (the end of the list), the loop stops automatically. <br>

We return `None` if the index is out of bounds, but unlike C, the caller **cannot** use the result without checking it first.

::: tip
In C, forgetting to check for `NULL` is an easy mistake that crashes your program at runtime. In Rust, `Option` forces you to handle the empty case, since the compiler won't let you ignore it.
:::

### Key Difference

|                  | C                            | Rust                   |
| ---------------- | ---------------------------- | ---------------------- |
| Traverse         | `current = current->next`    | `current = &node.next` |
| Out of bounds    | Returns `NULL`               | Returns `None`         |
| Caller check     | Manual `if (node != NULL)`   | Forced by `Option`     |
| Null dereference | Possible, crashes at runtime | Impossible             |
| Complexity       | O(n)                         | O(n)                   |

## Traverse and Print

Sometimes, usually for visualizing or debugging, you want to visit every node in order and print its value.

### In C

```c
// Prints all nodes in the list
void print_list(Node *head) {
    Node *current = head;

    // Walk through the entire list
    while (current != NULL) {
        // Print the current node's data
        printf("%d", current->data);

        // Add an arrow if there's a next node
        if (current->next != NULL) {
            printf(" -> ");
        }

        // Move to the next node
        current = current->next;
    }

    printf("\n");
}

// Usage
print_list(head);  // Output: 10 -> 20 -> 30
```

Just like before, `current = current->next` traverses the list, but this time, we don't stop after reaching a certain index, rather we just go until the end of the list. <br>

`if (current->next != NULL)` makes sure we don't print an arrow after the last node.

### In Rust

```rust
impl Node {
    fn print_list(head: &Option<Box<Node>>) {
        let mut current = head;

        // Walk through the entire list
        while let Some(node) = current {
            // Print the current node's data
            print!("{}", node.data);

            // Add an arrow if there's a next node
            if node.next.is_some() {
                print!(" -> ");
            }

            // Move to the next node
            current = &node.next;
        }

        println!();
    }
}

// Usage
Node::print_list(&head);  // Output: 10 -> 20 -> 30
```

The `&` in `&Option<Box<Node>>` means that instead of taking ownership, we just want to **borrow** or **look at** the values, not modify or consume them. <br>

`node.next.is_some()` checks if there's another node. In C, it's the same as `current->next != NULL`.

::: tip
Notice we're using `&head` when calling `print_list()`. This borrows the list instead of moving it. After printing, we can still use `head` because we never took ownership, we only borrowed its value.
:::

### Key Difference

|            | C                              | Rust                             |
| ---------- | ------------------------------ | -------------------------------- |
| Loop       | `while (current != NULL)`      | `while let Some(node) = current` |
| Next check | `current->next != NULL`        | `node.next.is_some()`            |
| Ownership  | Works on a copy of the pointer | Borrows the list with `&`        |
| Complexity | O(n)                           | O(n)                             |

## Traverse and Apply a Function

A more powerful use of traversal is applying a function to each node's data. For example, you might want to double every value, or mark certain nodes as "visited".

### In C

```c
// Apply a function to every node's data
void traverse_apply(Node *head, void (*func)(int *)) {
    Node *current = head;

    // Walk through the entire list
    while (current != NULL) {
        // Apply the function to the current node's data
        func(&current->data);

        // Move to the next node
        current = current->next;
    }
}

// Example function: double the value
void double_value(int *data) {
    *data *= 2;
}

// Usage
traverse_apply(head, double_value);  // All values doubled
print_list(head);  // Output: 20 -> 40 -> 60
```

`void (*func)(int *)` is a **function pointer** so that we can pass any function we want to apply to each node in the list (so long as it follows the same type).<br>

The function receives an integer pointer to the data (`int *`), so it can modify the value directly in the node.

### In Rust

```rust
impl Node {
    fn traverse_apply<F>(head: &mut Option<Box<Node>>, mut func: F)
    where
        F: FnMut(&mut i32),
    {
        let mut current = head;

        // Walk through the entire list
        while let Some(node) = current {
            // Apply the function to the current node's data
            func(&mut node.data);

            // Move to the next node
            current = &mut node.next;
        }
    }
}

// Usage
Node::traverse_apply(&mut head, |data| *data *= 2);  // All values doubled
Node::print_list(&head);  // Output: 20 -> 40 -> 60
```

`FnMut(&mut i32)` is a **generic function** that simply means "any function that takes a mutable reference to an `i32` or integer". <br>

`|data| *data *= 2` is a **closure** (Rust's version of an anonymous function). This lets us write the logic inline without having to define a separate function. We could also pass a named function if we wanted. <br>

Notice we use `&mut head` because we're modifying the data inside the nodes.

::: tip
Although we could just **define** a function beforehand and use it in the traversal directly, making the function a parameter means you can pass **any** function, like doubling values, incrementing counters, filtering, mapping, or even complex logic, so long as it matches the generic function type. This pattern makes `traverse_apply` much more flexible.
:::

### Key Difference

|                  | C                                         | Rust                                            |
| ---------------- | ----------------------------------------- | ----------------------------------------------- |
| Function type    | Function pointer `void (*func)(int *)`    | Generic with `FnMut(&mut i32)`                  |
| Syntax           | `traverse_apply(head, double_value)`      | `Node::traverse_apply(&mut head, \|data\| ...)` |
| Inline functions | Not available (need to define separately) | Closures allow inline logic                     |
| Complexity       | O(n)                                      | O(n)                                            |

## Summary

Both operations are O(n) because you have to walk through nodes one by one, there's no way around it with a singly linked list. This is why arrays are faster for random access (O(1)), but linked lists shine when you need fast insertion/deletion at the front.
