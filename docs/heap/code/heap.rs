struct Heap<T: Default + PartialOrd, F: Fn(&T, &T) -> bool> {
    data: Vec<T>,
    comparator: F,
}

impl<T: Default + PartialOrd, F: Fn(&T, &T) -> bool> Heap<T, F> {
    fn new(comparator: F) -> Self {
        let mut data = Vec::new();
        data.push(T::default());

        Heap { data, comparator }
    }

    fn parent_index(i: usize) -> usize {
        i / 2
    }

    fn left_child_index(i: usize) -> usize {
        2 * i
    }

    fn right_child_index(i: usize) -> usize {
        2 * i + 1
    }

    fn upheap(&mut self, index: usize) {
        if index <= 1 {
            return;
        }

        let parent_index = Self::parent_index(index);

        if !(self.comparator)(&self.data[index], &self.data[parent_index]) {
            return;
        }

        self.data.swap(index, parent_index);
        self.upheap(parent_index);
    }

    fn insert(&mut self, value: T) {
        self.data.push(value);
        let last = self.data.len() - 1;
        self.upheap(last);
    }

    fn downheap(&mut self, index: usize) {
        if Self::left_child_index(index) >= self.data.len() {
            return;
        }

        let left_child_index = Self::left_child_index(index);
        let right_child_index = Self::right_child_index(index);

        let mut priority_index = index;

        if (self.comparator)(&self.data[left_child_index], &self.data[priority_index]) {
            priority_index = left_child_index;
        }

        if right_child_index < self.data.len()
            && (self.comparator)(&self.data[right_child_index], &self.data[priority_index])
        {
            priority_index = right_child_index;
        }

        if priority_index == index {
            return;
        }

        self.data.swap(index, priority_index);
        self.downheap(priority_index);
    }

    fn pop(&mut self) -> Option<T> {
        if self.data.len() <= 1 {
            return None;
        }

        let last = self.data.len() - 1;
        self.data.swap(1, last);
        let removed = self.data.pop();

        self.downheap(1);

        removed
    }

    fn peek(&self) -> Option<&T> {
        if self.data.len() <= 1 {
            return None;
        }
        Some(&self.data[1])
    }

    fn is_empty(&self) -> bool {
        self.data.len() <= 1
    }
}

fn kth_largest(arr: &[i32], k: usize) -> Option<i32> {
    let mut min_heap = Heap::new(|a: &i32, b: &i32| a < b);

    for &val in arr {
        min_heap.insert(val);

        if min_heap.data.len() - 1 > k {
            min_heap.pop();
        }
    }

    min_heap.peek().copied()
}

fn heap_sort(arr: &[i32]) -> Vec<i32> {
    let mut min_heap = Heap::new(|a: &i32, b: &i32| a < b);

    for &val in arr {
        min_heap.insert(val);
    }

    let mut result = Vec::new();
    while let Some(val) = min_heap.pop() {
        result.push(val);
    }

    result
}

fn main() {
    let mut max_heap = Heap::new(|a: &i32, b: &i32| a > b);

    max_heap.is_empty(); // true

    max_heap.insert(5); // [_, 5]
    max_heap.insert(3); // [_, 5, 3]
    max_heap.insert(8); // [_, 8, 3, 5]
    max_heap.insert(1); // [_, 8, 3, 5, 1]
    max_heap.insert(2); // [_, 8, 3, 5, 1, 2]
    max_heap.insert(9); // [_, 9, 3, 8, 1, 2, 5]

    max_heap.is_empty(); // false

    max_heap.peek(); // returns Some(9)

    max_heap.pop(); // returns Some(9), heap: [_, 8, 3, 5, 1, 2]
    max_heap.pop(); // returns Some(8), heap: [_, 5, 3, 2, 1]
    max_heap.pop(); // returns Some(5), heap: [_, 3, 1, 2]

    let mut min_heap = Heap::new(|a: &i32, b: &i32| a < b);

    min_heap.insert(5); // [_, 5]
    min_heap.insert(3); // [_, 3, 5]
    min_heap.insert(8); // [_, 3, 5, 8]
    min_heap.insert(1); // [_, 1, 3, 8, 5]
    min_heap.insert(2); // [_, 1, 2, 8, 5, 3]
    min_heap.insert(9); // [_, 1, 2, 8, 5, 3, 9]

    min_heap.pop(); // returns Some(1), heap: [_, 2, 3, 8, 5, 9]
    min_heap.pop(); // returns Some(2), heap: [_, 3, 5, 8, 9]
    min_heap.pop(); // returns Some(3), heap: [_, 5, 9, 8]

    let arr = [3, 1, 5, 12, 2, 8];
    let k = 2;

    let result = kth_largest(&arr, k);
    println!("{:?}", result); // Some(8)

    let unsorted = [3, 1, 5, 2, 8];
    let sorted = heap_sort(&unsorted);

    println!("{:?}", sorted); // [1, 2, 3, 5, 8]
}
