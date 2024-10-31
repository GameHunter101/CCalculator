#include <stdio.h>
#include <stdlib.h>

#include "../include/stack.h"
#include "../include/parser.h"
#include "../include/ast.h"

int main() {
    FileData data = getData("../data.txt");

    AllTokens tokens = tokenize(data);

    free(data.string);

    Stack stack = newStack();

    for (int i = 0; i < tokens.tokenCount; i++) {
        Token thisToken = tokens.tokens[i];
        push(&stack, thisToken);
    }

    printf("Tree:\n");
    Node tree = createTree(&stack);
    printf("Final value: %i", executeNode(&tree));

    return 0;
}
