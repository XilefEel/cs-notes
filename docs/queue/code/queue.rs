struct Node<T> {
    data: T,
    next: Option<Box<Node<T>>>,
}

struct Queue<T> {
    front: Option<Box<Node<T>>>,
    back: *mut Node<T>,
    size: usize,
}

impl<T> Queue<T> {
    fn new() -> Queue<T> {
        Queue {
            front: None,
            back: std::ptr::null_mut(),
            size: 0,
        }
    }

    fn enqueue(&mut self, data: T) {
        let mut node = Box::new(Node { data, next: None });

        let raw = &mut *node as *mut Node<T>;

        if self.back.is_null() {
            self.front = Some(node);
            self.back = raw;
        } else {
            unsafe {
                (*self.back).next = Some(node);
            }

            self.back = raw;
        }

        self.size += 1;
    }
}

fn main() {
    let mut q: Queue<i32> = Queue::new();
    q.enqueue(10); // FRONT -> [10] <- BACK
    q.enqueue(20); // FRONT -> [10] -> [20] <- BACK
    q.enqueue(30); // FRONT -> [10] -> [20] -> [30] <- BACK
}
