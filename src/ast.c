#include "../include/ast.h"
#include "../include/parser.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

Node createTree(Stack *stack) {
    Token top = pop(stack);
    Node blank;
    if (errno == 1) {
        return blank;
    }
    if (top.type == ScalarValueToken) {
        Node leaf = {.data = top, .left = NULL, .right = NULL};
        return leaf;
    }

    Node *right = calloc(1, sizeof(blank));
    right[0] = createTree(stack);

    Node *left = calloc(1, sizeof(blank));
    left[0] = createTree(stack);

    Node node = {.data = top, .left = left, .right = right};
    return node;
}

float executeNode(Node *root) {
    if (root->data.type == ScalarValueToken) {
        return root->data.data.value;
    }
    float leftValue = executeNode(root->left);
    float rightValue = executeNode(root->right);
    switch (root->data.data.op) {
    case ScalarScalarAddition:
        return leftValue + rightValue;
    case ScalarScalarSubtraction:
        return leftValue - rightValue;
    case ScalarScalarMultiplication:
        return leftValue * rightValue;
    case ScalarScalarDivision:
        return leftValue / rightValue;
    default:
        return INT_MAX;
    }
}

void printTree(Node *root) {
    // printf("token type: %i\n", root->data.type);
    printToken(&root->data);
    if (root->data.type == OperandToken) {
        printf("Left: ");
        printTree(root->left);
        printf("Right: ");
        printTree(root->right);
    }
}
