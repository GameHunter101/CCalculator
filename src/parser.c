#include "../include/parser.h"
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const static char OPERATIONS[4] = {'+', '-', '*', '/'};
const static int OPERATIONS_COUNT = 4;

int floatStringSize(float num) {
    if (num == 0) {
        return 0;
    }
    return 1 + floatStringSize(num / 10);
}

FileData getData(char *path) {
    FILE *fptr = fopen(path, "r");

    fseek(fptr, 0, SEEK_END);
    int size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    char *fileContents = calloc(size, 1);
    fgets(fileContents, size, fptr);
    fclose(fptr);
    fileContents[strcspn(fileContents, "\r\n")] = 0;

    FileData data = {fileContents, strlen(fileContents)};

    return data;
}

AllTokens tokenize(FileData data) {
    char *stringTokens[data.stringLength];
    char *currentTokenString = strtok(data.string, " ");
    if (currentTokenString != NULL) {
        int tokenCount = 1;
        stringTokens[0] = currentTokenString;
        if (currentTokenString != NULL && currentTokenString[0] == '[') {
            printf("Index: 0\n");
        }
        for (int i = 1; i < data.stringLength; i++) {
            currentTokenString = strtok(NULL, " ");
            if (currentTokenString != NULL) {
                if (currentTokenString[0] == '[' ||
                    currentTokenString[strlen(currentTokenString) - 1] == ']') {
                    printf("Index: %i\n", i);
                }
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
            Token token = {.type = ScalarValueToken, .data.value = 0.0f};
            if (strlen(queriedTokenString) == 1) {
                int j;
                for (j = 0; j < OPERATIONS_COUNT; j++) {
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
                float value = strtof(queriedTokenString, &end);
                token.data.value = value;
                token.type = ScalarValueToken;
            }
            tokens[i] = token;
        }
        AllTokens allTokens = {tokens, tokenCount};

        return allTokens;
    }

    AllTokens nothing = {.tokens = NULL, .tokenCount = 0};
    return nothing;
}

Token parseToken(char *queriedTokenString) {
    Token token = {.type = ScalarValueToken, .data.value = 0.0f};
    if (strlen(queriedTokenString) == 1) {
        int j;
        for (j = 0; j < OPERATIONS_COUNT; j++) {
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
        float value = strtof(queriedTokenString, &end);
        token.data.value = value;
        token.type = ScalarValueToken;
    }
    return token;
}

int indexOf(char *string, char target, int startIndex, int stringLength) {
    // printf("Start: %i, length: %i", startIndex, stringLength);
    for (int i = startIndex; i < stringLength; i++) {
        // printf("Start: %i, Index for %c: %i, current char: %c\n", startIndex,
        // target, i, string[i]);
        if (string[i] == target) {
            return i;
        }
    }
    return INT_MAX;
}

AllTokens splitStringTokens(FileData fileData, int startingIndex) {
    int nearestBracketIndex =
        indexOf(fileData.string, '[', startingIndex, fileData.stringLength);
    int nearestSpaceIndex =
        indexOf(fileData.string, ' ', startingIndex, fileData.stringLength);
    int nearestIndex = min(nearestSpaceIndex, nearestBracketIndex);
    AllTokens blankTokens = {.tokens = NULL, .tokenCount = 0};

    if (nearestIndex == INT_MAX) {
        if (fileData.stringLength - startingIndex == 0) {
            return blankTokens;
        }
        int tokenSize = fileData.stringLength - startingIndex;
        char str[tokenSize + 1];
        memcpy(str, fileData.string + startingIndex,
               fileData.stringLength - startingIndex);
        str[tokenSize] = 0;
        printf("Final: %s\n", str);
        Token token = parseToken(str);
        Token *tokenArr = calloc(1, sizeof(token));
        tokenArr[0] = token;
        AllTokens tokens = {.tokens = tokenArr, .tokenCount = 1};
        return tokens;
    }

    // printf("Nearest: %i\n", nearestIndex);

    int tokenSize = nearestIndex - startingIndex;
    char str[tokenSize + 1];
    memcpy(str, fileData.string + startingIndex, tokenSize);
    str[tokenSize] = 0;
    printf("String: %s\n", str);

    int nextIndex = nearestIndex + 1;

    if (nearestBracketIndex - nearestSpaceIndex == 1) {
        nextIndex++;
    }

    AllTokens previousTokens = splitStringTokens(fileData, nextIndex);
    int tokenCount = previousTokens.tokenCount + 1;
    if (str[0] == 0) {
        return previousTokens;
    }

    // printf("Starting: %i, Nearest: %i, brackets: %i\n", startingIndex,
    // nearestIndex, nearestBracketIndex);

    if (fileData.string[startingIndex - 1] == '[') {
        int closeBracketIndex = indexOf(fileData.string, ']', startingIndex - 1,
                                        fileData.stringLength);
        int bracketContentsSize = closeBracketIndex - startingIndex;
        char bracketContents[bracketContentsSize + 1];
        memcpy(bracketContents, fileData.string+startingIndex, bracketContentsSize);
        bracketContents[bracketContentsSize] = 0;
        printf("Contents: %s\n", bracketContents);
        char *stringToken = strtok(bracketContents, ", ");

        AllTokens bracketsTokens = {.tokens = calloc(bracketContentsSize + previousTokens.tokenCount, sizeof(Token))};
        int tokenCount = 0;
        while (stringToken != NULL) {
            Token thisToken = parseToken(stringToken);
            bracketsTokens.tokens[tokenCount] = thisToken;
            tokenCount++;
            stringToken = strtok(NULL, ", ");
        }
        tokenCount += previousTokens.tokenCount;
        memcpy(bracketsTokens.tokens + tokenCount * sizeof(Token), previousTokens.tokens, previousTokens.tokenCount);
        return bracketsTokens;
    }

    Token token = parseToken(str);

    AllTokens tokens = {.tokenCount = tokenCount};
    Token *tokenArr = calloc(tokenCount, sizeof(token));
    tokenArr[0] = token;
    memcpy(tokenArr + 1, previousTokens.tokens,
           previousTokens.tokenCount * sizeof(token));
    tokens.tokens = tokenArr;

    return tokens;
}

void printToken(Token *token) {
    if (token == NULL) {
        printf("NULL");
        return;
    }
    switch (token->type) {
    case ScalarValueToken:
        printf("Value: %f\n", token->data.value);
        break;
    case OperandToken: {
        char thisOp;
        switch (token->data.op) {
        case VectorAddition:
        case ScalarScalarAddition:
            thisOp = '+';
            break;
        case VectorSubtraction:
        case ScalarScalarSubtraction:
            thisOp = '-';
            break;
        case VectorScalarMultiplication:
        case ScalarScalarMultiplication:
            thisOp = '*';
            break;
        case VectorScalarDivision:
        case ScalarScalarDivision:
            thisOp = '/';
            break;
        default:
            thisOp = 'E';
            break;
        }
        printf("Operand: %c\n", thisOp);
        break;
    }
    case VectorValueToken:
        int stringSize = 0;
        int componentSizes[token->data.vectorData.dimension];
        for (int i = 0; i < token->data.vectorData.dimension; i++) {
            float component = token->data.vectorData.components[i];
            int componentSize = floatStringSize(component) + 5;
            stringSize += componentSize;
            componentSizes[i] = componentSize;
        }
        char str[stringSize];
        for (int i = 0; i < token->data.vectorData.dimension; i++) {
            float component = token->data.vectorData.components[i];
            int componentStringSize = componentSizes[i];
            char thisString[componentStringSize];
            snprintf(thisString, componentStringSize, "%0.2f, ", component);
            strcat(str, thisString);
        }
        printf("Vector Value: %s\n", str);
    }
}
