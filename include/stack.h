#include "../include/individuals.h"
#ifndef STACK_H
#define STACK_H
#include <errno.h>

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
#endif //STACK_H
