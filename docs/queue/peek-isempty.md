# Peek and Is Empty

Peeking and checking if empty in a queue are similar to peeking and checking if empty in a [stack](../stack/peek-isempty.md).

**Peek** returns the element at the **front** of the queue **without** removing it.

**Is empty** checks if the queue has **no elements**.

## Peek

```
FRONT -> [10] -> [20] -> [30] <- BACK
          ^
        peek() returns the front without modifying the queue.
```

### In C

```c
int peek(Queue *q) {
    // If the queue is empty, there is nothing to peek
    if (q->front == NULL) {
        printf("Queue is empty\n");
        return -1;
    }

    // Return the front element
    return q->front->data;
}

// Usage
Queue q = create_queue();
enqueue(&q, 10);    // FRONT -> [10] <- BACK
enqueue(&q, 20);    // FRONT -> [10] -> [20] <- BACK
enqueue(&q, 30);    // FRONT -> [10] -> [20] -> [30] <- BACK

int a = peek(&q);   // a = 10, queue unchanged
int b = peek(&q);   // b = 10, queue unchanged
```

`if (q->front == NULL)` checks if the queue is empty before peeking.

`return q->front->data` returns the data of the front node without removing it.

::: warning
Just like `dequeue()`, we return `-1` when the queue is empty. This can be a problem if the queue can contain negative numbers. Again, we'd want to handle this differently in production code.
:::

### In Rust

```rust
impl<T> Queue<T> {
    fn peek(&self) -> Option<&T> {
        // Return the front element
        self.front.as_ref().map(|node| &node.data)
    }
}

// Usage
let mut q: Queue<i32> = Queue::new();
q.enqueue(10);          // FRONT -> [10] <- BACK
q.enqueue(20);          // FRONT -> [10] -> [20] <- BACK
q.enqueue(30);          // FRONT -> [10] -> [20] -> [30] <- BACK

let a = q.peek();       // a = Some(10), queue unchanged
let b = q.peek();       // b = Some(10), queue unchanged
```

`&self` gives the method read-only access to the queue since we're only reading, not modifying, just like in the stack's `peek()`.

`self.front.as_ref().map(|node| &node.data)` borrows the front node and extracts its data, returning `Some(&data)` or `None` if the queue is empty.

::: info Why return a reference?
`peek()` returns `Option<&T>` instead of `Option<T>` because we don't know if `T` is cheap to copy. Returning a reference lets the caller read the value without taking ownership or making unnecessary copies, regardless of what `T` is.
:::

::: tip
Since `peek()` returns a reference, you need to dereference it to use the value directly:

```rust
// Using if let
if let Some(value) = q.peek() {
    println!("{}", value);  // Rust auto-derefs here
}

// Or dereference manually
let a = *q.peek().unwrap();  // dereferences the &T to get T
```

Note that `println!` and most Rust operations auto-dereference, so in practice you rarely need the explicit `*`.
:::

### Key Difference

|              | C                       | Rust                                     |
| ------------ | ----------------------- | ---------------------------------------- |
| Empty check  | `if (q->front == NULL)` | Handled by `Option`                      |
| Return type  | `int` (-1 on empty)     | `Option<&T>`                             |
| Access front | `q->front->data`        | `self.front.as_ref().map(\|n\| &n.data)` |

## Is Empty

```
FRONT -> [10] -> [20] -> [30] <- BACK

is_empty() returns false (queue is not empty).

FRONT -> NULL

is_empty() returns true (queue is empty).
```

### In C

```c
// Return 1 if the queue is empty, 0 otherwise
int is_empty(Queue *q) {
    return q->size == 0;
}

// Usage
Queue q = create_queue();
is_empty(&q);   // 1 (true)

enqueue(&q, 10);
is_empty(&q);   // 0 (false)
```

`return q->size == 0` returns `1` if `size` is `0` (empty), and `0` otherwise.

::: details Alternative: Without a size field
If we didn't have a `size` field, we could check if the queue is empty by checking if `front` is `NULL`:

```c
int is_empty(Queue *q) {
    return q->front == NULL;
}
```

:::

### In Rust

```rust
impl<T> Queue<T> {
    fn is_empty(&self) -> bool {
        self.size == 0
    }
}

// Usage
let mut q: Queue<i32> = Queue::new();
q.is_empty();   // true

q.enqueue(10);
q.is_empty();   // false
```

`self.size == 0` returns `true` if the queue is empty, `false` otherwise.

::: details Alternative: Without a size field
If we didn't have a `size` field, we could check if the queue is empty by checking if `front` is `None`:

```rust
fn is_empty(&self) -> bool {
    self.front.is_none()
}
```

:::

### Key Difference

|             | C              | Rust             |
| ----------- | -------------- | ---------------- |
| Empty check | `q->size == 0` | `self.size == 0` |
| Return type | `int` (1 or 0) | `bool`           |
