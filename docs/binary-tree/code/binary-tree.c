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

    return 0;
}
