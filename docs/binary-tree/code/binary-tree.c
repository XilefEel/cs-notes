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

int main() {
    BST bst = create_bst();
    insert(&bst, 5);
    insert(&bst, 3);
    insert(&bst, 7);
    insert(&bst, 1);
    insert(&bst, 4);
    //       [5]
    //      /   \
    //    [3]   [7]
    //   /   \
    // [1]   [4]

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
}
