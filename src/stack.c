#include "../include/stack.h"
#include <stdlib.h>
#include <string.h>

Token pop(Stack *stack) {
    Token lastItem = stack->data[stack->size - 1];

    stack->size--;
    return lastItem;
}

Token *peek(Stack *stack) { 
    if (stack->size == 0) {
        return NULL;
    }
    return &stack->data[stack->size - 1];
}
void push(Stack *stack, Token item) {
    if (stack->size == stack->maxSize) {
        Token *data = stack->data;
        stack->data = calloc(stack->maxSize * 1.5, sizeof(item));
        int _ = memcmp(stack->data, data, stack->maxSize * sizeof(item));
        stack->maxSize *= 1.5;
    }
    stack->data[stack->size++] = item;
}
