/*
 * Binary search tree of int. Values are unique in this starter.
 * Average search/insert O(log n) for balanced-ish trees; worst case O(n).
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int value;
    struct TreeNode *left, *right;
} TreeNode;

TreeNode *bst_insert(TreeNode *root, int value, int *ok) {
    if (root == NULL) {
        TreeNode *n = malloc(sizeof *n);
        if (!n) { *ok = 0; return NULL; }
        n->value = value; n->left = n->right = NULL;
        return n;
    }
    if (value < root->value) {
        TreeNode *child = bst_insert(root->left, value, ok);
        if (*ok) root->left = child;
    } else if (value > root->value) {
        TreeNode *child = bst_insert(root->right, value, ok);
        if (*ok) root->right = child;
    }
    return root; /* duplicate value: no change */
}

TreeNode *bst_find(TreeNode *root, int value) {
    while (root) {
        if (value == root->value) return root;
        root = (value < root->value) ? root->left : root->right;
    }
    return NULL;
}

void bst_inorder(const TreeNode *root) {
    if (!root) return;
    bst_inorder(root->left);
    printf("%d ", root->value);
    bst_inorder(root->right);
}

void bst_free(TreeNode *root) {
    if (!root) return;
    bst_free(root->left);
    bst_free(root->right);
    free(root);
}
