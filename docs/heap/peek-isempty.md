# Peek and Is Empty

Peeking and checking if a heap is empty are very similar to the same operations in a [stack](../stack/peek-isempty.md) or [queue](../queue/peek-isempty.md). The main difference is that instead of looking at the top element, we look at the root element of the heap.

## Peek

```
Heap: [_, 9, 3, 8, 1, 2, 5]
          ^
        peek() returns the root without modifying the heap.
```

### In C

```c
int peek(Heap *heap) {
    // If the heap is empty, there is nothing to peek
    if (heap->size == 0) {
        printf("Heap is empty\n");
        return -1;
    }

    // Return the root element
    return heap->data[1];
}

// Usage
Heap max_heap = create_heap(10);
insert(&max_heap, 5);
insert(&max_heap, 3);
insert(&max_heap, 9); // [_, 9, 3, 5]

int a = peek(&max_heap);    // a = 9, heap unchanged
int b = peek(&max_heap);    // b = 9, heap unchanged
```

`if (heap->size == 0)` checks if the heap is empty before peeking.

`return heap->data[1]` returns the root element at index `1` without removing it.

::: warning
Just like `pop()`, we return `-1` when the heap is empty. This can be a problem if the heap contains negative numbers. In production code you might want to handle this differently, such as returning a special error code.
:::

### In Rust

```rust
impl<T: Default + PartialOrd, F: Fn(&T, &T) -> bool> Heap<T, F> {
    fn peek(&self) -> Option<&T> {
        // Return the root element
        if self.data.len() <= 1 {
            return None;
        }
        Some(&self.data[1])
    }
}

// Usage
let mut max_heap = Heap::new(|a: &i32, b: &i32| a > b);
max_heap.insert(5);
max_heap.insert(3);
max_heap.insert(9);     // [_, 9, 3, 5]

let a = max_heap.peek();    // a = Some(9), heap unchanged
let b = max_heap.peek();    // b = Some(9), heap unchanged
```

`if self.data.len() <= 1` checks if the heap is empty before peeking. Since index `0` is a dummy value, the heap is empty if the length is `1` or less.

`Some(&self.data[1])` returns a reference to the root element at index `1` without removing it.

::: info Why return a reference?
`peek()` returns `Option<&T>` instead of `Option<T>` because we don't know if `T` is cheap to copy. Returning a reference lets the caller read the value without taking ownership or making unnecessary copies, regardless of what `T` is.
:::

### Key Difference

|              | C                      | Rust                  |
| ------------ | ---------------------- | --------------------- |
| Empty check  | `heap->size == 0`      | `self.data.len() <= 1`|
| Return type  | `int` (-1 on empty)    | `Option<&T>`          |
| Access root  | `heap->data[1]`        | `&self.data[1]`       |

## Is Empty

```
Heap: [_, 9, 3, 8, 1, 2, 5]

is_empty() returns false (heap is not empty).

Heap: [_]

is_empty() returns true (heap is empty).
```

### In C

```c
int is_empty(Heap *heap) {
    return heap->size == 0;
}

// Usage
Heap max_heap = create_heap(10);
is_empty(&max_heap);    // 1 (true)

insert(&max_heap, 9);
is_empty(&max_heap);    // 0 (false)
```

`return heap->size == 0` returns `1` if `size` is `0` (empty), and `0` otherwise.

### In Rust

```rust
impl<T: Default + PartialOrd, F: Fn(&T, &T) -> bool> Heap<T, F> {
    fn is_empty(&self) -> bool {
        self.data.len() <= 1
    }
}

// Usage
let mut max_heap = Heap::new(|a: &i32, b: &i32| a > b);
max_heap.is_empty();    // true

max_heap.insert(9);
max_heap.is_empty();    // false
```

`self.data.len() <= 1` returns `true` if the heap is empty (length is `1` or less), and `false` otherwise.

### Key Difference

|              | C                  | Rust                   |
| ------------ | ------------------ | ---------------------- |
| Empty check  | `heap->size == 0`  | `self.data.len() <= 1` |
| Return type  | `int` (1 or 0)     | `bool`                 |
