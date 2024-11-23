#ifndef INDIVIDUALS_H
#define INDIVIDUALS_H

typedef struct {
    char *string;
    int stringLength;
} FileData;

typedef enum {
    Addition,
    Subtraction,
    Multiplication,
    Division,
} Operand;

typedef enum {
    ScalarValueToken,
    VectorValueToken,
    OperandToken,
    EmptyToken,
} TokenType;

typedef struct {
    float *components;
    int dimension;
} VectorData;

typedef struct {
    Operand op;
    int parameterCount;
} OperandData;

typedef struct {
    TokenType type;
    union {
        float value;
        OperandData operandData;
        VectorData vectorData;
    } data;
} Token;

typedef struct Node {
    Token data;
    struct Node *left;
    struct Node *right;
} Node;
#endif //INDIVIDUALS_H
