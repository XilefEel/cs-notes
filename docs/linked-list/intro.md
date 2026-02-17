# What is a Linked List?

A linked list is a data structure that stores a sequence of elements. Unlike an array, the elements are **not** stored in contiguous memory — instead, each element lives somewhere on the heap (dynamically allocated memory) and holds a **pointer** to the **next element**.

## Structure

```
HEAD --> [data | next] --> [data | next] --> [data | next] --> NULL
```

Each element in a linked list is called a **node**. Each node contains:

- The actual **data** you want to store (i.e., int, float, char, string, structs)
- A **pointer** to the next node in the list

The list keeps track of the **head** pointer, which is a pointer to the first node. From there you can reach every other node by following the pointers. If `HEAD == NULL`, then the list is empty.

## Linked List vs Array

|                        | Array                  | Linked List                       |
| ---------------------- | ---------------------- | --------------------------------- |
| Memory                 | Contiguous             | Scattered on heap                 |
| Access by index        | O(1) Random Access     | O(n) Sequential Access            |
| Insert/Delete at front | O(n) Requires shifting | O(1) Update pointers              |
| Insert/Delete at end   | O(1) amortized         | O(n) without tail; O(1) with tail |
| Memory overhead        | Low                    | Higher (Extra pointer per node)   |

## Types of Linked Lists

- Singly Linked List: Each node points only to the next node. Traversal is one-way.
- Doubly Linked List: Each node has next and prev pointers. Allows two-way traversal but increases memory overhead.

## When to use a Linked List

- You need frequent insertions or deletions at the front
- You don't know the size of the list ahead of time
- You don't need random access by index

## When to use an Array instead

- You need to access elements by index frequently
- Memory efficiency matters (no pointer overhead)
- Cache performance matters (contiguous memory is CPU friendly)
