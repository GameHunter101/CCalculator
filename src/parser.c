#include "../include/parser.h"
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const static int OPERATIONS_COUNT = 4;
const static char OPERATIONS[OPERATIONS_COUNT] = {'+', '-', '*', '/'};

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
    if (startIndex >= stringLength) {
        return INT_MAX;
    }
    for (int i = startIndex; i < stringLength; i++) {
        /* printf("Start: %i, Index for %c: %i, current char: %c\n", startIndex,
               target, i, string[i]); */
        if (string[i] == target) {
            return i;
        }
    }
    return INT_MAX;
}

char *createFragment(int startIndex, int nextIndex, char *inputStr) {
    int size = nextIndex - startIndex;
    char *fragStr = calloc(size, 1);
    memcpy(fragStr, inputStr + startIndex, size);
    return fragStr;
}

AllFragments splitStringTokens(FileData fileData) {
    int seperatorCount = 2;
    Separator bracketSeparator = {.type = WrappingCharacters,
                                  .separator.wrapping = "[]"};
    Separator spaceSeparator = {.type = SingleCharacter,
                                .separator.single = ' '};
    Separator commaSeparator = {.type = SingleCharacter,
                                .separator.single = ','};

    Separator separators[] = {bracketSeparator, spaceSeparator, commaSeparator};
    int separatorCount = 3;

    char *inputStr = fileData.string;
    int strLen = fileData.stringLength;

    char **fragments = calloc(strLen, sizeof(char *));
    int fragmentCount = 0;

    int fragmentStartIndex = 0;
    while (fragmentStartIndex < strLen) {
        // printf("Start: %i, ", fragmentStartIndex);
        int fragmentEndIndex = INT_MAX;
        for (int i = 0; i < separatorCount; i++) {
            Separator separator = separators[i];
            if (separator.type == SingleCharacter) {
                fragmentEndIndex =
                    min(min(fragmentEndIndex,
                            indexOf(inputStr, separator.separator.single,
                                    fragmentStartIndex, strLen)),
                        strLen);
            }
            if (separator.type == WrappingCharacters &&
                separator.separator.wrapping[0] ==
                    inputStr[fragmentStartIndex]) {
                fragmentEndIndex =
                    indexOf(inputStr, separator.separator.wrapping[1],
                            fragmentStartIndex, strLen) + 1;

                break;
            }
        }
        // printf("end: %i\n", fragmentEndIndex);
        char *fragment =
            createFragment(fragmentStartIndex, fragmentEndIndex, inputStr);
        if (fragment[0] != 0) {
            // printf("frag: %s\n", fragment);
            fragments[fragmentCount] = fragment;
            fragmentCount++;
        }
        fragmentStartIndex = fragmentEndIndex + 1;
    }
    for (int i = 0; i < fragmentCount; i++) {
        printf("Frag: %s\n", fragments[i]);
    }

    AllFragments allFragments = {.fragments = fragments,
                                 .fragmentCount = fragmentCount};
    return allFragments;
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
