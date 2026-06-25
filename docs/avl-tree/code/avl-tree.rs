struct Node {
    data: i32,
    height: i32,
    left: Option<Box<Node>>,
    right: Option<Box<Node>>,
}

struct AVLTree {
    root: Option<Box<Node>>,
}

impl AVLTree {
    fn new() -> Self {
        AVLTree { root: None }
    }

    fn height(node: Option<&Box<Node>>) -> i32 {
        match node {
            None => -1,
            Some(n) => n.height,
        }
    }

    fn get_balance_factor(node: Option<&Box<Node>>) -> i32 {
        match node {
            None => 0,
            Some(n) => Self::height(n.left.as_ref()) - Self::height(n.right.as_ref()),
        }
    }

    fn update_height(node: &mut Box<Node>) {
        let left_height = Self::height(node.left.as_ref());
        let right_height = Self::height(node.right.as_ref());
        node.height = 1 + left_height.max(right_height);
    }

    fn rotate_right(mut y: Box<Node>) -> Box<Node> {
        let mut x = y.left.take().unwrap();
        let t2 = x.right.take();

        y.left = t2;
        Self::update_height(&mut y);

        x.right = Some(y);
        Self::update_height(&mut x);

        x
    }

    fn rotate_left(mut y: Box<Node>) -> Box<Node> {
        let mut x = y.right.take().unwrap();
        let t2 = x.left.take();

        y.right = t2;
        Self::update_height(&mut y);

        x.left = Some(y);
        Self::update_height(&mut x);

        x
    }

    fn rebalance(mut n: Box<Node>) -> Box<Node> {
        Self::update_height(&mut n);
        let bf = Self::get_balance_factor(Some(&n));

        if bf > 1 && Self::get_balance_factor(n.left.as_ref()) >= 0 {
            return Self::rotate_right(n);
        }

        if bf < -1 && Self::get_balance_factor(n.right.as_ref()) <= 0 {
            return Self::rotate_left(n);
        }

        if bf > 1 && Self::get_balance_factor(n.left.as_ref()) < 0 {
            let left = n.left.take().unwrap();
            n.left = Some(Self::rotate_left(left));
            return Self::rotate_right(n);
        }

        if bf < -1 && Self::get_balance_factor(n.right.as_ref()) > 0 {
            let right = n.right.take().unwrap();
            n.right = Some(Self::rotate_right(right));
            return Self::rotate_left(n);
        }

        n
    }

    fn insert_node(node: Option<Box<Node>>, data: i32) -> Box<Node> {
        match node {
            None => Box::new(Node {
                data,
                height: 0,
                left: None,
                right: None,
            }),
            Some(mut current) => {
                if data < current.data {
                    current.left = Some(Self::insert_node(current.left.take(), data));
                } else if data > current.data {
                    current.right = Some(Self::insert_node(current.right.take(), data));
                } else {
                    return current;
                }

                Self::rebalance(current)
            }
        }
    }

    fn insert(&mut self, data: i32) {
        self.root = Some(Self::insert_node(self.root.take(), data));
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

                Some(Self::rebalance(current))
            }
        }
    }

    fn delete(&mut self, data: i32) {
        self.root = Self::delete_node(self.root.take(), data);
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
}

fn main() {
    let mut tree = AVLTree::new();

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);

    tree.preorder();
    println!();

    tree.delete(1);
    tree.delete(4);
    tree.delete(3);

    tree.preorder();
    println!();
}
