struct Heap<T: Default, F: Fn(&T, &T) -> bool> {
    data: Vec<T>,
    comparator: F,
}

impl<T: Default, F: Fn(&T, &T) -> bool> Heap<T, F> {
    fn new(comparator: F) -> Self {
        let mut data = Vec::new();
        data.push(T::default());

        Heap { data, comparator }
    }
}

fn main() {
    let min_heap = Heap::new(|a: &i32, b: &i32| a < b);
    let max_heap = Heap::new(|a: &i32, b: &i32| a > b);
}
