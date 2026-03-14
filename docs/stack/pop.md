# Popping an Element

Popping removes the element at the **top** of the stack and returns it. Popping an element is similar to deleting the head node of a linked list, but we also need to **return** the data the old top node contained.

```
Before: TOP -> [30] -> [20] -> [10] -> NULL

Step 1: Save the current top
TOP -> [30] -> [20] -> [10] -> NULL
        ^
       temp

Step 2: Point the top to the next node
       [30] -> [20] -> [10] -> NULL
        ^       ^
       temp    TOP

Step 3: Free temp and return its data
TOP -> [20] -> [10] -> NULL

return 30
```

## In C

```c
// Pop the top node and return its data
int pop(Stack *s) {
    // If the stack is empty, there is nothing to pop
    if (s->top == NULL) {
        printf("Stack is empty\n");
        return -1;
    }
    
    // Save the current top
    Node *temp = s->top;
    
    // Save the data to return later
    int data = temp->data;
    
    // Point top to the next node
    s->top = s->top->next;
    
    // Decrement the size
    s->size--;
    
    // Free the old top
    free(temp);
    
    // Return the popped data
    return data;
}

// Usage
Stack s = create_stack();
push(&s, 10);       // TOP -> [10] -> NULL
push(&s, 20);       // TOP -> [20] -> [10] -> NULL
push(&s, 30);       // TOP -> [30] -> [20] -> [10] -> NULL

int a = pop(&s);    // a = 30, TOP -> [20] -> [10] -> NULL
int b = pop(&s);    // b = 20, TOP -> [10] -> NULL
int c = pop(&s);    // c = 10, TOP -> NULL
int d = pop(&s);    // d = -1, stack is empty
```

`if (s->top == NULL)` checks if the stack is empty before popping.

`Node *temp = s->top` saves the top node so we can free it later.

`s->top = s->top->next` updates the top pointer to the next node.

`s->size--` decrements the size counter.

`free(temp)` deallocates the removed node to avoid a memory leak.

::: warning
Always check if the stack is empty before popping, since accessing `top` when it's `NULL` will crash the program.
:::

::: warning
We return `-1` when the stack is empty, but this can be a problem if the stack can contain **negative numbers**. In production code, we'd want to handle this differently, such as using a separate status code or passing a pointer to store the value.
:::

## In Rust

```rust
impl Stack {
    fn pop(&mut self) -> Option<i32> {
        // Take ownership of the top node, return None if the stack is empty
        let node = self.top.take()?;

        // Point top to the next node
        self.top = node.next;

        // Decrement the size
        self.size -= 1;

        // Return the popped data
        Some(node.data)
    }
}

// Usage
let mut s = Stack::new();
s.push(10);         // TOP -> [10] -> NONE
s.push(20);         // TOP -> [20] -> [10] -> NONE
s.push(30);         // TOP -> [30] -> [20] -> [10] -> NONE

let a = s.pop();    // a = Some(30), TOP -> [20] -> [10] -> NONE
let b = s.pop();    // b = Some(20), TOP -> [10] -> NONE
let c = s.pop();    // c = Some(10), TOP -> NONE
let d = s.pop();    // d = NONE, stack is empty
```

`self.top.take()?` takes ownership of the top node and returns `None` early if the stack is empty.

`self.top = node.next` updates the top pointer to the next node.

`self.size -= 1` decrements the size counter.

`Some(node.data)` returns the data wrapped in `Some`. Rust will then drop the node automatically at the end of the function.

::: info What is the ? operator?
The `?` operator is a shorthand for returning early if a value is `None`. So, instead of writing:
```rust
let node = match self.top.take() {
    Some(node) => node,
    None => return None,
};
```
We can write `self.top.take()?` and Rust handles the early return automatically. This makes the code cleaner and more concise, unlike in the C version where we have to manually check for `NULL` and return `-1`.
:::

::: tip
Our `pop()` method returns `Option<i32>`, meaning that if we want to use the value directly, we have to **unwrap** it, for example:
```rust
let a = s.pop().unwrap();   // This will panic! if the stack is empty

// To handle the empty case gracefully, we can use if let:
if let Some(value) = s.pop() {
    println!("Popped value: {}", value);
}

// Or use match/pattern matching:
match s.pop() {
    Some(value) => println!("Popped value: {}", value),
    None => println!("Stack is empty"),
}
```
:::

## Key Difference

|                 | C                       | Rust                        |
| --------------- | ----------------------- | --------------------------- |
| Empty check     | `if (s->top == NULL)`   | `?` operator returns `None` |
| Save top        | `Node *temp = s->top`   | `self.top.take()`           |
| Update top      | `s->top = s->top->next` | `self.top = node.next`      |
| Decrement size  | `s->size--`             | `self.size -= 1`            |
| Free node       | `free(temp)`            | Automatic                   |
| Return value    | `int` (-1 on empty)     | `Option<i32>`               |
