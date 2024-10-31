#include "../include/ast.h"
#include "../include/parser.h"

#include <stdio.h>
#include <stdlib.h>

Node createTree(Stack *stack) {
    Token top = pop(stack);
    Node blank;
    if (errno == 1) {
        return blank;
    }
    if (top.type == ValueToken) {
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
    if (root->data.type == ValueToken) {
        return root->data.data.value;
    }
    float leftValue = executeNode(root->left);
    float rightValue = executeNode(root->right);
    switch (root->data.data.op) {
    case Addition:
        return leftValue + rightValue;
    case Subtraction:
        return leftValue - rightValue;
    case Multiplication:
        return leftValue * rightValue;
    case Division:
        return leftValue / rightValue;
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
