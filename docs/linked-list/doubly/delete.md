# Deleting a Node (Doubly Linked List)

Just like in singly linked lists, there are three ways to delete a node from a doubly linked list:

- **Delete at head** — remove the first node (O(1))
- **Delete at tail** — remove the last node (O(n))
- **Delete at index** — remove a node at a specific position (O(n))

## Delete at Head

Removing the first node requires updating both the head pointer and the new head's `prev` pointer.

```
Step 1: Save reference to old head
NULL <- [30] <-> [20] <-> [10] -> NULL
         ^
        temp

Step 2: Move head to next node
        [30] <-> [20] <-> [10] -> NULL
         ^        ^
        temp     HEAD

Step 3: Update new head's prev to NULL
        NULL <- [20] <-> [10] -> NULL
                 ^
temp -> [30]    HEAD

Step 4: Free old head
NULL <- [20] <-> [10] -> NULL

```

### In C

```c
// Delete the first node in the list
void delete_at_head(Node **head) {
    // If list is empty, nothing to delete
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }

    // Save the current head
    Node *temp = *head;

    // Move head to the next node
    *head = (*head)->next;

    // If new head exists, update its prev to NULL
    if (*head != NULL) {
        (*head)->prev = NULL;
    }

    // Free the old head
    free(temp);
}

// Usage
Node *head = NULL;
insert_at_head(&head, 10);  // NULL <- [10] -> NULL
insert_at_head(&head, 20);  // NULL <- [20] <-> [10] -> NULL
insert_at_head(&head, 30);  // NULL <- [30] <-> [20] <-> [10] -> NULL
delete_at_head(&head);      // NULL <- [20] <-> [10] -> NULL
```

`Node *temp = *head` saves a pointer to the node we're about to delete.

`*head = (*head)->next` moves the head pointer to the next node.

`if (*head != NULL)` checks if there's still a node in the list after deleting. If yes, we set its `prev` to `NULL` since it's now the new head.

`free(temp)` deallocates the old head.

::: warning
We have to update the new head's `prev` pointer to `NULL`. Forgetting this breaks backward traversal.
:::

## Delete at Tail

Removing the last node is easier in doubly linked lists than singly because we can access the second-to-last node through the tail's `prev` pointer.

```
Step 1: Traverse to last node
NULL <- [10] <-> [20] <-> [30] -> NULL
                           ^
                          current

Step 2: Move to second-to-last via prev
NULL <- [10] <-> [20] <-> [30] -> NULL
                           ^
                          current

Step 3: Set second-to-last's next to NULL
NULL <- [10] <-> [20] -> NULL
                         [30] <- current

Step 4: Free old tail
NULL <- [10] <-> [20] -> NULL
```

### In C

```c
// Delete the last node in the list
void delete_at_tail(Node **head) {
    // If list is empty, nothing to delete
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }

    // If there's only one node
    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }

    // Traverse to the last node
    Node *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    // Update second-to-last node's next to NULL
    current->prev->next = NULL;

    // Free the last node
    free(current);
}

// Usage
Node *head = NULL;
insert_at_tail(&head, 10);  // NULL <- [10] -> NULL
insert_at_tail(&head, 20);  // NULL <- [10] <-> [20] -> NULL
insert_at_tail(&head, 30);  // NULL <- [10] <-> [20] <-> [30] -> NULL
delete_at_tail(&head);      // NULL <- [10] <-> [20] -> NULL
```

`while (current->next != NULL)` traverses to the last node.

Once we're at the tail, we can use `current->prev` to directly access the second-to-last node.

`current->prev->next = NULL` points its `next` to `NULL`.

`free(current)` deallocates the last node.

::: tip
Although doubly linked lists makes tail deletion cleaner, it is still O(n) without a tail pointer, since O(n-1) = O(n).
:::

## Delete at Index

Deleting at a specific index requires traversing to the target node, then rewiring both `prev` and `next` pointers of the nodes before and after it.

```
Step 1: Traverse to target index
NULL <- [10] <-> [20] <-> [30] -> NULL
                   ^
                 target (index 1)

Step 2: Link previous node to next node
NULL <- [10] -> [30] -> NULL
                 ^
                [20]

Step 3: Link next node to previous node
NULL <- [10] <-> [30] -> NULL
                  ^
                 [20]

Step 4: Free target node
NULL <- [10] <-> [30] -> NULL
```

### In C

```c
// Delete the node at a specific index
void delete_at_index(Node **head, int index) {
    // If list is empty, nothing to delete
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }

    // If index is 0, delete at head
    if (index == 0) {
        delete_at_head(head);
        return;
    }

    // Traverse to the target index
    Node *current = *head;
    for (int i = 0; i < index && current != NULL; i++) {
        current = current->next;
    }

    // If current is NULL, index is out of bounds
    if (current == NULL) {
        printf("Index out of bounds\n");
        return;
    }

    // Link previous node to next node
    if (current->prev != NULL) {
        current->prev->next = current->next;
    }

    // Link next node back to previous node
    if (current->next != NULL) {
        current->next->prev = current->prev;
    }

    // Free the target node
    free(current);
}

// Usage
Node *head = NULL;
insert_at_tail(&head, 10);       // NULL <- [10] -> NULL
insert_at_tail(&head, 20);       // NULL <- [10] <-> [20] -> NULL
insert_at_tail(&head, 30);       // NULL <- [10] <-> [20] <-> [30] -> NULL
delete_at_index(&head, 1);       // NULL <- [10] <-> [30] -> NULL
```

Unlike singly linked lists where we stop at `index - 1`, we traverse directly to `index` because we have `prev` pointers to access the node before it.

`current->prev->next = current->next` points the previous node to the next node.

`current->next->prev = current->prev` points the next node back to the previous node.

`free(current)` deallocates the target node.

::: warning
Always check `if (current->prev != NULL)` and `if (current->next != NULL)` to avoid dereferencing `NULL` when deleting edge nodes.
:::

## Key Differences from Singly Linked Lists

| Operation       | Singly Linked     | Doubly Linked     | Why?                                 |
| --------------- | ----------------- | ----------------- | ------------------------------------ |
| Delete at head  | 2 pointer updates | 3 pointer updates | Must update new head's `prev`        |
| Delete at tail  | 2 pointer updates | 3 pointer updates | Can access second-to-last via `prev` |
| Delete at index | 2 pointer updates | 4 pointer updates | Both directions need updating        |
| Memory overhead | Lower             | Higher            | Extra `prev` pointer per node        |

## Summary

| Operation       | C Complexity | Notes                                       |
| --------------- | ------------ | ------------------------------------------- |
| Delete at head  | O(1)         | Must update new head's `prev` to NULL       |
| Delete at tail  | O(n)         | Can access second-to-last via `prev`        |
| Delete at index | O(n)         | Must update both `prev` and `next` pointers |
