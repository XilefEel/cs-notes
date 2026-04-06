# Dequeuing an Element

Dequeuing removes the element at the **front** of the queue and returns it. Dequeuing is also similar to deleting the head node of a [linked list](../linked-list/singly/delete.md), but we also need to **return** the dequeued data and **sync** the back pointer if the queue becomes empty.

```
Before: FRONT -> [10] -> [20] -> [30] -> [40] <- BACK

Step 1: Save the front node
FRONT -> [10] -> [20] -> [30] -> [40] <- BACK
          ^
         temp

Step 2: Update front to point to the next node
         [10]    [20] -> [30] -> [40] <- BACK
          ^       ^
         temp    FRONT

Step 3: Return the saved data
return 10
```

## In C

```c
int dequeue(Queue *q) {
    // If the queue is empty, there is nothing to dequeue
    if (q->front == NULL) {
        printf("Queue is empty\n");
        return -1;
    }

    // Save the front node
    Node *temp = q->front;
    int data = temp->data;

    // Update front to point to the next node
    q->front = q->front->next;

    // If the queue is now empty, update back to NULL
    if (q->front == NULL) {
        q->back = NULL;
    }

    // Free the old front node
    free(temp);
    q->size--;

    // Return the dequeued data
    return data;
}

// Usage
Queue q = create_queue();
enqueue(&q, 10);    // FRONT -> [10] <- BACK
enqueue(&q, 20);    // FRONT -> [10] -> [20] <- BACK
enqueue(&q, 30);    // FRONT -> [10] -> [20] -> [30] <- BACK

int a = dequeue(&q);    // a = 10, FRONT -> [20] -> [30] <- BACK
int b = dequeue(&q);    // b = 20, FRONT -> [30] <- BACK
int c = dequeue(&q);    // c = 30, FRONT -> NULL, BACK -> NULL
```

`if (q->front == NULL)` checks if the queue is empty before dequeuing.

`Node *temp = q->front` saves the front node so we can free it later.

`q->front = q->front->next` updates the front pointer to point to the next node.

`if (q->front == NULL)` checks if the queue is now empty after dequeuing. If so, we must also update `back` to `NULL` to keep both pointers in sync.

`free(temp)` deallocates the removed node to avoid a memory leak.

`q->size--` decrements the size counter.

::: warning
Just like `pop()`, we return `-1` when the queue is empty. This can be a problem if the queue can contain negative numbers. In production code, we'd want to handle this differently.
:::

## In Rust

```rust
impl<T> Queue<T> {
    fn dequeue(&mut self) -> Option<T> {
        // Take ownership of the front node
        let node = self.front.take()?;

        // Update front to point to the next node
        self.front = node.next;

        // If the queue is now empty, update back to null
        if self.front.is_none() {
            self.back = std::ptr::null_mut();
        }

        self.size -= 1;

        // Return the dequeued data
        Some(node.data)
    }
}

// Usage
let mut q: Queue<i32> = Queue::new();
q.enqueue(10);  // FRONT -> [10] <- BACK
q.enqueue(20);  // FRONT -> [10] -> [20] <- BACK
q.enqueue(30);  // FRONT -> [10] -> [20] -> [30] <- BACK

let a = q.dequeue();    // Some(10), FRONT -> [20] -> [30] <- BACK
let b = q.dequeue();    // Some(20), FRONT -> [30] <- BACK
let c = q.dequeue();    // Some(30), FRONT -> None, BACK -> None
```

`self.front.take()?` takes ownership of the front node and transfers it to `node`. If the queue is empty, `?` returns `None` early.

`self.front = node.next` updates the front pointer to the next node. Since `node` owns its `next`, this transfers ownership back to `front`.

`if self.front.is_none()` checks if the queue is now empty. If so, we must also update `back` to `null_mut()` to keep both pointers in sync.

`Some(node.data)` returns the data wrapped in `Some`.

::: info Why no unsafe here?
Dequeuing only affects `front`, which is a safe `Option<Box<Node<T>>>`. We only need `unsafe` when accessing the `back` raw pointer. Setting `back` to `null_mut()` is always safe so we don't need `unsafe` for that either.
:::

## Key Difference

|              | C                           | Rust                           |
| ------------ | --------------------------- | ------------------------------ |
| Empty check  | `if (q->front == NULL)`     | `?` operator returns `None`    |
| Save front   | `Node *temp = q->front`     | `let node = self.front.take()` |
| Update front | `q->front = q->front->next` | `self.front = node.next`       |
| Sync back    | `q->back = NULL`            | `self.back = null_mut()`       |
| Free node    | `free(temp)`                | Automatic                      |
| Return value | `int` (-1 on empty)         | `Option<T>`                    |
