# Implement a Queue Using Two Stacks

A [queue](../queue/intro.md) is a data structure that follows **FIFO** (First In, First Out), which means the first element added is the first one removed.

Our goal is to implement a queue using two stacks, which as we know follows **LIFO** (the opposite of FIFO).

## The Approach

We can use two stacks:

- **in_stack** — the inbox, where we push new elements
- **out_stack** — the outbox, where we pop elements from

When we want to queue, we just push to the `in_stack`.

When we want to dequeue, if `out_stack` is empty, we **pour** everything from `in_stack` into `out_stack`, which **reverses the order** and gives us FIFO, then we can pop from it. If `out_stack` already has elements, we just pop from it directly.

```
Enqueue 1, 2, 3:
in_stack:   [1] -> [2] -> [3]    (top is 3)
out_stack:  []

Dequeue:
out_stack is empty, pour in_stack into out_stack:
in_stack:   []
out_stack:  [3] -> [2] -> [1]   (top is 1)
pop out_stack -> 1

Enqueue 4:
in_stack:   [4]
out_stack:  [3] -> [2]

Dequeue:
out_stack is not empty, just pop:
in_stack:   [4]
out_stack:  [3] -> [2]    (top is 2)
pop out_stack -> 2

Dequeue:
in_stack:   [4]
out_stack:  [3]   (top is 3)
pop out_stack -> 3

Dequeue:
out_stack is empty, pour in_stack into out_stack:
in_stack:   []
out_stack:  [4]  (top is 4)
pop out_stack -> 4
```

## In C

```c
typedef struct {
    Stack in_stack;
    Stack out_stack;
} TwoStackQueue;

TwoStackQueue create_queue() {
    TwoStackQueue q;
    q.in_stack = create_stack();
    q.out_stack = create_stack();
    return q;
}

// Add an element to the back of the queue
void enqueue(TwoStackQueue *q, int data) {
    // Always push onto in_stack
    push(&q->in_stack, data);
}

// Remove and return the front element of the queue
int dequeue(TwoStackQueue *q) {
    // If out_stack is empty, pour in_stack into out_stack
    if (is_empty(&q->out_stack)) {
        while (!is_empty(&q->in_stack)) {
            // Pop from in_stack and push onto out_stack
            push(&q->out_stack, pop(&q->in_stack));
        }
    }

    // Pop from out_stack
    return pop(&q->out_stack);
}

// Usage
TwoStackQueue q = create_queue();
enqueue(&q, 1);     // [1]
enqueue(&q, 2);     // [1, 2]
enqueue(&q, 3);     // [1, 2, 3]

printf("%d\n", dequeue(&q));    // 1
printf("%d\n", dequeue(&q));    // 2

enqueue(&q, 4);     // [3, 4]

printf("%d\n", dequeue(&q));    // 3
printf("%d\n", dequeue(&q));    // 4
```

`enqueue()` always pushes onto `in_stack`, pretty simple.

`if (is_empty(&q->out_stack))` checks if `out_stack` is empty before pouring. If it still has elements, we don't need to pour.

`while (!is_empty(&q->in_stack))` pours all elements from `in_stack` into `out_stack`, reversing the order.

We can then `return pop(&q->out_stack)` since `out_stack` now has elements in FIFO order.

::: tip
We only pour from `in_stack` to `out_stack` when `out_stack` is empty because if we poured every time, we'd mix up the order of elements already in `out_stack`.
:::

## In Rust

```rust
struct TwoStackQueue {
    in_stack: Stack<i32>,
    out_stack: Stack<i32>,
}

impl TwoStackQueue {
    fn new() -> TwoStackQueue {
        TwoStackQueue {
            in_stack: Stack::new(),
            out_stack: Stack::new(),
        }
    }

    // Add an element to the back of the queue
    fn enqueue(&mut self, data: i32) {
        // Always push onto in_stack
        self.in_stack.push(data);
    }

    // Remove and return the front element of the queue
    fn dequeue(&mut self) -> Option<i32> {
        // If out_stack is empty, pour in_stack into out_stack
        if self.out_stack.is_empty() {
            while let Some(data) = self.in_stack.pop() {
                // Pop from in_stack and push onto out_stack
                self.out_stack.push(data);
            }
        }

        // Pop from out_stack
        self.out_stack.pop()
    }
}

// Usage
let mut q = TwoStackQueue::new();
q.enqueue(1);   // [1]
q.enqueue(2);   // [1, 2]
q.enqueue(3);   // [1, 2, 3]

println!("{:?}", q.dequeue());      // Some(1)
println!("{:?}", q.dequeue());      // Some(2)

q.enqueue(4);   // [3, 4]

println!("{:?}", q.dequeue());      // Some(3)
println!("{:?}", q.dequeue());      // Some(4)
```

`if self.out_stack.is_empty()` checks if the outbox is empty before pouring.

Just like in C, `while let Some(data) = self.in_stack.pop()` pours all elements from `in_stack` into `out_stack`. It keeps popping until `in_stack` returns `None`.

`self.out_stack.pop()` pops from `out_stack`, which now has elements in FIFO order.

::: info Why while let?
`while let Some(data) = self.in_stack.pop()` is a clean way to loop while `pop()` returns `Some` and pushes each value into out_stack. Once the stack is empty and `pop()` returns `None`, the loop stops automatically. It's the Rust equivalent of `while (!is_empty(&q->in_stack))` in C.
:::

## Complexity

| Operation | Time           | Space |
| --------- | -------------- | ----- |
| enqueue   | O(1)           | O(1)  |
| dequeue   | O(1) amortized | O(n)  |

`dequeue` is O(1) **amortized**, since each element is moved from `in_stack` to `out_stack` at most once, so the average cost per operation is O(1) even though a single dequeue can be O(n) when pouring.

## Key Difference

|             | C                                 | Rust                                         |
| ----------- | --------------------------------- | -------------------------------------------- |
| Pour loop   | `while (!is_empty(&q->in_stack))` | `while let Some(data) = self.in_stack.pop()` |
| Dequeue     | `return pop(&q->out_stack)`       | `self.out_stack.pop()`                       |
| Empty check | `is_empty(&q->out_stack)`         | `self.out_stack.is_empty()`                  |
