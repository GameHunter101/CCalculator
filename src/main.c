#include <stdio.h>
#include <stdlib.h>

#include "../include/stack.h"

int main() {
    FileData data = getData("../data.txt");

    AllTokens tokens = tokenize(data);

    free(data.string);

    Stack stack = {
        .data = calloc(2, sizeof(tokens.tokens[0])), .size = 0, .maxSize = 2};

    for (int i = 0; i < tokens.tokenCount; i++) {
        Token thisToken = tokens.tokens[i];
        if (thisToken.type == ValueToken) {
            push(&stack, thisToken);
            continue;
        }

        Token lhsToken = pop(&stack);
        int lhs = lhsToken.data.value;
        Token rhsToken = pop(&stack);
        int rhs = rhsToken.data.value;

        Token newToken = {.type = ValueToken};

        switch (thisToken.data.op) {
        case Addition:
            newToken.data.value = lhs + rhs;
            break;
        case Subtraction:
            newToken.data.value = lhs - rhs;
            break;
        case Multiplication:
            newToken.data.value = lhs * rhs;
            break;
        case Division:
            newToken.data.value = lhs / rhs;
            break;
        }

        push(&stack, newToken);
    }

    printToken(peek(&stack));

    return 0;
}
