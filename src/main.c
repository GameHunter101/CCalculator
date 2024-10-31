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
        push(&stack, thisToken);
    }

    printToken(peek(&stack));

    return 0;
}
