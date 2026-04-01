struct Node<T> {
    data: T,
    next: Option<Box<Node<T>>>,
}

struct Stack<T> {
    top: Option<Box<Node<T>>>,
    size: usize,
}

impl<T> Stack<T> {
    fn new() -> Stack<T> {
        Stack { top: None, size: 0 }
    }

    fn push(&mut self, data: T) {
        let node = Box::new(Node {
            data,
            next: self.top.take(),
        });
        self.top = Some(node);
        self.size += 1;
    }

    fn pop(&mut self) -> Option<T> {
        let node = self.top.take()?;
        self.top = node.next;
        self.size -= 1;
        Some(node.data)
    }

    fn peek(&self) -> Option<&T> {
        self.top.as_ref().map(|node| &node.data)
    }

    fn is_empty(&self) -> bool {
        self.size == 0
    }
}

fn is_balanced(str: &str) -> bool {
    let mut s: Stack<char> = Stack::new();

    for c in str.chars() {
        match c {
            '(' | '[' | '{' => s.push(c),
            ')' | ']' | '}' => match s.pop() {
                None => return false,
                Some(top) => {
                    if (c == ')' && top != '(')
                        || (c == ']' && top != '[')
                        || (c == '}' && top != '{')
                    {
                        return false;
                    }
                }
            },
            _ => {}
        }
    }

    s.is_empty()
}

struct MinStack {
    stack: Stack<i32>,
    min_stack: Stack<i32>,
}

impl MinStack {
    fn new() -> MinStack {
        MinStack {
            stack: Stack::<i32>::new(),
            min_stack: Stack::<i32>::new(),
        }
    }

    fn push(&mut self, data: i32) {
        self.stack.push(data);

        if self.min_stack.is_empty() || data <= *self.min_stack.peek().unwrap() {
            self.min_stack.push(data);
        }
    }

    fn pop(&mut self) -> Option<i32> {
        let data = self.stack.pop()?;

        if Some(&data) == self.min_stack.peek() {
            self.min_stack.pop();
        }

        Some(data)
    }

    fn get_min(&self) -> Option<&i32> {
        self.min_stack.peek()
    }
}

struct TwoStackQueue {
    in_stack: Stack<i32>,
    out_stack: Stack<i32>,
}

impl TwoStackQueue {
    fn new() -> TwoStackQueue {
        TwoStackQueue {
            in_stack: Stack::new(),
            out_stack: Stack::new(),
        }
    }

    // Add an element to the back of the queue
    fn enqueue(&mut self, data: i32) {
        // Always push onto in_stack
        self.in_stack.push(data);
    }

    // Remove and return the front element of the queue
    fn dequeue(&mut self) -> Option<i32> {
        // If out_stack is empty, pour in_stack into out_stack
        if self.out_stack.is_empty() {
            while let Some(data) = self.in_stack.pop() {
                // Pop from in_stack and push onto out_stack
                self.out_stack.push(data);
            }
        }

        // Pop from out_stack
        self.out_stack.pop()
    }

    fn peek(&mut self) -> Option<&i32> {
        // If out_stack is empty, pour in_stack into out_stack
        if self.out_stack.is_empty() {
            while let Some(data) = self.in_stack.pop() {
                self.out_stack.push(data);
            }
        }

        // Peek from out_stack
        self.out_stack.peek()
    }
}

fn main() {
    let mut s = Stack::<i32>::new();

    s.push(10); // TOP -> [10] -> NONE
    s.push(20); // TOP -> [20] -> [10] -> NONE
    s.push(30); // TOP -> [30] -> [20] -> [10] -> NONE

    let a = s.pop(); // a = Some(30), TOP -> [20] -> [10] -> NONE
    let b = s.pop(); // b = Some(20), TOP -> [10] -> NONE
    let c = s.pop(); // c = Some(10), TOP -> NONE
    let d = s.pop(); // d = NONE, stack is empty

    println!("Popped values: {:?}, {:?}, {:?}, {:?}", a, b, c, d);

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

    println!("Peeked values: {:?}, {:?}", e, f);

    s.is_empty(); // false

    println!("{}", is_balanced("([{}])")); // true
    println!("{}", is_balanced("([)]")); // false
    println!("{}", is_balanced("(((")); // false

    let mut ms = MinStack::new();
    ms.push(3); // stack: [3],          min: [3]
    ms.push(5); // stack: [3, 5],       min: [3]
    ms.push(2); // stack: [3, 5, 2],    min: [3, 2]
    ms.push(1); // stack: [3, 5, 2, 1], min: [3, 2, 1]

    println!("{:?}", ms.get_min()); // Some(1)

    ms.pop(); // stack: [3, 5, 2],    min: [3, 2]
    println!("{:?}", ms.get_min()); // Some(2)

    ms.pop(); // stack: [3, 5],       min: [3]
    println!("{:?}", ms.get_min()); // Some(3)

    // Usage
    let mut q = TwoStackQueue::new();
    q.enqueue(1); // [1]
    q.enqueue(2); // [1, 2]
    q.enqueue(3); // [1, 2, 3]

    println!("{:?}", q.dequeue()); // Some(1)
    println!("{:?}", q.dequeue()); // Some(2)

    q.enqueue(4); // [3, 4]

    println!("{:?}", q.dequeue()); // Some(3)
    println!("{:?}", q.dequeue()); // Some(4)
}
