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
}

fn main() {
    let mut bst = BST::new();
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(1);
    bst.insert(4);
    //       [5]
    //      /   \
    //    [3]   [7]
    //   /   \
    // [1]   [4]
}
