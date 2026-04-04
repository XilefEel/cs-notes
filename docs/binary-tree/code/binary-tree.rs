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

    match bst.search(4) {
        Some(node) => println!("Found: {}", node.data), // Found: 4
        None => println!("Not found"),
    }

    match bst.search(99) {
        Some(node) => println!("Found: {}", node.data),
        None => println!("Not found"), // Not found
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
}
