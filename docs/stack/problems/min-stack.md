# Min Stack

A min stack is a stack that supports all regular stack operations, but also allows us to get the minimum element in **O(1) time**.

Getting the minimum element in a regular stack would be **O(n)**, since we have to traverse the entire stack to find the minimum. But we want **O(1) time**, so we need a different approach.

## The Approach

We create a second stack that **tracks** the minimum element. Every time we push a value to our main stack, we also push it onto the min stack if it's **smaller than or equal** to the current minimum. When we pop from the main stack, we also pop from the min stack if the popped value was the **minimum**.

```
Push 3:
stack:     [3]
min_stack: [3]

Push 5:
stack:     [3] -> [5]
min_stack: [3]                      (3 <= 5, don't push)

Push 2:
stack:     [3] -> [5] -> [2]
min_stack: [3] -> [2]               (2 <= 3, push)

Push 1:
stack:     [3] -> [5] -> [2] -> [1]
min_stack: [3] -> [2] -> [1]        (1 <= 2, push)

getMin() -> 1

Pop:
stack:     [3] -> [5] -> [2]
min_stack: [3] -> [2]               (1 == min, pop)

getMin() -> 2
```

## In C

```c
typedef struct {
    Stack stack;
    Stack min_stack;
} MinStack;

MinStack create_min_stack() {
    MinStack ms;
    ms.stack = create_stack();
    ms.min_stack = create_stack();
    return ms;
}

void min_push(MinStack *ms, int data) {
    // Always push onto the main stack
    push(&ms->stack, data);

    // Push onto min stack if it's empty or data is <= current minimum
    if (is_empty(&ms->min_stack) || data <= peek(&ms->min_stack)) {
        push(&ms->min_stack, data);
    }
}

int min_pop(MinStack *ms) {
    // Always pop the main stack
    int data = pop(&ms->stack);

    // Pop from min stack too if the popped value is the current minimum
    if (data == peek(&ms->min_stack)) {
        pop(&ms->min_stack);
    }

    return data;
}

int get_min(MinStack *ms) {
    if (is_empty(&ms->min_stack)) return -1; // or some sentinel value
    
    return peek(&ms->min_stack);
}

// Usage
MinStack ms = create_min_stack();
min_push(&ms, 3);   // stack: [3],          min: [3]
min_push(&ms, 5);   // stack: [3, 5],       min: [3]
min_push(&ms, 2);   // stack: [3, 5, 2],    min: [3, 2]
min_push(&ms, 1);   // stack: [3, 5, 2, 1], min: [3, 2, 1]

printf("%d\n", get_min(&ms));   // 1

min_pop(&ms);                   // stack: [3, 5, 2],    min: [3, 2]
printf("%d\n", get_min(&ms));   // 2

min_pop(&ms);                   // stack: [3, 5],       min: [3]
printf("%d\n", get_min(&ms));   // 3
```

`MinStack` wraps two stacks into one struct. `stack` is our main stack and `min_stack` tracks the minimums.

`if (is_empty(&ms->min_stack) || data <= peek(&ms->min_stack))` pushes onto the min stack if the new value is smaller than or equal to the current minimum.

`if (data == peek(&ms->min_stack))` pops from the min stack too if the popped value was the current minimum.

`get_min()` peeks at the top of the min stack, which is always the current minimum.

::: info Why <= and not < ?
If we pushed two equal minimums, like `2` and `2`, and used `<`, we'd only push the first `2` onto the min stack. When we pop the second `2`, we'd also pop the only `2` from the min stack. Since the first `2` is still in the main stack, we'd have the **wrong** minimum. Using `<=` ensures we track all occurrences of the minimum value.
:::

## In Rust

```rust
struct MinStack {
    stack: Stack<i32>,
    min_stack: Stack<i32>,
}

impl MinStack {
    fn new() -> MinStack {
        MinStack {
            stack: Stack::<i32>::new(),
            min_stack: Stack::<i32>::new(),
        }
    }

    fn push(&mut self, data: i32) {
        // Always push onto the main stack
        self.stack.push(data);

        // Push onto min stack if it's empty or data is <= current minimum
        if self.min_stack.is_empty() || data <= *self.min_stack.peek().unwrap() {
            self.min_stack.push(data);
        }
    }

    fn pop(&mut self) -> Option<i32> {
        // Always pop the main stack
        let data = self.stack.pop()?;

        // Pop from min stack too if the popped value is the current minimum
        if Some(&data) == self.min_stack.peek() {
            self.min_stack.pop();
        }

        Some(data)
    }

    fn get_min(&self) -> Option<&i32> {
        self.min_stack.peek()
    }
}

// Usage
let mut ms = MinStack::new();
ms.push(3);     // stack: [3],          min: [3]
ms.push(5);     // stack: [3, 5],       min: [3]
ms.push(2);     // stack: [3, 5, 2],    min: [3, 2]
ms.push(1);     // stack: [3, 5, 2, 1], min: [3, 2, 1]

println!("{:?}", ms.get_min());   // Some(1)

ms.pop();       // stack: [3, 5, 2],    min: [3, 2]
println!("{:?}", ms.get_min());   // Some(2)

ms.pop();       // stack: [3, 5],       min: [3]
println!("{:?}", ms.get_min());   // Some(3)
```

Just like in C, `MinStack` contains two stacks. `stack` is for the main values and `min_stack` is for tracking minimums.

`data <= *self.min_stack.peek().unwrap()` uses `*` to dereference the current minimum, since `peek()` returns `Option<&i32>`.

`if Some(&data) == self.min_stack.peek()` wraps `data` in `Some(&data)` to match `Option<&i32>` that `peek()` returns.


## Complexity

| Operation | Time | Space |
| --------- | ---- | ----- |
| push      | O(1) | O(1)  |
| pop       | O(1) | O(1)  |
| get_min   | O(1) | O(1)  |

The min stack uses **O(n)** space in the worst case, since we could push `n` elements that are all the same value or strictly decreasing. But each operation runs in **O(1)** time.

## Key Differences

|                 | C                                      | Rust                                   |
| --------------- | -------------------------------------- | -------------------------------------- |
| Two stacks      | `Stack stack, min_stack`               | `Stack<i32> stack, min_stack`          |
| Peek min        | `peek(&ms->min_stack)`                 | `self.min_stack.peek()`                |
| Compare min     | `data == peek(&ms->min_stack)`         | `Some(&data) == self.min_stack.peek()` |
| Get min         | `int` (returns -1 on empty)            | `Option<&i32>`                         |
