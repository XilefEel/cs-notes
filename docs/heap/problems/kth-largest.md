# Kth Largest Element

Given an array of numbers and a number `K`, find the **Kth largest element** in the array.

```
Array: [3, 1, 5, 12, 2, 8]
K = 2

Sorted descending: [12, 8, 5, 3, 2, 1]
                        ^
                    2nd largest = 8
```

## The Approach

The naive approach is to sort the array in descending order and return the element at index `K-1`. This works but has a `O(n log n)` time complexity.

The better approach is to maintain a **min heap of size K**:

- Insert all the elements one by one
- Whenever the heap grows beyond size `K`, we pop the smallest element out
- At the end, the root is the guaranteed to be the Kth largest element
 
The reason why is because if we always keep only the **K largest elements** seen so far, the smallest of those (the root of the min heap) is by definition the Kth largest.

```
K = 2, Array: [3, 1, 5, 12, 2, 8]

insert 3:   [_, 3]          size = 1,
insert 1:   [_, 1, 3]       size = 2,
insert 5:   [_, 1, 3, 5]    size = 3, pop min -> [_, 3, 5]
insert 12:  [_, 3, 5, 12]   size = 3, pop min -> [_, 5, 12]
insert 2:   [_, 2, 12, 5]   size = 3, pop min -> [_, 5, 12]
insert 8:   [_, 5, 12, 8]   size = 3, pop min -> [_, 8, 12]

root = 8, which is the 2nd largest ✓
```

## In C

```c
int kth_largest(int *arr, int n, int k) {
    Heap min_heap = create_heap(k);

    // Iterate through the array
    for (int i = 0; i < n; i++) {
        insert(&min_heap, arr[i]);

        // If heap grows beyond k, pop the smallest element
        if (min_heap.size > k) {
            pop(&min_heap);
        }
    }

    // Root is the Kth largest element
    return peek(&min_heap);
}

// Usage
int arr[] = {3, 1, 5, 12, 2, 8};
int n = 6;
int k = 2;

int result = kth_largest(arr, n, k);
printf("%d\n", result);  // 8
```

`create_heap(k)` creates a min heap with capacity `k`.

`insert(&min_heap, arr[i])` inserts the current element into the min heap.

We check if the heap has grown beyond `k` elements with `if (min_heap.size > k)`. If so
we use `pop(&min_heap)` to remove the smallest element, keeping only the `k` largest elements seen so far.

`peek(&min_heap)` gives us the root of the min heap, which is guranteed to be the Kth largest element.

::: info Why use a min heap instead of a max heap?
It might seem counterintuitive to use a min heap to find the Kth largest. But by keeping only the `K` largest elements in a min heap, the root is always the smallest of those `K` elements, which is exactly the Kth largest. A max heap would give us the largest element instantly but wouldn't help us track the Kth largest efficiently.
:::

::: warning
In C, our `insert()`, `pop()`, and `peek()` implementations are hardcoded for a max heap. To use them as a min heap here, you need a separate min heap implementation with the comparisons flipped. This is one of the key advantages of the Rust approach with closures.
:::

## In Rust

```rust
fn kth_largest(arr: &[i32], k: usize) -> Option<i32> {
    let mut min_heap = Heap::new(|a: &i32, b: &i32| a < b);

    // Iterate through the array
    for &val in arr {
        min_heap.insert(val);

        // If heap grows beyond k, pop the smallest element
        if min_heap.data.len() - 1 > k {
            min_heap.pop();
        }
    }

    // Root is the Kth largest element
    min_heap.peek().copied()
}

// Usage
let arr = [3, 1, 5, 12, 2, 8];
let k = 2;

let result = kth_largest(&arr, k);
println!("{:?}", result);   // Some(8)
```

`Heap::new(|a: &i32, b: &i32| a < b)` creates a min heap by passing a comparator that returns `true` when the left element is smaller.

`min_heap.data.len() - 1 > k` checks if the heap has grown beyond `k` elements. We have to subtract `1` to account for the dummy value at index `0`.

The `.copied()` after `min_heap.peek()` converts the `Option<&i32>` to `Option<i32>` since `i32` is cheap to copy.

:::info What is .copied()?
`.copied()` is a method on `Option<&T>` that converts it to `Option<T>` by copying the value inside. It is equivalent to `.map(|x| *x)`, it just dereferences the reference and wraps it back in `Some<T>`. It only works when `T` implements `Copy`, which `i32` does since it is a primitive type.
:::

## Complexity

| Operation       | Time         | Space |
| --------------- | ------------ | ----- |
| Kth largest     | O(n log k)   | O(k)  |

Each of the `n` elements is inserted into a heap of size at most `k`, and each insert or pop costs `O(log k)`. Space is `O(k)` since we only keep `k` elements in the heap at any time.

## Key Difference

|                  | C                          | Rust                             |
| ---------------- | -------------------------- | -------------------------------- |
| Min heap         | Separate implementation    | `Heap::new(\|a, b\| a < b)`      |
| Size check       | `min_heap.size > k`        | `min_heap.data.len() - 1 > k`    |
| Return type      | `int`                      | `Option<i32>`                    |
