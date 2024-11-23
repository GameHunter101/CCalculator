#include "../include/individuals.h"
#ifndef STACK_H
#define STACK_H
#include <errno.h>

typedef struct {
    Node* data;
    int size;
    int maxSize;
} Stack;

Node pop(Stack*);
Node* peek(Stack*);
void push(Stack*, Node item);
Stack newStack();
Stack newStackWithSize(int);
#endif //STACK_H
