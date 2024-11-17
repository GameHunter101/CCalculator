#ifndef INDIVIDUALS_H
#define INDIVIDUALS_H
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
} TokenType;

typedef struct {
    float *components;
    int dimension;
} VectorData;

typedef struct {
    TokenType type;
    union {
        float value;
        Operand op;
        VectorData vectorData;
    } data;
} Token;

typedef struct Node {
    Token data;
    struct Node *left;
    struct Node *right;
} Node;
#endif //INDIVIDUALS_H
