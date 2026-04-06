# Enqueuing an Element

Enqueuing adds a new element to the **back** of the queue. Enqueuing an element is also very similar to inserting a node at the head of a [linked list](../linked-list/singly/insert.md).

```
Before: FRONT -> [10] -> [20] -> [30] <- BACK

Step 1: Create a new node
        [40]

Step 2: Point the current back to the new node
FRONT -> [10] -> [20] -> [30] -> [40]
                          ^
                         BACK

Step 3: Update back to point to the new node
FRONT -> [10] -> [20] -> [30] -> [40] <- BACK
```

## In C

```c
void enqueue(Queue *q, int data) {
    // Create a new node
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;

    // If the queue is empty, front and back both point to the new node
    if (q->back == NULL) {
        q->front = node;
        q->back = node;
    } else {
        // Point the current back to the new node
        q->back->next = node;
        // Update back to point to the new node
        q->back = node;
    }

    q->size++;
}

// Usage
Queue q = create_queue();
enqueue(&q, 10);     // FRONT -> [10] <- BACK
enqueue(&q, 20);     // FRONT -> [10] -> [20] <- BACK
enqueue(&q, 30);     // FRONT -> [10] -> [20] -> [30] <- BACK
```

`if (q->back == NULL)` handles the empty queue case. If so, both `front` and `back` point to the new node.

`q->back->next = node` links the current back node to the new node.

`q->back = node` updates the back pointer to the new node.

`q->size++` increments the size counter.

## In Rust

```rust
impl<T> Queue<T> {
    fn enqueue(&mut self, data: T) {
        // Create a new node
        let mut node = Box::new(Node {
            data,
            next: None,
        });

        let raw = &mut *node as *mut Node<T>;

        if self.back.is_null() {
            // If the queue is empty, front and back both point to the new node
            self.front = Some(node);
            self.back = raw;
        } else {
            unsafe {
                // Point the current back to the new node
                (*self.back).next = Some(node);
            }
            // Update back to point to the new node
            self.back = raw;
        }

        self.size += 1;
    }
}

// Usage
let mut q: Queue<i32> = Queue::new();
q.enqueue(10);   // FRONT -> [10] <- BACK
q.enqueue(20);   // FRONT -> [10] -> [20] <- BACK
q.enqueue(30);   // FRONT -> [10] -> [20] -> [30] <- BACK
```

`let raw = &mut *node as *mut Node<T>` saves a raw pointer to the new node **before** we move it into the queue. Once we move `node` into `self.front` or `(*self.back).next`, we can no longer access it directly, so we save the pointer first.

`if self.back.is_null()` handles the empty queue case. If so, then both `front` and `back` point to the new node.

`(*self.back).next = Some(node)` links the current back node to the new node using the raw pointer.

`self.back = raw` updates the back pointer to the new node.

::: info Why save raw before moving node?
In Rust, once we move a value, we can no longer access it. We need `raw` to update `self.back` after moving `node` into the queue. If we tried to get the pointer after the move, the compiler would reject it. So we grab the raw pointer first, then move the node, then update `self.back`.
:::

## Key Difference

|                    | C                       | Rust                             |
| ------------------ | ----------------------- | -------------------------------- |
| Allocate node      | `malloc(sizeof(Node))`  | `Box::new(Node { ... })`         |
| Empty check        | `q->back == NULL`       | `self.back.is_null()`            |
| Link to back       | `q->back->next = node`  | `(*self.back).next = Some(node)` |
| Update back        | `q->back = node`        | `self.back = raw`                |
| Allocation failure | Returns `NULL` silently | Panics immediately               |
