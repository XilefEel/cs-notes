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

    fn peek(&self) -> Option<&T> {
        self.front.as_ref().map(|node| &node.data)
    }

    fn is_empty(&self) -> bool {
        self.size == 0
    }
}

fn main() {
    let mut q: Queue<i32> = Queue::new();

    q.is_empty(); // true

    q.enqueue(10); // FRONT -> [10] <- BACK
    q.enqueue(20); // FRONT -> [10] -> [20] <- BACK
    q.enqueue(30); // FRONT -> [10] -> [20] -> [30] <- BACK

    let a = q.peek(); // a = Some(10), queue unchanged
    let b = q.peek(); // b = Some(10), queue unchanged

    println!("Peeked values: {:?}, {:?}", a, b); // Output: Peeked values: Some(10), Some(10)

    q.is_empty(); // false

    let d = q.dequeue(); // Some(10), FRONT -> [20] -> [30] <- BACK
    let e = q.dequeue(); // Some(20), FRONT -> [30] <- BACK
    let f = q.dequeue(); // Some(30), FRONT -> None <- BACK

    println!("Dequeued values: {:?}, {:?}, {:?}", d, e, f); // Output: Dequeued values: Some(10), Some(20), Some(30)
}
