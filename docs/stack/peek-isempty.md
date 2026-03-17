# Peek and Is Empty

**Peek** returns the element at the top of the stack **without** removing it.

**Is empty** checks if the stack has **no elements**.

## Peek

```
TOP -> [30] -> [20] -> [10] -> NULL
        ^
      peek() returns the top without modifying the stack.
```

### In C

```c
// Return the top element without removing it
int peek(Stack *s) {
    // If the stack is empty, there is nothing to peek
    if (s->top == NULL) {
        printf("Stack is empty\n");
        return -1;
    }

    // Return the top element
    return s->top->data;
}

// Usage
Stack s = create_stack();
push(&s, 10);       // TOP -> [10] -> NULL
push(&s, 20);       // TOP -> [20] -> [10] -> NULL
push(&s, 30);       // TOP -> [30] -> [20] -> [10] -> NULL

int a = peek(&s);   // a = 30, stack unchanged
int b = peek(&s);   // b = 30, stack unchanged
```

`if (s->top == NULL)` checks if the stack is empty before peeking.

`return s->top->data` returns the data of the top node without modifying it.

::: warning
Just like `pop()`, we return `-1` when the stack is empty, which is problematic if the stack can contain negative numbers. In production code, we'd want to handle this differently.
:::

### In Rust

```rust
impl Stack {
    fn peek(&self) -> Option<i32> {
        // Return the top element
        self.top.as_ref().map(|node| node.data)
    }
}

// Usage
let mut s = Stack::new();
s.push(10);             // TOP -> [10] -> NONE
s.push(20);             // TOP -> [20] -> [10] -> NONE
s.push(30);             // TOP -> [30] -> [20] -> [10] -> NONE

let a = s.peek();       // a = Some(30), stack unchanged
let b = s.peek();       // b = Some(30), stack unchanged
```

`&self` gives the method read-only access to the stack since we're only reading, not modifying.

`self.top.as_ref()` borrows the top node without taking ownership of it.

`map(|node| node.data)` extracts the data from the node, returning `Some(data)` if it exists or `None` if the stack is empty.

::: info Why &self instead of &mut self?
Since we only want to read the top node and not modify it, we use `&self` instead of `&mut self`. Only needing a reference means that we can call `peek()` multiple times without any issues.
:::

::: info Why return `Option<&i32>` and not `Option<i32>`?
Returning `Option<&i32>` lets us read the value without moving ownership. If it returned `Option<i32>`, it would have to move the value out of the stack, which would break it.
:::

::: info Why do we need .map()?
When we call `self.top.as_ref()`, we get an `Option<&Box<Node>>`. But `peek()` needs to return an `Option<i32>`, so we use `.map()` to transform the `Option<&Box<Node>>` into an `Option<i32>`. The closure `|node| node.data` takes the reference to the node and returns its data, basically unwrapping the node while keeping the `Option` wrapper intact.

Using `.map()` here is equivalent of doing:

```rust
match self.top.as_ref() {
    Some(node) => Some(node.data),
    None => None,
}
```
:::

### Key Difference

|             | C                       | Rust                                  |
| ----------- | ----------------------- | ------------------------------------- |
| Empty check | `if (s->top == NULL)`   | Handled by `Option`                   |
| Return type | `int` (-1 on empty)     | `Option<i32>`                         |
| Access top  | `s->top->data`          | `self.top.as_ref().map(\|n\| n.data)` |
| Mutability  | `Stack *s` (mutable)    | `&self` (read-only)                   |

## Is Empty

```
TOP -> [30] -> [20] -> [10] -> NULL

is_empty() returns false (stack is not empty).

TOP -> NULL

is_empty() returns true (stack is empty).
```

### In C

```c
// Return 1 if the stack is empty, 0 otherwise
int is_empty(Stack *s) {
    return s->size == 0;
}

// Usage
Stack s = create_stack();
is_empty(&s);   // 1 (true)

push(&s, 10);
is_empty(&s);   // 0 (false)
```

`return s->size == 0` returns `1` if `size` is `0` (empty), and `0` otherwise .

::: details Alternative: Without a size field
If we didn't have a `size` field, we could check if the stack is empty by checking if `top` is `NULL`:
```c
int is_empty(Stack *s) {
    return s->top == NULL;
}
```
:::

### In Rust

```rust
impl Stack {
    // Return true if the stack is empty, false otherwise
    fn is_empty(&self) -> bool {
        self.size == 0
    }
}

// Usage
let mut s = Stack::new();
s.is_empty();   // true

s.push(10);
s.is_empty();   // false
```

`self.size == 0` returns `true` if `size` is 0 (empty), `false` otherwise.

::: details Alternative: Without a size field
Same with the C version, if we didn't have a `size` field, we could check if the stack is empty by checking if `top` is `NULL`:

```rust
fn is_empty(&self) -> bool {
    self.top.is_none()
}
```
:::

### Key Difference

|              | C                       | Rust                  |
| ------------ | ----------------------- | --------------------- |
| Empty check  | `s->size == 0`          | `self.size == 0`      |
| Return type  | `int` (1 or 0)          | `bool`                |
