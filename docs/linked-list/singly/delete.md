# Deleting a Node (Singly Linked List)

There are three ways to delete a node from a singly linked list:

- **Delete at head** — remove the first node (O(1))
- **Delete at tail** — remove the last node (O(n))
- **Delete at index** — remove a node at a specific position (O(n))

## Delete at Head

Removing the first node is the simplest operation. You just move the head pointer to the next node and free the old head.

### In C

```c
// Delete the first node in the list
void delete_at_head(Node **head) {
    // If list is empty, there is nothing to delete
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }

    // Save the current head
    Node *temp = *head;

    // Move head to the next node
    *head = (*head)->next;

    // Free the old head
    free(temp);
}

// Usage
Node *head = NULL;
insert_at_head(&head, 10);  // HEAD -> [10] -> NULL
insert_at_head(&head, 20);  // HEAD -> [20] -> [10] -> NULL
insert_at_head(&head, 30);  // HEAD -> [30] -> [20] -> [10] -> NULL
delete_at_head(&head);      // HEAD -> [20] -> [10] -> NULL
```

`Node *temp = *head` saves a pointer to the node we're about to delete.<br>
`*head = (*head)->next` moves the head pointer to the next node.<br>
`free(temp)` deallocates the memory for the old head.

::: warning
Notice that, just like inserting a node, we pass `Node **head` in the function signature (pointer to a pointer) and `&head` when calling it. This is because we need to modify the original `head` variable. The `&` gives us the address of `head`, and `Node **` receives that address. If you used `Node *head` instead, the function would only modify a local copy, so the caller's head wouldn't change.
:::

### In Rust

```rust
impl Node {
    // Takes ownership of the head, returns the new head (None if list becomes empty)
    fn delete_at_head(head: Option<Box<Node>>) -> Option<Box<Node>> {
        match head {
            None => {
                // List is empty, nothing to delete
                println!("List is empty");
                None
            }
            Some(node) => {
                // Return the next node as the new head
                // The old head is dropped automatically
                node.next
            }
        }
    }
}

// Usage
let mut head = None;
head = Node::insert_at_head(head, 10);  // HEAD -> [10] -> NONE
head = Node::insert_at_head(head, 20);  // HEAD -> [20] -> [10] -> NONE
head = Node::insert_at_head(head, 30);  // HEAD -> [30] -> [20] -> [10] -> NONE
head = Node::delete_at_head(head);      // HEAD -> [20] -> [10] -> NONE
```

`match head` handles both cases: if the list is empty (`None`), we just return `None`. If it has nodes (`Some`), we return `node.next`.<br>

::: tip
In C you must explicitly `free()` the old head. In Rust, the old node is automatically dropped when `node` goes out of scope at the end of the `Some` branch. This makes memory leaks **impossible**.
:::

### Key Difference

|                    | C                                  | Rust                                                              |
| ------------------ | ---------------------------------- | ----------------------------------------------------------------- |
| Function signature | `void delete_at_head(Node **head)` | `fn delete_at_head(head: Option<Box<Node>>) -> Option<Box<Node>>` |
| Empty check        | `if (*head == NULL)`               | `match head` with `None` branch                                   |
| Free memory        | `free(temp)` manually              | Automatic when `node` goes out of scope                           |
| Complexity         | O(1)                               | O(1)                                                              |

---

## Delete at Tail

Removing the last node requires traversing to the **second-to-last node** so you can update its `next` pointer to `NULL`.

This makes it O(n).

### In C

```c
// Delete the last node in the list
void delete_at_tail(Node **head) {
    // If list is empty, there is nothing to delete
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }

    // If there's only one node, delete it
    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }

    // Traverse to the second-to-last node
    Node *current = *head;
    while (current->next->next != NULL) {
        current = current->next;
    }

    // Free the last node
    free(current->next);

    // Set the second-to-last node's next to NULL
    current->next = NULL;
}

// Usage
Node *head = NULL;
insert_at_tail(&head, 10);  // HEAD -> [10] -> NULL
insert_at_tail(&head, 20);  // HEAD -> [10] -> [20] -> NULL
insert_at_tail(&head, 30);  // HEAD -> [10] -> [20] -> [30] -> NULL
delete_at_tail(&head);      // HEAD -> [10] -> [20] -> NULL
```

`while (current->next->next != NULL)` stops at the second-to-last node.<br>
`free(current->next)` deallocates the last node (basically deleting it).<br>
`current->next = NULL` Points the second-to-last node to NULL (becomes the new tail).

::: warning
We need to handle the single-node case separately because `current->next->next` would dereference `NULL` and **crash** if there's only one node.
:::

### In Rust

```rust
impl Node {
    // Delete the last node in the list
    fn delete_at_tail(head: Option<Box<Node>>) -> Option<Box<Node>> {
        match head {
            None => {
                // List is empty, nothing to delete
                println!("List is empty");
                None
            }
            Some(mut node) => {
                // If there's only one node, just delete the head
                if node.next.is_none() {
                    return Node::delete_at_head(Some(node));
                }

                // Traverse to the second-to-last node
                let mut current = &mut node;
                while current.next.as_ref().unwrap().next.is_some() {
                    current = current.next.as_mut().unwrap();
                }

                // Remove the last node by setting next to None
                // The old last node is automatically dropped
                current.next = None;

                // Return the original head
                Some(node)
            }
        }
    }
}

// Usage
let mut head = None;
head = Node::insert_at_tail(head, 10);  // HEAD -> [10] -> NONE
head = Node::insert_at_tail(head, 20);  // HEAD -> [10] -> [20] -> NONE
head = Node::insert_at_tail(head, 30);  // HEAD -> [10] -> [20] -> [30] -> NONE
head = Node::delete_at_tail(head);      // HEAD -> [10] -> [20] -> NONE
```

`match head`, just like before, handles both cases: if the list is empty (`None`), we just return `None`. If it has nodes (`Some`), we can continue.<br>
`if node.next.is_none()` checks if the next pointer is `None`, which we call `delete_at_head()` to delete it and returns `None` (empty list).<br>
`while current.next.as_ref().unwrap().next.is_some()` checks if there's a node two steps ahead. This is Rust's verbose way of writing `while (current->next->next != NULL)` in C.<br>
`current.next = None` sets the next node to be None, essentially removing the last node, which is then automatically dropped.

::: info What is .unwrap()?
`.unwrap()` extracts the value from an `Option` or `Result`. If the value is `Some(x)`, it returns `x`. If it's `None`, the program panics (crashes). We use it here because we've already checked that the value exists. If it doesn't, we want the program to crash loudly rather than continue with invalid data.

In production code, you'd typically handle `None` with `match` or `if let` instead of using `.unwrap()`.
:::

::: tip
In C you explicitly `free()` the last node. In Rust, when you set `current.next = None`, the old `Some(Box<Node>)` that was there gets dropped automatically. This makes memory leaks **impossible**.
:::

### Key Difference

|                    | C                                     | Rust                                                  |
| ------------------ | ------------------------------------- | ----------------------------------------------------- |
| Traverse condition | `while (current->next->next != NULL)` | `while current.next.as_ref().unwrap().next.is_some()` |
| Single node check  | `if ((*head)->next == NULL)`          | `if node.next.is_none()`                              |
| Free last node     | `free(current->next)` manually        | Automatic when `current.next = None`                  |
| Complexity         | O(n)                                  | O(n)                                                  |

---

## Delete at Index

Deleting a node at a specific index requires traversing to the node just **before** the target, then rewiring pointers to skip over the target node.

If the index is 0, this is the same as deleting at the head.

### In C

```c
// Delete the node at a specific index
void delete_at_index(Node **head, int index) {
    // If list is empty, there is nothing to delete
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }

    // If index is 0, delete at head
    if (index == 0) {
        delete_at_head(head);
        return;
    }

    // Traverse to the node just before the target index
    Node *current = *head;
    for (int i = 0; i < index - 1 && current->next != NULL; i++) {
        current = current->next;
    }

    // If current->next is NULL, index is out of bounds
    if (current->next == NULL) {
        printf("Index out of bounds\n");
        return;
    }

    // Save the node to delete
    Node *temp = current->next;

    // Point current to the node next to temp
    current->next = temp->next;

    // Free the node to delete
    free(temp);
}

// Usage
Node *head = NULL;
insert_at_tail(&head, 10);       // HEAD -> [10] -> NULL
insert_at_tail(&head, 20);       // HEAD -> [10] -> [20] -> NULL
insert_at_tail(&head, 30);       // HEAD -> [10] -> [20] -> [30] -> NULL
delete_at_index(&head, 1);       // HEAD -> [10] -> [30] -> NULL
```

We stop at `index - 1` because we need access to the node **before** the deletion point to rewire its `next` pointer.<br>
`Node *temp = current->next` saves a pointer to the node we're deleting.<br>
`current->next = temp->next` bypasses the node we're deleting, linking the previous node to the next node.<br>
`free(temp)` deallocates the deleted node.

::: warning
If the index is out of bounds, this function just prints an error and returns. In production code you'd want to handle it properly.
:::

### In Rust

```rust
impl Node {
    // Delete the node at a specific index
    fn delete_at_index(head: Option<Box<Node>>, index: usize) -> Option<Box<Node>> {
        match head {
            None => {
                // List is empty, nothing to delete
                println!("List is empty");
                None
            }
            Some(mut node) => {
                // If index is 0, delete at head (return the next node)
                if index == 0 {
                    return Node::delete_at_head(Some(node));
                }

                // Traverse to the node just before the target index
                let mut current = &mut node;
                for _ in 0..index - 1 {
                    // If current.next is None, index is out of bounds
                    if current.next.is_none() {
                        panic!("Index out of bounds");
                    }
                    // Move to the next node
                    current = current.next.as_mut().unwrap();
                }

                // Check if the target node exists
                if current.next.is_none() {
                    panic!("Index out of bounds");
                }

                // Skip over the target node by pointing current to target's next
                let target = current.next.take();
                current.next = target.unwrap().next;

                // Return the original head
                Some(node)
            }
        }
    }
}

// Usage
let mut head = None;
head = Node::insert_at_tail(head, 10);  // HEAD -> [10] -> NONE
head = Node::insert_at_tail(head, 20);  // HEAD -> [10] -> [20] -> NONE
head = Node::insert_at_tail(head, 30);  // HEAD -> [10] -> [20] -> [30] -> NONE
head = Node::delete_at_index(head, 1);  // HEAD -> [10] -> [30] -> NONE
```

`if index == 0` handles deletion at the head, so we call `delete_at_head()`<br>
`let target = current.next.take()` removes the target node from `current.next` and gives us ownership of it. In C, it's simmilar to `Node *temp = current->next`, except `.take()` also sets `current.next` to `None` automatically.<br>
`current.next = target.unwrap().next` links `current` directly to the node after `target`. The `target` node is automatically dropped (freed) when it goes out of scope.

### Key Difference

|                | C                                        | Rust                                      |
| -------------- | ---------------------------------------- | ----------------------------------------- |
| Stop condition | `i < index - 1 && current->next != NULL` | `for _ in 0..index - 1` + manual check    |
| Bypass node    | `current->next = temp->next`             | `current.next = target.unwrap().next`     |
| Free node      | `free(temp)` manually                    | Automatic when `target` goes out of scope |
| Out of bounds  | Print error or return                    | `panic!` or return `Result`               |
| Complexity     | O(n)                                     | O(n)                                      |

---

## Summary

| Operation       | C Complexity | Rust Complexity | Notes                                    |
| --------------- | ------------ | --------------- | ---------------------------------------- |
| Delete at head  | O(1)         | O(1)            | Just move head pointer                   |
| Delete at tail  | O(n)         | O(n)            | Must traverse to the second-to-last node |
| Delete at index | O(n)         | O(n)            | Must traverse to the node before target  |
