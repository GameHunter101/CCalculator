#include "parser.h"

typedef struct {
    Token* data;
    int size;
    int maxSize;
} Stack;

Token pop(Stack*);
Token* peek(Stack*);
void push(Stack*, Token item);
Stack newStack();
Stack newStackWithSize(int);
