# Heap Sort

Given an array of numbers, sort it in **ascending order** using a heap.

```
Array: [3, 1, 5, 2, 8]

Result: [1, 2, 3, 5, 8]
```

## The Approach

Heap sort is pretty straightforward:

- We insert each element from the array into a **min heap**
- We then pop each element out one by one into the result array
- The result is the array is guranteed to be sorted in **ascending order** since the min heap always pops the smallest element first

```
Array: [3, 1, 5, 2, 8]

Step 1: insert everything
insert 3:  [_, 3]
insert 1:  [_, 1, 3]
insert 5:  [_, 1, 3, 5]
insert 2:  [_, 1, 2, 5, 3]
insert 8:  [_, 1, 2, 5, 3, 8]

Step 2: pop everything
pop -> 1   [1]
pop -> 2   [1, 2]
pop -> 3   [1, 2, 3]
pop -> 5   [1, 2, 3, 5]
pop -> 8   [1, 2, 3, 5, 8] ✓
```

## In C

```c
int *heap_sort(int *arr, int n) {
    Heap min_heap = create_heap(n);

    // Step 1: insert everything into the min heap
    for (int i = 0; i < n; i++) {
        insert(&min_heap, arr[i]);
    }

    // Step 2: pop everything out into the result array
    int *result = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        result[i] = pop(&min_heap);
    }

    return result;
}

// Usage
int arr[] = {3, 1, 5, 2, 8};
int n = 5;

int *result = heap_sort(arr, n);

// result: [1, 2, 3, 5, 8]
for (int i = 0; i < n; i++) {
    printf("%d ", result[i]);
}
```

`create_heap(n)` creates a min heap with capacity `n` since the heap will hold all `n` elements from the array.

`insert(&min_heap, arr[i])` inserts each element into the min heap.

`result[i] = pop(&min_heap)` pops the smallest element each time, filling the result array in ascending order.

::: warning
Just like our other C implementations, `insert()` and `pop()` are hardcoded for a max heap. For heap sort we need a min heap, so you need a separate min heap implementation with the comparisons flipped.
:::

## In Rust

```rust
fn heap_sort(arr: &[i32]) -> Vec<i32> {
    let mut min_heap = Heap::new(|a: &i32, b: &i32| a < b);

    // Step 1: insert everything into the min heap
    for &val in arr {
        min_heap.insert(val);
    }

    // Step 2: pop everything out into the result array
    let mut result = Vec::new();
    while let Some(val) = min_heap.pop() {
        result.push(val);
    }

    result
}

// Usage
let arr = [3, 1, 5, 2, 8];
let result = heap_sort(&arr);

println!("{:?}", result);   // [1, 2, 3, 5, 8]
```

`Heap::new(|a: &i32, b: &i32| a < b)` creates a min heap for ascending order.

`for &val in arr` iterates over the array, inserting each element into the min heap.

`while let Some(val) = min_heap.pop()` pops elements one by one until the heap is empty.

`result.push(val)` adds each popped element to the result in sorted order.

## Complexity

| Operation | Time       | Space |
| --------- | ---------- | ----- |
| Heap sort | O(n log n) | O(n)  |

Inserting `n` elements each costs `O(log n)`, and popping `n` elements each costs `O(log n)`, giving us `O(n log n)` total. Space is `O(n)` for the heap itself.

::: info Can we do better on space?
There is an **in-place** version of heap sort that sorts the array directly without extra space, but it requires a different approach called **heapify** which builds the heap directly from the array. This brings the space complexity down to `O(1)` but is a bit more complex to implement.
:::

:::info Heap Sort vs Other Sorts
|                | Heap Sort  | Merge Sort | Quick Sort |
| -------------- | ---------- | ---------- | ---------- |
| Time (average) | O(n log n) | O(n log n) | O(n log n) |
| Time (worst)   | O(n log n) | O(n log n) | O(n²)      |
| Space          | O(n)       | O(n)       | O(log n)   |
| In-place       | No\*       | No         | Yes        |
| Stable         | No         | Yes        | No         |
| Cache friendly | No         | Yes        | Yes        |
:::

## Key Difference

|             | C                       | Rust                        |
| ----------- | ----------------------- | --------------------------- |
| Min heap    | Separate implementation | `Heap::new(\|a, b\| a < b)` |
| Insert loop | `for` loop with index   | `for &val in arr`           |
| Pop loop    | `for` loop with index   | `while let Some(val)`       |
| Descending  | Separate max heap       | Swap comparator to `a > b`  |
| Result      | Manual `malloc` array   | `Vec<i32>`                  |
