#include "individuals.h"
#include "stack.h"

#include <stdbool.h>

typedef enum {
    ScalarType,
    VectorType,
    BothType,
} NodeType;

typedef struct {
    NodeType type;
    union {
        float scalar;
        VectorData vector;
    } data;
} ExpressionValue;

Node createTree(FileData);
ExpressionValue executeNode(Node *);
void printTree(Node *);
void destroyTree(Node *);
bool validateTree(Node *);
NodeType evaluateTreeType(Node *);
