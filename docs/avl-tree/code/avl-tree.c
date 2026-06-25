#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    int height;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct AVLTree {
    Node *root;
} AVLTree;

Node *create_node(int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->height = 0;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

AVLTree create_avl_tree() {
    AVLTree tree;
    tree.root = NULL;
    return tree;
}

int height(Node *node) {
    if (node == NULL) return -1;
    return node->height;
}

int get_balance_factor(Node *node) {
    if (node == NULL) return 0;
    return height(node->left) - height(node->right);
}

void update_height(Node *node) {
    int left_height = height(node->left);
    int right_height = height(node->right);
    node->height = 1 + (left_height > right_height ? left_height : right_height);
}

Node *rotate_right(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    update_height(y);
    update_height(x);

    return x;
}

Node *rotate_left(Node *y) {
    Node *x = y->right;
    Node *T2 = x->left;

    x->left = y;
    y->right = T2;

    update_height(y);
    update_height(x);

    return x;
}

Node *rebalance(Node *n) {
    update_height(n);
    int bf = get_balance_factor(n);

    if (bf > 1 && get_balance_factor(n->left) >= 0)
        return rotate_right(n);

    if (bf < -1 && get_balance_factor(n->right) <= 0)
        return rotate_left(n);

    if (bf > 1 && get_balance_factor(n->left) < 0) {
        n->left = rotate_left(n->left);
        return rotate_right(n);
    }

    if (bf < -1 && get_balance_factor(n->right) > 0) {
        n->right = rotate_right(n->right);
        return rotate_left(n);
    }

    return n;
}

Node *insert_node(Node *node, int data) {
    if (node == NULL) {
        return create_node(data);
    }

    if (data < node->data) {
        node->left = insert_node(node->left, data);
    } else if (data > node->data) {
        node->right = insert_node(node->right, data);
    } else {
        return node;
    }

    return rebalance(node);
}

void insert(AVLTree *tree, int data) {
    tree->root = insert_node(tree->root, data);
}

Node *find_successor(Node *node) {
    Node *current = node->right;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

Node *delete_node(Node *node, int data) {
    if (node == NULL) return NULL;

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

    return rebalance(node);
}

void delete(AVLTree *tree, int data) {
    tree->root = delete_node(tree->root, data);
}

void preorder(Node *node) {
    if (node == NULL) return;
    printf("%d ", node->data);
    preorder(node->left);
    preorder(node->right);
}

int main() {
    AVLTree tree = create_avl_tree();

    insert(&tree, 1);
    insert(&tree, 2);
    insert(&tree, 3);
    insert(&tree, 4);
    insert(&tree, 5);

    preorder(tree.root);
    printf("\n");

    delete(&tree, 1);
    delete(&tree, 4);
    delete(&tree, 3);

    preorder(tree.root);
    printf("\n");

    return 0;
}
