# What is a Linked List?

A linked list is a data structure that stores a sequence of elements. Unlike an array, the elements are **not** stored in contiguous memory (next to each other). Each element lives somewhere on the heap (dynamically allocated memory) and holds a **pointer** to the **next element** (and the **previous element** for doubly).

## Structure

### Singly Linked List

```
HEAD --> [data | next] --> [data | next] --> [data | next] --> NULL
```

`[data | next]` represents a node in a linked list.

`data` is the actual data being stored in the node.

`next` is a pointer that points to the next node.

`HEAD` is a special pointer. It points to the head of the linked list. It is **NOT** a node.

`NULL` means there is nothing. The last node will always point to `NULL`.

Each node only points forward to the next node. You can only traverse in one direction.

### Doubly Linked List

```
NULL <-- [prev | data | next] <-> [prev | data | next] <-> [prev | data | next] --> NULL
                   ^
                  HEAD
```

`prev` is a pointer (just like `head`) that points to the previous node.

Each node points both forward (`next`) and backward (`prev`). You can traverse in both directions.

## Singly vs Doubly Linked Lists

| Feature                   | Singly Linked     | Doubly Linked                          |
| ------------------------- | ----------------- | -------------------------------------- |
| Pointers per node         | 1 (`next`)        | 2 (`next` + `prev`)                    |
| Traversal direction       | Forward only      | Both directions                        |
| Insert at head            | O(1)              | O(1)                                   |
| Insert at tail            | O(n) without tail | O(n) without tail                      |
| Delete at tail            | O(n)              | O(1) with prev pointer                 |
| Memory overhead           | Lower             | Higher (extra pointer)                 |
| Implementation complexity | Simpler           | More complex (more pointers to manage) |

## Linked List vs Array

|                        | Array                  | Linked List                       |
| ---------------------- | ---------------------- | --------------------------------- |
| Memory                 | Contiguous             | Scattered on heap                 |
| Access by index        | O(1) Random Access     | O(n) Sequential Access            |
| Insert/Delete at front | O(n) Requires shifting | O(1) Update pointers              |
| Insert/Delete at end   | O(1) amortized         | O(n) without tail; O(1) with tail |
| Memory overhead        | Low                    | Higher (Extra pointer per node)   |

## When to use a Linked List

- You need frequent insertions or deletions at the front
- You don't know the size of the list ahead of time
- You don't need random access by index
- (Doubly) You need to traverse backwards

## When to use an Array instead

- You need to access elements by index frequently
- Memory efficiency matters (no pointer overhead)
- Cache performance matters (contiguous memory is CPU friendly)
