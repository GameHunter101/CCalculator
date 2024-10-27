#include "../include/parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const static char OPERATIONS[4] = {'+', '-', '*', '/'};
const static int OPERATIONS_COUNT = 4;

FileData getData(char *path) {
    FILE *fptr = fopen(path, "r");

    fseek(fptr, 0, SEEK_END);
    int size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    char *fileContents = calloc(size, sizeof('a'));
    fgets(fileContents, size * sizeof('a'), fptr);
    fclose(fptr);
    fileContents[strcspn(fileContents, "\r\n")] = 0;

    FileData data = {fileContents, size};

    return data;
}

AllTokens tokenize(FileData data) {
    char *stringTokens[data.stringLength];
    char *currentTokenString = strtok(data.string, " ");
    if (currentTokenString != NULL) {
        int tokenCount = 1;
        stringTokens[0] = currentTokenString;
        for (int i = 1; i < data.stringLength; i++) {
            currentTokenString = strtok(NULL, " ");
            if (currentTokenString != NULL) {
                stringTokens[i] = currentTokenString;
                tokenCount++;
            } else {
                break;
            }
        }
        Token tempToken;
        Token *tokens = calloc(tokenCount, sizeof(tempToken));
        for (int i = 0; i < tokenCount; i++) {
            char *queriedTokenString = stringTokens[i];
            Token token = {.type = ValueToken, .data.value = 0};
            if (strlen(queriedTokenString) == 1) {
                for (int j = 0; j < OPERATIONS_COUNT; j++) {
                    if ((*queriedTokenString) == OPERATIONS[j]) {
                        token.data.op = j;
                        token.type = OperandToken;
                        break;
                    }
                }
            }
            if (token.type != OperandToken) {
                errno = 0;
                char *end;
                long value = strtol(queriedTokenString, &end, 10);
                token.data.value = value;
                token.type = ValueToken;
            }
            tokens[i] = token;
        }
        AllTokens allTokens = {tokens, tokenCount};

        return allTokens;
    }

    AllTokens nothing = {.tokens = NULL, .tokenCount = 0};
    return nothing;
}

void printToken(Token *token) {
    if (token == NULL) {
        printf("NULL");
    }
    switch (token->type) {
    case ValueToken:
        printf("Value: %i\n", token->data.value);
        break;
    case OperandToken: {
        char thisOp;
        switch (token->data.op) {
        case Addition:
            thisOp = '+';
            break;
        case Subtraction:
            thisOp = '-';
            break;
        case Multiplication:
            thisOp = '*';
            break;
        case Division:
            thisOp = '/';
            break;
        }
        printf("Operand: %c\n", thisOp);
        break;
    }
    }
}
