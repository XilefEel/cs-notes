#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    Node *root;
} BST;

BST create_bst() {
    BST bst;
    bst.root = NULL;
    return bst;
}

Node *create_node(int data) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node *insert_node(Node *node, int data) {
    if (node == NULL) {
        return create_node(data);
    }

    if (data < node->data) {
        node->left = insert_node(node->left, data);
    } else if (data > node->data) {
        node->right = insert_node(node->right, data);
    }

    return node;
}

void insert(BST *bst, int data) {
    bst->root = insert_node(bst->root, data);
}

Node *search(Node *node, int data) {
    if (node == NULL) {
        return NULL;
    }

    if (data == node->data) {
        return node;
    }

    if (data < node->data) {
        return search(node->left, data);
    } else {
        return search(node->right, data);
    }
}

Node *find_successor(Node *node) {
    node = node->right;
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

Node *delete_node(Node *node, int data) {
    if (node == NULL) {
        return NULL;
    }

    if (data < node->data) {
        node->left = delete_node(node->left, data);
    } else if (data > node->data) {
        node->right = delete_node(node->right, data);
    } else {
        if (node->left == NULL) {
            Node *temp = node->right;
            free(node);
            return temp;
        }
        if (node->right == NULL) {
            Node *temp = node->left;
            free(node);
            return temp;
        }

        Node *successor = find_successor(node);

        node->data = successor->data;

        node->right = delete_node(node->right, successor->data);
    }

    return node;
}

void delete(BST *bst, int data) {
    bst->root = delete_node(bst->root, data);
}

void preorder(Node *node) {
    if (node == NULL) {
        return;
    }

    printf("%d ", node->data);
    preorder(node->left);
    preorder(node->right);
}

void inorder(Node *node) {
    if (node == NULL) {
        return;
    }

    inorder(node->left);
    printf("%d ", node->data);
    inorder(node->right);
}

void postorder(Node *node) {
    if (node == NULL) {
        return;
    }

    postorder(node->left);
    postorder(node->right);
    printf("%d ", node->data);
}

typedef struct QueueNode {
    Node *data;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *back;
    int size;
} Queue;

Queue create_bfs_queue() {
    Queue q;
    q.front = NULL;
    q.back = NULL;
    q.size = 0;
    return q;
}

void bfs_enqueue(Queue *q, Node *data) {
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    node->data = data;
    node->next = NULL;
    if (q->back == NULL) {
        q->front = node;
        q->back = node;
    } else {
        q->back->next = node;
        q->back = node;
    }
    q->size++;
}

Node *bfs_dequeue(Queue *q) {
    QueueNode *temp = q->front;
    Node *data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->back = NULL;
    }
    free(temp);
    q->size--;
    return data;
}

int bfs_is_empty(Queue *q) {
    return q->size == 0;
}

void level_order(BST *bst) {
    if (bst->root == NULL) {
        return;
    }

    Queue q = create_bfs_queue();
    bfs_enqueue(&q, bst->root);

    while (!bfs_is_empty(&q)) {
        Node *node = bfs_dequeue(&q);
        printf("%d ", node->data);

        if (node->left != NULL) {
            bfs_enqueue(&q, node->left);
        }

        if (node->right != NULL) {
            bfs_enqueue(&q, node->right);
        }
    }
}

int max(int a, int b) {
    return a > b ? a : b;
};

int height(Node *node) {
    if (node == NULL) {
        return -1;
    }

    return 1 + max(height(node->left), height(node->right));
}


int check_balanced(Node *node) {
    if (node == NULL) {
        return 0;
    }

    int left_height = check_balanced(node->left);
    int right_height = check_balanced(node->right);

    if (left_height == -1 || right_height == -1) {
        return -1;
    }

    if (abs(left_height - right_height) > 1) {
        return -1;
    }

    return 1 + max(left_height, right_height);
}

int is_balanced(BST *bst) {
    return check_balanced(bst->root) != -1;
}

Node *lca(Node *node, int p, int q) {
    if (node == NULL) {
        return NULL;
    }

    if (p < node->data && q < node->data) {
        return lca(node->left, p, q);
    }

    if (p > node->data && q > node->data) {
        return lca(node->right, p, q);
    }

    return node;
}

int is_valid_bst(Node *node, long min, long max) {
    if (node == NULL) {
        return 1;
    }

    if (node->data <= min || node->data >= max) {
        return 0;
    }

    return is_valid_bst(node->left, min, node->data) &&
           is_valid_bst(node->right, node->data, max);
}

int is_valid(BST *bst) {
    return is_valid_bst(bst->root, LONG_MIN, LONG_MAX);
}

int main() {
    BST bst = create_bst();
    insert(&bst, 5);
    insert(&bst, 3);
    insert(&bst, 7);
    insert(&bst, 1);
    insert(&bst, 4);
    insert(&bst, 10);
    //        [5]
    //       /   \
    //     [3]   [7]
    //    /   \     \
    //  [1]   [4]   [10]

    preorder(bst.root);     // 5 3 1 4 7 10
    printf("\n");
    inorder(bst.root);      // 1 3 4 5 7 10
    printf("\n");
    postorder(bst.root);    // 1 4 3 10 7 5
    printf("\n");
    level_order(&bst);      // 5 3 7 1 4 10
    printf("\n");

    Node *result = search(bst.root, 4);
    if (result != NULL) {
        printf("Found: %d\n", result->data);    // Found: 4
    } else {
        printf("Not found\n");
    }

    Node *missing = search(bst.root, 99);
    if (missing != NULL) {
        printf("Found: %d\n", missing->data);
    } else {
        printf("Not found\n");  // Not found
    }

    printf("%d\n", height(bst.root));   // 2
    printf("%d\n", is_balanced(&bst));  // 1 (true)

    result = lca(bst.root, 1, 4);
    printf("%d\n", result->data);   // 3

    result = lca(bst.root, 1, 7);
    printf("%d\n", result->data);   // 5

    result = lca(bst.root, 3, 4);
    printf("%d\n", result->data);   // 3

    delete(&bst, 1);    // Case 1: leaf node
    //        [5]
    //       /   \
    //     [3]   [7]
    //       \     \
    //       [4]   [10]

    delete(&bst, 3);    // Case 2: one child
    //        [5]
    //       /   \
    //     [4]   [7]
    //              \
    //              [10]

    delete(&bst, 5);    // Case 3: two children
    //        [7]
    //       /   \
    //     [4]   [10]

    BST unbalanced = create_bst();
    insert(&unbalanced, 5);
    insert(&unbalanced, 3);
    insert(&unbalanced, 1);

    printf("%d\n", is_balanced(&unbalanced));   // 0 (false)

    BST valid = create_bst();
    insert(&valid, 5);
    insert(&valid, 3);
    insert(&valid, 7);
    insert(&valid, 1);
    insert(&valid, 4);

    printf("%d\n", is_valid(&valid));   // 1 (true)

    Node *root = create_node(5);
    root->left = create_node(3);
    root->left->right = create_node(9);  // 9 > 5, invalid!

    BST invalid;
    invalid.root = root;

    printf("%d\n", is_valid(&invalid));  // 0 (false)

    return 0;
}
