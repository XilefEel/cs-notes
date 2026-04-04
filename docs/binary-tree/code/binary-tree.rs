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
}

fn main() {
    let mut bst = BST::new();
}
