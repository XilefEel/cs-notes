#[allow(dead_code)]
struct Node {
    data: i32,
    next: Option<Box<Node>>,
}

#[allow(dead_code)]
impl Node {
    fn new(data: i32) -> Box<Node> {
        Box::new(Node { data, next: None })
    }

    fn get(head: &Option<Box<Node>>, index: usize) -> Option<&Node> {
        let mut current = head;
        let mut i = 0;

        while let Some(node) = current {
            if i == index {
                return Some(node);
            }

            current = &node.next;
            i += 1;
        }

        None
    }

    fn print_list(head: &Option<Box<Node>>) {
        let mut current = head;

        print!("HEAD -> ");

        while let Some(node) = current {
            print!("{}", node.data);

            if node.next.is_some() {
                print!(" -> ");
            }

            current = &node.next;
        }

        println!(" -> NONE");
    }

    fn traverse_apply<F>(head: &mut Option<Box<Node>>, mut func: F)
    where
        F: FnMut(&mut i32),
    {
        let mut current = head;

        while let Some(node) = current {
            func(&mut node.data);

            current = &mut node.next;
        }
    }

    fn insert_at_head(head: Option<Box<Node>>, data: i32) -> Option<Box<Node>> {
        Some(Box::new(Node { data, next: head }))
    }

    fn insert_at_tail(head: Option<Box<Node>>, data: i32) -> Option<Box<Node>> {
        match head {
            None => Some(Node::new(data)),
            Some(mut node) => {
                let mut current = &mut node;

                while current.next.is_some() {
                    current = current.next.as_mut().unwrap();
                }

                current.next = Some(Node::new(data));

                Some(node)
            }
        }
    }

    fn insert_at_index(head: Option<Box<Node>>, data: i32, index: usize) -> Option<Box<Node>> {
        match head {
            None => {
                if index == 0 {
                    Some(Node::new(data))
                } else {
                    panic!("Index out of bounds");
                }
            }
            Some(mut node) => {
                if index == 0 {
                    return Some(Box::new(Node {
                        data,
                        next: Some(node),
                    }));
                }

                let mut current = &mut node;
                for _ in 0..index - 1 {
                    if current.next.is_none() {
                        panic!("Index out of bounds");
                    }
                    current = current.next.as_mut().unwrap();
                }

                let new_node = Box::new(Node {
                    data,
                    next: current.next.take(),
                });

                current.next = Some(new_node);

                Some(node)
            }
        }
    }

    fn delete_at_head(head: Option<Box<Node>>) -> Option<Box<Node>> {
        match head {
            None => {
                println!("List is empty");
                None
            }
            Some(node) => node.next,
        }
    }

    fn delete_at_tail(head: Option<Box<Node>>) -> Option<Box<Node>> {
        match head {
            None => {
                println!("List is empty");
                None
            }
            Some(mut node) => {
                if node.next.is_none() {
                    return Node::delete_at_head(Some(node));
                }

                let mut current = &mut node;
                while current.next.as_ref().unwrap().next.is_some() {
                    current = current.next.as_mut().unwrap();
                }
                current.next = None;

                Some(node)
            }
        }
    }

    fn delete_at_index(head: Option<Box<Node>>, index: usize) -> Option<Box<Node>> {
        match head {
            None => {
                println!("List is empty");
                None
            }
            Some(mut node) => {
                if index == 0 {
                    return Node::delete_at_head(Some(node));
                }

                let mut current = &mut node;
                for _ in 0..index - 1 {
                    if current.next.is_none() {
                        panic!("Index out of bounds");
                    }
                    current = current.next.as_mut().unwrap();
                }

                if current.next.is_none() {
                    panic!("Index out of bounds");
                }

                let target = current.next.take();
                current.next = target.unwrap().next;

                Some(node)
            }
        }
    }

    fn reverse(head: Option<Box<Node>>) -> Option<Box<Node>> {
        let mut prev = None;
        let mut curr = head;

        // Traverse and reverse each pointer
        while let Some(mut node) = curr {
            let next = node.next.take();

            node.next = prev;

            prev = Some(node);
            curr = next;
        }

        prev
    }
}

fn main() {
    let mut head = None;

    head = Node::insert_at_head(head, 10); // HEAD -> [10] -> NONE
    head = Node::insert_at_head(head, 20); // HEAD -> [20] -> [10] -> NONE
    head = Node::insert_at_head(head, 30); // HEAD -> [30] -> [20] -> [10] -> NONE

    Node::print_list(&head);

    head = Node::insert_at_tail(head, 40); // HEAD -> [30] -> [20] -> [10] -> [40] -> NONE
    Node::print_list(&head);

    head = Node::insert_at_index(head, 50, 2); // HEAD -> [30] -> [20] -> [50] -> [10] -> [40] -> NONE
    Node::print_list(&head);

    head = Node::delete_at_head(head); // HEAD -> [50] -> [20] -> [10] -> [40] -> NONE
    Node::print_list(&head);

    head = Node::delete_at_tail(head); // HEAD -> [50] -> [20] -> [10] -> NONE
    Node::print_list(&head);

    head = Node::delete_at_index(head, 1); // HEAD -> [50] -> [10] -> NONE
    Node::print_list(&head);

    head = Node::reverse(head); // HEAD -> [10] -> [50] -> NONE
    Node::print_list(&head);
}
