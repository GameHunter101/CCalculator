#include <stdio.h>
#include <stdlib.h>

#include "../include/ast.h"
#include "../include/parser.h"
#include "../include/stack.h"

int main() {
    FileData data = getData("../data.txt");

    AllTokens tokens = splitStringTokens(data, 0);

    printf("Main print:\n");
    for (int i = 0; i < tokens.tokenCount; i++) {
        printToken(&tokens.tokens[i]);
    }

    free(tokens.tokens);
    free(data.string);

    /* AllTokens tokens = tokenize(data);

    free(data.string);

    Stack stack = newStack();

    for (int i = 0; i < tokens.tokenCount; i++) {
        Token thisToken = tokens.tokens[i];
        push(&stack, thisToken);
    }

    printf("Tree:\n");
    Node tree = createTree(&stack);
    printf("Final value: %f", executeNode(&tree)); */

    return 0;
}
