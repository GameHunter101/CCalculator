#include "../include/ast.h"
#include "../include/parser.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node createTree(FileData data) {
    char *nextFrag = NULL;
    char *frag = getFragment(data.string, data.stringLength, &nextFrag);
    Stack stack = newStack();
    while (nextFrag != NULL) {
        Token token = tokenize(frag);
        Node currentNode = {.data = token, .left = NULL, .right = NULL};
        if (token.type == OperandToken) {
            Node right = pop(&stack);
            Node left = pop(&stack);
            if (right.data.type != EmptyToken) {
                currentNode.right = calloc(1, sizeof(Node));
                *currentNode.right = right;
            }

            if (left.data.type != EmptyToken) {
                currentNode.left = calloc(1, sizeof(Node));
                *currentNode.left = left;
            }
        }
        push(&stack, currentNode);
        frag = getFragment(nextFrag, strlen(nextFrag), &nextFrag);
    }
    return pop(&stack);
}

ExpressionValue executeNode(Node *root) {
    ExpressionValue returnValue;
    if (root->data.type == ScalarValueToken) {
        returnValue.type = ScalarType;
        returnValue.data.scalar = root->data.data.value;
        return returnValue;
    }
    if (root->data.type == VectorValueToken) {
        returnValue.type = VectorType;
        returnValue.data.vector = root->data.data.vectorData;
        return returnValue;
    }

    ExpressionValue leftValue = executeNode(root->left);
    ExpressionValue rightValue = executeNode(root->right);
    switch (root->data.data.operandData.op) {
    case Addition: {
        if (leftValue.type == ScalarType) {
            returnValue.type = ScalarType;
            returnValue.data.scalar =
                leftValue.data.scalar + rightValue.data.scalar;
            return returnValue;
        }
        returnValue.type = VectorType;
        int maxVectorDimension = max(leftValue.data.vector.dimension,
                                     rightValue.data.vector.dimension);
        returnValue.data.vector.components =
            calloc(maxVectorDimension, sizeof(float));
        for (int i = 0; i < maxVectorDimension; i++) {
            float leftComponent = 0;
            if (i < leftValue.data.vector.dimension) {
                leftComponent = leftValue.data.vector.components[i];
            }
            float rightComponent = 0;
            if (i < rightValue.data.vector.dimension) {
                rightComponent = rightValue.data.vector.components[i];
            }
            returnValue.data.vector.components[i] =
                leftComponent + rightComponent;
        }
        returnValue.data.vector.dimension = maxVectorDimension;
        return returnValue;
    }
    case Subtraction: {
        if (leftValue.type == ScalarType) {
            returnValue.type = ScalarType;
            returnValue.data.scalar =
                leftValue.data.scalar - rightValue.data.scalar;
            return returnValue;
        }
        returnValue.type = VectorType;
        int maxVectorDimension = max(leftValue.data.vector.dimension,
                                     rightValue.data.vector.dimension);
        returnValue.data.vector.components =
            calloc(maxVectorDimension, sizeof(float));
        for (int i = 0; i < maxVectorDimension; i++) {
            float leftComponent = 0;
            if (i < leftValue.data.vector.dimension) {
                leftComponent = leftValue.data.vector.components[i];
            }
            float rightComponent = 0;
            if (i < rightValue.data.vector.dimension) {
                rightComponent = rightValue.data.vector.components[i];
            }
            returnValue.data.vector.components[i] =
                leftComponent - rightComponent;
        }
        returnValue.data.vector.dimension = maxVectorDimension;
        return returnValue;
    }
    case Multiplication: {
        if (leftValue.type == ScalarType && rightValue.type == ScalarType) {
            returnValue.type = ScalarType;
            returnValue.data.scalar =
                leftValue.data.scalar * rightValue.data.scalar;
            return returnValue;
        }
        returnValue.type = VectorType;
        if (leftValue.type == ScalarType && rightValue.type == VectorType) {
            returnValue.data.vector.components =
                calloc(rightValue.data.vector.dimension, sizeof(float));
            for (int i = 0; i < rightValue.data.vector.dimension; i++) {
                returnValue.data.vector.components[i] =
                    rightValue.data.vector.components[i] *
                    leftValue.data.scalar;
            }
            returnValue.data.vector.dimension =
                rightValue.data.vector.dimension;
            return returnValue;
        }
        if (leftValue.type == VectorType && rightValue.type == ScalarType) {
            returnValue.type = VectorType;
            returnValue.data.vector.components =
                calloc(leftValue.data.vector.dimension, sizeof(float));
            for (int i = 0; i < leftValue.data.vector.dimension; i++) {
                returnValue.data.vector.components[i] =
                    leftValue.data.vector.components[i] *
                    rightValue.data.scalar;
            }
            returnValue.data.vector.dimension = leftValue.data.vector.dimension;
            return returnValue;
        }
        returnValue.type = ScalarType;
        int minVectorDimension = min(leftValue.data.vector.dimension,
                                     rightValue.data.vector.dimension);
        returnValue.data.scalar = 0.0;
        for (int i = 0; i < minVectorDimension; i++) {
            float leftComponent = 1;
            if (i < leftValue.data.vector.dimension) {
                leftComponent = leftValue.data.vector.components[i];
            }
            float rightComponent = 0;
            if (i < rightValue.data.vector.dimension) {
                rightComponent = rightValue.data.vector.components[i];
            }
            returnValue.data.scalar += leftComponent * rightComponent;
        }
        return returnValue;
    }
    case Division: {
        if (leftValue.type == ScalarType) {
            returnValue.type = ScalarType;
            returnValue.data.scalar =
                leftValue.data.scalar / rightValue.data.scalar;
            return returnValue;
        }
        returnValue.type = VectorType;

        returnValue.data.vector.components =
            calloc(leftValue.data.vector.dimension, sizeof(float));
        for (int i = 0; i < leftValue.data.vector.dimension; i++) {
            returnValue.data.vector.components[i] =
                leftValue.data.vector.components[i] / rightValue.data.scalar;
            returnValue.data.vector.dimension = leftValue.data.vector.dimension;
        }
        return returnValue;
    }
    }
}

void printTreeHelper(Node *root, int paddingSize) {
    if (root == NULL) {
        return;
    }
    char padding[paddingSize + 1];
    for (int i = 0; i < paddingSize; i++) {
        padding[i] = '\t';
    }
    padding[paddingSize] = 0;
    printf("%s", padding);
    printToken(&root->data);

    if (root->left != NULL) {
        printf("%sLeft: \n", padding);
        printTreeHelper(root->left, paddingSize + 1);
    }
    if (root->right != NULL) {
        printf("%sRight: \n", padding);
        printTreeHelper(root->right, paddingSize + 1);
    }
}

void printTree(Node *root) { printTreeHelper(root, 0); }

void destroyTree(Node *root) {
    if (root == NULL) {
        return;
    }
    if (root->data.type == VectorValueToken) {
        free(root->data.data.vectorData.components);
    }
    if (root->left != NULL) {
        destroyTree(root->left);
    }
    if (root->right != NULL) {
        destroyTree(root->right);
    }
}

NodeType evaluateTreeType(Node *root) {
    if (root->data.type == ScalarValueToken) {
        return ScalarType;
    } else if (root->data.type == VectorValueToken) {
        return VectorType;
    }
    NodeType leftType = evaluateTreeType(root->left);
    NodeType rightType = evaluateTreeType(root->right);
    if (leftType == rightType) {
        if (root->data.data.operandData.op == Multiplication &&
            leftType == VectorType) {
            return ScalarType;
        }
        return leftType;
    }
    return BothType;
}

bool validateTree(Node *root) {
    bool currentNodeValidity = false;
    if (root->data.type != OperandToken) {
        return true;
    }
    if (root->data.data.operandData.parameterCount != 2) {
        return true;
    }
    NodeType treeType = evaluateTreeType(root);
    NodeType leftType = evaluateTreeType(root->left);
    NodeType rightType = evaluateTreeType(root->right);

    switch (root->data.data.operandData.op) {
    case Addition:
    case Subtraction:
        printToken(&root->data);
        currentNodeValidity = treeType != BothType;
        break;
    case Multiplication:
        currentNodeValidity = true;
        break;
    case Division:
        currentNodeValidity = rightType == ScalarType;
        break;
    }
    return currentNodeValidity && validateTree(root->left) &&
           validateTree(root->right);
}
