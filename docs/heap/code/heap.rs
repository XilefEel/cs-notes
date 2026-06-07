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
}

fn main() {
    let mut max_heap = Heap::new(|a: &i32, b: &i32| a > b);

    max_heap.insert(5); // [_, 5]
    max_heap.insert(3); // [_, 5, 3]
    max_heap.insert(8); // [_, 8, 3, 5]
    max_heap.insert(1); // [_, 8, 3, 5, 1]
    max_heap.insert(2); // [_, 8, 3, 5, 1, 2]
    max_heap.insert(9); // [_, 9, 3, 8, 1, 2, 5]

    for i in 1..max_heap.data.len() {
        println!("{}", max_heap.data[i]);
    }

    let mut min_heap = Heap::new(|a: &i32, b: &i32| a < b);

    min_heap.insert(5); // [_, 5]
    min_heap.insert(3); // [_, 3, 5]
    min_heap.insert(8); // [_, 3, 5, 8]
    min_heap.insert(1); // [_, 1, 3, 8, 5]
    min_heap.insert(2); // [_, 1, 2, 8, 5, 3]
    min_heap.insert(9); // [_, 1, 2, 8, 5, 3, 9]

    for i in 1..min_heap.data.len() {
        println!("{}", min_heap.data[i]);
    }
}
