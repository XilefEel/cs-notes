
# Creating a Heap

To create a heap, we need two things:

- A **dynamic array** to store the elements
- A **comparator** to determine the order of elements

An empty heap is simply a heap with an empty array and a comparator that defines whether it behaves as a max heap or a min heap.

## In C

```c
typedef struct {
    int *data;      // a dynamic array to store elements
    int size;       // the current number of elements
    int capacity;   // the maximum number of elements
} Heap;

// Creates a new empty heap
Heap create_heap(int capacity) {
    Heap h;
    h.data = (int *)malloc((capacity + 1) * sizeof(int));
    h.size = 0;
    h.capacity = capacity;
    return h;
}

// Usage
Heap max_heap = create_heap(10);
Heap min_heap = create_heap(10);
```

`data` is a dynamically allocated array that stores the heap elements. 

`size` tracks how many elements are currently in the heap.

`capacity` tracks how large the array is before we need to resize it.

`create_heap()` returns an empty heap with `size = 0` and an allocated array.

::: info Why capacity + 1?
Since we're using 1-based indexing, the root lives at index `1` and index `0` is unused. We allocate one extra slot so index `capacity` is still valid without going out of bounds.
:::

::: warning
In C, we don't have a built-in comparator like in Rust. This means for max heap and min heap, the comparison logic is handled separately in each operation. We will see this in the insert and delete pages.
:::

## In Rust

This is where things get interesting. Instead of two separate structs for max heap and min heap, we can use a single struct with **a comparator closure**. The comparator is a function that takes two references and returns a `bool`, deciding which element has higher priority.

```rust
struct Heap<T: Default, F: Fn(&T, &T) -> bool> {
    data: Vec<T>,
    comparator: F,
}

impl<T: Default, F: Fn(&T, &T) -> bool> Heap<T, F> {
    fn new(comparator: F) -> Self {
        let mut data = Vec::new();
        data.push(T::default());    // index 0 unused
        
        Heap {
            data,
            comparator,
        }
    }
}

// Usage
let mut max_heap = Heap::new(|a: &i32, b: &i32| a > b);
let mut min_heap = Heap::new(|a: &i32, b: &i32| a < b);
```

`data` is a `Vec<T>` that stores the heap elements. Index 0 is occupied by a dummy default value and is never used.

`data.push(T::default())` inserts a dummy value at index 0 to keep the 1-based indexing consistent with our C version.

The `Default` in `T: Default` ensures `T` has a default value we can use as the dummy at index 0.

`comparator` is a closure (a type of function) stored in the struct that will define the heap order. 

`comparator` has the trait bound `F: Fn(&T, &T) -> bool` that says it must be a function that takes two references and returns a `bool`.

`Heap::new()` takes a comparator closure and returns an empty heap. `|a: &i32, b: &i32| a > b` creates a max heap, while `|a: &i32, b: &i32| a < b` creates a min heap.

::: info What is a default value?
A default value is a value that can be created without any input. For example, the default value for `i32` is `0`, for `String` is an empty string, and for `Vec<T>` is an empty vector. If we want to use a custom struct, we can implement the `Default` trait for it to define what its default value should be
:::

::: info Why use a closure instead of two separate structs?
We could have separate `MaxHeap` and `MinHeap` structs, but they would share identical code differing only the comparison. By storing the comparator as a closure, one struct can handle both cases.
:::

## Key Difference

|                   | C                            | Rust                          |
| ----------------- | ---------------------------- | ----------------------------- |
| Storage           | `int *data` (manual)         | `Vec<T>` (automatic)          |
| Size tracking     | Manual `size` field          | `data.len() - 1`              |
| Capacity tracking | Manual `capacity` field      | `data.capacity()`             |
| Heap type         | Separate logic per operation | Comparator closure in struct  |
| Empty heap        | `size = 0`                   | `vec![T::default()]`          |
