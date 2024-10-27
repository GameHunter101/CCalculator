#include "parser.h"

typedef struct Stack {
    Token* data;
    int size;
    int maxSize;
} Stack;

Token pop(Stack*);
Token* peek(Stack*);
void push(Stack*, Token item);

