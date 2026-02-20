# Inserting a Node (Doubly Linked List)

Just like in singly linked lists, there are three ways to insert a new node into a doubly linked list:

- **Insert at head** — add to the front (O(1))
- **Insert at tail** — add to the end (O(n) without tail pointer, O(1) with tail pointer)
- **Insert at index** — add at a specific position (O(n))

---

## Insert at Head

Adding a node to the front is slightly more complex than in a singly linked list because we need to update **both** the `next` and `prev` pointers.

```c
// Insert a new node at the front of the list
void insert_at_head(Node **head, int data) {
    // Create a new node
    Node *new_node = create_node(data);

    // New head has no previous node
    new_node->prev = NULL;

    // Point the new node to the old head
    new_node->next = *head;

    // If list is not empty, update old head's prev pointer
    if (*head != NULL) {
        (*head)->prev = new_node;
    }

    // Update head to point to the new node
    *head = new_node;
}

// Usage
Node *head = NULL;
insert_at_head(&head, 10);  // HEAD <-> [10] <-> NULL
insert_at_head(&head, 20);  // HEAD <-> [20] <-> [10] <-> NULL
insert_at_head(&head, 30);  // HEAD <-> [30] <-> [20] <-> [10] <-> NULL
```

`new_node->prev = NULL` because the head has no previous node.

`(*head)->prev = new_node` updates the old head's `prev` to point to the new node.

::: warning
Don't forget to update `prev` pointers! Forgetting this is a common bug that breaks backwards traversal.
:::

## Insert at Tail

Adding a node to the end requires traversing to the last node, then updating pointers in both directions.

```c
// Insert a new node at the end of the list
void insert_at_tail(Node **head, int data) {
    Node *new_node = create_node(data);

    // New tail has no next node
    new_node->next = NULL;

    // If list is empty, new node becomes the head
    if (*head == NULL) {
        new_node->prev = NULL; // Don't forget to update prev
        *head = new_node;
        return;
    }

    // Traverse to the last node
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    // Point the last node's next to the new node
    current->next = new_node;

    // Point the new node back to the last node
    new_node->prev = current;
}

// Usage
Node *head = NULL;
insert_at_tail(&head, 10);  // HEAD <-> [10] <-> NULL
insert_at_tail(&head, 20);  // HEAD <-> [10] <-> [20] <-> NULL
insert_at_tail(&head, 30);  // HEAD <-> [10] <-> [20] <-> [30] <-> NULL
```

Just like in singly linked lists, we traverse with `while (current->next != NULL)` to find the last node.

`new_node->prev = current` points the new node (the new tail) back to the old tail.

::: warning
Inserting at the tail n times in a row is still O(n²) total, just like with singly linked lists. If you're building a list from scratch, consider keeping a **tail pointer** or inserting at the head.
:::

## Insert at Index

Inserting at a specific index is similar to singly linked lists, but now we need to update **four** pointers instead of two.

```c
// Insert a new node at a specific index
void insert_at_index(Node **head, int data, int index) {
    // If index is 0, insert at head
    if (index == 0) {
        insert_at_head(head, data);
        return;
    }

    // Traverse to the node just before the target index
    Node *current = *head;
    for (int i = 0; i < index - 1 && current != NULL; i++) {
        current = current->next;
    }

    // If current is NULL, index is out of bounds
    if (current == NULL) {
        printf("Index out of bounds\n");
        return;
    }

    // Create a new node
    Node *new_node = create_node(data);

    // Point new node's next to what current's next was pointing to
    new_node->next = current->next;

    // Point new node's prev to where current is
    new_node->prev = current;

    // Update the next node's prev pointer (if it exists)
    if (current->next != NULL) {
        current->next->prev = new_node;
    }

    // Point current node's next to the new node
    current->next = new_node;
}

// Usage
Node *head = NULL;
insert_at_tail(&head, 10);      // HEAD <-> [10] <-> NULL
insert_at_tail(&head, 30);      // HEAD <-> [10] <-> [30] <-> NULL
insert_at_index(&head, 20, 1);  // HEAD <-> [10] <-> [20] <-> [30] <-> NULL
```

We stop at `index - 1` because we need access to the node **before** the insertion point to rewire its pointers.

The four pointer updates happen in this order:

1. `new_node->next = current->next` makes `new node` point forward to the node `current` was pointing to.
2. `new_node->prev = current` updates `new node` to point backward to `current`.
3. `current->next->prev = new_node` makes the node after `new node` point back to it.
4. finally, `current->next = new_node` points `current` forward to the `new node`.

::: tip
For comparison, in singly linked lists, we only update:

- `new_node->next = current->next`
- `current->next = new_node`

In doubly linked lists, we update **4 pointers** because we need to maintain both forward and backward links.
:::

::: warning
The `if (current->next != NULL)` check is crucial! If we're inserting at the end, there's no next node to update. Forgetting this check causes a segfault.
:::

## Summary

Doubly linked lists give you **bidirectional traversal** at the cost of:

- More complex insertion logic (more pointers to update)
- Higher memory usage (extra `prev` pointer per node)
- More potential for bugs (forgetting to update `prev` pointers)

**When to use:**

- You need to traverse backwards frequently
- You're implementing a deque (double-ended queue)
- You need to delete nodes in the middle efficiently (easier with `prev` access)

**When to skip:**

- You only traverse forward (use singly linked list)
- Memory is tight (the extra pointers add up)
- You're using Rust 🦀
