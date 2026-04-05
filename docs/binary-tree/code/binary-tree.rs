struct QueueNode<T> {
    data: T,
    next: Option<Box<QueueNode<T>>>,
}

struct Queue<T> {
    front: Option<Box<QueueNode<T>>>,
    back: *mut QueueNode<T>,
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
        let mut node = Box::new(QueueNode { data, next: None });

        let raw = &mut *node as *mut QueueNode<T>;

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

    fn is_empty(&self) -> bool {
        self.size == 0
    }
}

struct Node {
    data: i32,
    left: Option<Box<Node>>,
    right: Option<Box<Node>>,
}

struct BST {
    root: Option<Box<Node>>,
}

impl BST {
    fn new() -> BST {
        BST { root: None }
    }

    fn insert(&mut self, data: i32) {
        self.root = Self::insert_node(self.root.take(), data);
    }

    fn insert_node(node: Option<Box<Node>>, data: i32) -> Option<Box<Node>> {
        match node {
            None => Some(Box::new(Node {
                data,
                left: None,
                right: None,
            })),
            Some(mut current) => {
                if data < current.data {
                    current.left = Self::insert_node(current.left.take(), data);
                } else if data > current.data {
                    current.right = Self::insert_node(current.right.take(), data);
                }

                Some(current)
            }
        }
    }

    fn search(&self, data: i32) -> Option<&Node> {
        Self::search_node(self.root.as_ref(), data)
    }

    fn search_node(node: Option<&Box<Node>>, data: i32) -> Option<&Node> {
        match node {
            None => None,
            Some(current) => {
                if data == current.data {
                    Some(current)
                } else if data < current.data {
                    Self::search_node(current.left.as_ref(), data)
                } else {
                    Self::search_node(current.right.as_ref(), data)
                }
            }
        }
    }

    fn delete(&mut self, data: i32) {
        self.root = Self::delete_node(self.root.take(), data);
    }

    fn find_successor(node: &Box<Node>) -> i32 {
        let mut current = node.right.as_ref().unwrap();
        while current.left.is_some() {
            current = current.left.as_ref().unwrap();
        }
        current.data
    }

    fn delete_node(node: Option<Box<Node>>, data: i32) -> Option<Box<Node>> {
        match node {
            None => None,
            Some(mut current) => {
                if data < current.data {
                    current.left = Self::delete_node(current.left.take(), data);
                } else if data > current.data {
                    current.right = Self::delete_node(current.right.take(), data);
                } else {
                    if current.left.is_none() {
                        return current.right;
                    }
                    if current.right.is_none() {
                        return current.left;
                    }

                    let successor_data = Self::find_successor(&current);

                    current.data = successor_data;

                    current.right = Self::delete_node(current.right.take(), successor_data);
                }

                Some(current)
            }
        }
    }

    fn preorder(&self) {
        Self::preorder_node(self.root.as_ref());
    }

    fn preorder_node(node: Option<&Box<Node>>) {
        match node {
            None => return,
            Some(current) => {
                print!("{} ", current.data);
                Self::preorder_node(current.left.as_ref());
                Self::preorder_node(current.right.as_ref());
            }
        }
    }

    fn inorder(&self) {
        Self::inorder_node(self.root.as_ref());
    }

    fn inorder_node(node: Option<&Box<Node>>) {
        match node {
            None => return,
            Some(current) => {
                Self::inorder_node(current.left.as_ref());
                print!("{} ", current.data);
                Self::inorder_node(current.right.as_ref());
            }
        }
    }

    fn postorder(&self) {
        Self::postorder_node(self.root.as_ref());
    }

    fn postorder_node(node: Option<&Box<Node>>) {
        match node {
            None => return,
            Some(current) => {
                Self::postorder_node(current.left.as_ref());
                Self::postorder_node(current.right.as_ref());
                print!("{} ", current.data);
            }
        }
    }

    fn level_order(&self) {
        if self.root.is_none() {
            return;
        }

        let mut q: Queue<*const Node> = Queue::new();

        unsafe {
            q.enqueue(self.root.as_deref().unwrap() as *const Node);

            while !q.is_empty() {
                let node = &*q.dequeue().unwrap();
                print!("{} ", node.data);

                if let Some(left) = node.left.as_deref() {
                    q.enqueue(left as *const Node);
                }

                if let Some(right) = node.right.as_deref() {
                    q.enqueue(right as *const Node);
                }
            }
        }
    }

    fn height(&self) -> i32 {
        Self::height_node(self.root.as_ref())
    }

    fn height_node(node: Option<&Box<Node>>) -> i32 {
        match node {
            None => -1,
            Some(current) => {
                let left_height = Self::height_node(current.left.as_ref());
                let right_height = Self::height_node(current.right.as_ref());

                1 + left_height.max(right_height)
            }
        }
    }

    fn is_balanced(&self) -> bool {
        Self::check_balanced(self.root.as_ref()) != -1
    }

    fn check_balanced(node: Option<&Box<Node>>) -> i32 {
        match node {
            None => 0,
            Some(current) => {
                let left_height = Self::check_balanced(current.left.as_ref());
                let right_height = Self::check_balanced(current.right.as_ref());

                if left_height == -1 || right_height == -1 {
                    return -1;
                }

                if (left_height - right_height).abs() > 1 {
                    return -1;
                }

                1 + left_height.max(right_height)
            }
        }
    }

    fn lca(&self, p: i32, q: i32) -> Option<&Node> {
        Self::lca_node(self.root.as_ref(), p, q)
    }

    fn lca_node(node: Option<&Box<Node>>, p: i32, q: i32) -> Option<&Node> {
        match node {
            None => None,
            Some(current) => {
                if p < current.data && q < current.data {
                    return Self::lca_node(current.left.as_ref(), p, q);
                }

                if p > current.data && q > current.data {
                    return Self::lca_node(current.right.as_ref(), p, q);
                }

                Some(current)
            }
        }
    }

    fn is_valid(&self) -> bool {
        Self::is_valid_bst(self.root.as_ref(), i64::MIN, i64::MAX)
    }

    fn is_valid_bst(node: Option<&Box<Node>>, min: i64, max: i64) -> bool {
        match node {
            None => true,
            Some(current) => {
                let val = current.data as i64;

                if val <= min || val >= max {
                    return false;
                }

                Self::is_valid_bst(current.left.as_ref(), min, val)
                    && Self::is_valid_bst(current.right.as_ref(), val, max)
            }
        }
    }
}

fn main() {
    let mut bst = BST::new();
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(1);
    bst.insert(4);
    bst.insert(10);
    //        [5]
    //       /   \
    //     [3]   [7]
    //    /   \     \
    //  [1]   [4]   [10]

    bst.preorder(); // 5 3 1 4 7 10
    println!();
    bst.inorder(); // 1 3 4 5 7 10
    println!();
    bst.postorder(); // 1 4 3 10 7 5
    println!();
    bst.level_order(); // 5 3 7 1 4 10
    println!();

    match bst.search(4) {
        Some(node) => println!("Found: {}", node.data), // Found: 4
        None => println!("Not found"),
    }

    match bst.search(99) {
        Some(node) => println!("Found: {}", node.data),
        None => println!("Not found"), // Not found
    }

    println!("{}", bst.height()); // 2
    println!("{}", bst.is_balanced()); // true

    if let Some(node) = bst.lca(1, 4) {
        println!("{}", node.data); // 3
    }

    if let Some(node) = bst.lca(1, 7) {
        println!("{}", node.data); // 5
    }

    if let Some(node) = bst.lca(3, 4) {
        println!("{}", node.data); // 3
    }

    bst.delete(1); // Case 1: leaf node
                   //        [5]
                   //       /   \
                   //     [3]   [7]
                   //       \     \
                   //       [4]   [10]

    bst.delete(3); // Case 2: one child
                   //        [5]
                   //       /   \
                   //     [4]   [7]
                   //              \
                   //              [10]

    bst.delete(5); // Case 3: two children
                   //        [7]
                   //       /   \
                   //     [4]   [10]

    let mut valid = BST::new();
    valid.insert(5);
    valid.insert(3);
    valid.insert(7);
    valid.insert(1);
    valid.insert(4);

    println!("{}", valid.is_valid()); // true

    // Manually create an invalid BST
    let mut invalid = BST::new();
    invalid.root = Some(Box::new(Node {
        data: 5,
        left: Some(Box::new(Node {
            data: 3,
            left: None,
            right: Some(Box::new(Node {
                data: 9, // 9 > 5, invalid!
                left: None,
                right: None,
            })),
        })),
        right: None,
    }));

    println!("{}", invalid.is_valid()); // false
}
