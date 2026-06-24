# Sort a Nearly Sorted Array

A **nearly sorted array** (or **K-sorted array**) is an array where every element is at most **K positions away** from its correct sorted position.

```
K = 2, every element is at most 2 positions from its sorted position:

Array:  [3, 2, 1, 5, 4, 7, 6]
Sorted: [1, 2, 3, 4, 5, 6, 7]
```

## The Approach

The naive approach is to just sort the entire array using a standard sorting algorithm, which would take `O(n log n)` time. But we can do better by taking advantage of the fact that the array is nearly sorted.

Since every element is at most `K` positions away, the correct element for position `i` must be somewhere in the window `[i, i + K]`. We can maintain a **min heap of size K + 1** and slide it across the array:

- Insert the first `K + 1` elements into a **min heap**
- Then pop the root into the result, which is guaranteed to be the next smallest element
- Insert the next element from the array into the heap
- We repeat until the array is exhausted, then pop the remaining elements

```
K = 2, Array: [3, 2, 1, 5, 4, 7, 6]

Step 1: insert first K + 1 = 3 elements
heap: [_, 1, 3, 2]

pop 1 -> result: [1], insert 5 -> heap: [_, 2, 3, 5]
pop 2 -> result: [1, 2], insert 4 -> heap: [_, 3, 5, 4]
pop 3 -> result: [1, 2, 3], insert 7 -> heap: [_, 4, 5, 7]
pop 4 -> result: [1, 2, 3, 4], insert 6 -> heap: [_, 5, 7, 6]
pop 5 -> result: [1, 2, 3, 4, 5], no more elements
pop 6 -> result: [1, 2, 3, 4, 5, 6]
pop 7 -> result: [1, 2, 3, 4, 5, 6, 7] ✓
```

## In C

```c
int *sort_nearly_sorted(int *arr, int n, int k, int *result_size) {
    Heap min_heap = create_heap(k + 1);

    // Step 1: insert first k + 1 elements
    int i;
    for (i = 0; i <= k && i < n; i++) {
        insert(&min_heap, arr[i]);
    }

    // Step 2: slide the window across the array
    *result_size = n;
    int *result = (int *)malloc(n * sizeof(int));
    int idx = 0;

    while (i < n) {
        result[idx++] = pop(&min_heap);
        insert(&min_heap, arr[i++]);
    }

    // Step 3: pop remaining elements
    while (min_heap.size > 0) {
        result[idx++] = pop(&min_heap);
    }

    return result;
}

// Usage
int arr[] = {3, 2, 1, 5, 4, 7, 6};
int n = 7;
int k = 2;

int result_size;
int *result = sort_nearly_sorted(arr, n, k, &result_size);

// result: [1, 2, 3, 4, 5, 6, 7]
for (int i = 0; i < result_size; i++) {
    printf("%d ", result[i]);
}
```

`create_heap(k + 1)` creates a min heap with capacity `k + 1` since we only ever hold `k + 1` elements at a time.

`for (i = 0; i <= k && i < n; i++)` inserts the first `k + 1` elements into the heap to fill the initial window.

`result[idx++] = pop(&min_heap)` pops the smallest element in the current window into the result.

`insert(&min_heap, arr[i++])` slides the window forward by inserting the next element.

The final `while` loop pops the remaining elements from the heap after the array is exhausted.

::: info Why K + 1?
We use a window of size `K + 1` instead of `K` because if an element can be at most `K` positions away, the correct next element must be within the next `K + 1` candidates. A window of size `K` would miss the element that is exactly `K` positions away.
:::

## In Rust

```rust
fn sort_nearly_sorted(arr: &[i32], k: usize) -> Vec<i32> {
    let mut min_heap = Heap::new(|a: &i32, b: &i32| a < b);

    // Step 1: insert first k + 1 elements
    for &val in arr.iter().take(k + 1) {
        min_heap.insert(val);
    }

    // Step 2: slide the window across the array
    let mut result = Vec::new();

    for &val in arr.iter().skip(k + 1) {
        if let Some(min) = min_heap.pop() {
            result.push(min);
        }
        min_heap.insert(val);
    }

    // Step 3: pop remaining elements
    while let Some(val) = min_heap.pop() {
        result.push(val);
    }

    result
}

// Usage
let arr = [3, 2, 1, 5, 4, 7, 6];
let k = 2;

let result = sort_nearly_sorted(&arr, k);
println!("{:?}", result);   // [1, 2, 3, 4, 5, 6, 7]
```

`arr.iter().take(k + 1)` takes the first `k + 1` elements from the array to fill the initial window.

`arr.iter().skip(k + 1)` skips the first `k + 1` elements since they are already in the heap.

`if let Some(min) = min_heap.pop()` pops the smallest element in the current window into the result.

`min_heap.insert(val)` slides the window forward by inserting the next element.

The final `while let` loop pops the remaining elements after the array is exhausted.

::: info What is .iter(), .take(), and .skip()?
`.iter()` creates an iterator over the array, which is a lazy sequence that produces items one at a time without copying the data. Once we have an iterator, we can chain **iterator adapters** onto it to control how we iterate. `.take(n)` yields the first `n` elements, while `.skip(n)` skips the first `n` elements and yields the rest. This makes the sliding window logic much cleaner than manually managing indices like in C.
:::

## Complexity

| Operation           | Time         | Space  |
| ------------------- | ------------ | ------ |
| Sort nearly sorted  | O(n log k)   | O(k)   |

We process all `n` elements, and each insert or pop on a heap of size `k + 1` costs `O(log k)`. Space is `O(k)` since we only keep `k + 1` elements in the heap at any time. This is much faster than `O(n log n)` when `K` is small, but if `K` approaches `n`, it degrades to `O(n log n)`.

## Key Difference

|                  | C                              | Rust                           |
| ---------------- | ------------------------------ | ------------------------------ |
| Initial window   | `for` loop up to `k + 1`         | `.iter().take(k + 1)`          |
| Slide window     | Manual index `i`               | `.iter().skip(k + 1)`          |
| Pop and insert   | `pop()` then `insert()`        | `if let Some` then `insert()`  |
| Remaining pop    | `while (size > 0)`             | `while let Some(val)`          |
| Min heap         | Separate implementation        | `Heap::new(\|a, b\| a < b)`   |
