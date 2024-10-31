#ifndef INDIVIDUALS_H
#define INDIVIDUALS_H
typedef enum {
    Addition,
    Subtraction,
    Multiplication,
    Division,
} Operand;

typedef enum {
    ValueToken,
    OperandToken,
} TokenType;

typedef struct {
    TokenType type;
    union {
        int value;
        Operand op;
    } data;
} Token;

typedef struct Node {
    Token data;
    struct Node *left;
    struct Node *right;
} Node;
#endif //INDIVIDUALS_H
