struct Node {
    data: i32,
    next: Option<Box<Node>>,
}

struct Stack {
    top: Option<Box<Node>>,
    size: usize,
}

#[allow(dead_code)]
impl Stack {
    fn new() -> Stack {
        Stack { top: None, size: 0 }
    }

    fn push(&mut self, data: i32) {
        let node = Box::new(Node {
            data,
            next: self.top.take(),
        });
        self.top = Some(node);
        self.size += 1;
    }

    fn pop(&mut self) -> Option<i32> {
        let node = self.top.take()?;
        self.top = node.next;
        self.size -= 1;
        Some(node.data)
    }

    fn peek(&self) -> Option<i32> {
        self.top.as_ref().map(|node| node.data)
    }

    fn is_empty(&self) -> bool {
        self.size == 0
    }
}

fn main() {
    let mut s = Stack::new();

    s.push(10); // TOP -> [10] -> NONE
    s.push(20); // TOP -> [20] -> [10] -> NONE
    s.push(30); // TOP -> [30] -> [20] -> [10] -> NONE

    let a = s.pop(); // a = Some(30), TOP -> [20] -> [10] -> NONE
    let b = s.pop(); // b = Some(20), TOP -> [10] -> NONE
    let c = s.pop(); // c = Some(10), TOP -> NONE
    let d = s.pop(); // d = NONE, stack is empty

    s.is_empty(); // true

    if let Some(value) = s.pop() {
        println!("Popped value: {}", value);
    }

    match s.pop() {
        Some(value) => println!("Popped value: {}", value),
        None => println!("Stack is empty"),
    }

    s.push(10); // TOP -> [10] -> NONE
    s.push(20); // TOP -> [20] -> [10] -> NONE
    s.push(30); // TOP -> [30] -> [20] -> [10] -> NONE

    let e = s.peek(); // e = Some(30), TOP -> [30] -> [20] -> [10] -> NONE
    let f = s.peek(); // f = Some(30), TOP -> [30] -> [20] -> [10] -> NONE

    s.is_empty(); // false
}
