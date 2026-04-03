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

    fn dequeue(&mut self) -> Option<T> {
        let node = self.front.take()?;

        self.front = node.next;

        if self.front.is_none() {
            self.back = std::ptr::null_mut();
        }

        self.size -= 1;

        Some(node.data)
    }
}

fn main() {
    let mut q: Queue<i32> = Queue::new();
    q.enqueue(10); // FRONT -> [10] <- BACK
    q.enqueue(20); // FRONT -> [10] -> [20] <- BACK
    q.enqueue(30); // FRONT -> [10] -> [20] -> [30] <- BACK

    let a = q.dequeue(); // Some(10), FRONT -> [20] -> [30] <- BACK
    let b = q.dequeue(); // Some(20), FRONT -> [30] <- BACK
    let c = q.dequeue(); // Some(30), FRONT -> None <- BACK

    println!("Dequeued values: {:?}, {:?}, {:?}", a, b, c); // Output: Dequeued values: Some(10), Some(20), Some(30)
}
