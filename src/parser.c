#include "../include/parser.h"
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const static int OPERATIONS_COUNT = 4;
const static char *OPERATIONS[OPERATIONS_COUNT] = {"+", "-", "*", "/"};

const Separator BRACKET_SEPARATOR = {.type = WrappingCharacters,
                                     .separator.wrapping = "[]"};
const Separator SPACE_SEPARATOR = {.type = SingleCharacter,
                                   .separator.single = ' '};
const Separator COMMA_SEPARATOR = {.type = SingleCharacter,
                                   .separator.single = ','};

int SEPARATOR_COUNT = 3;
Separator SEPARATORS[] = {BRACKET_SEPARATOR, SPACE_SEPARATOR, COMMA_SEPARATOR};

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

int indexOf(char *string, char target, int startIndex, int stringLength) {
    if (startIndex >= stringLength) {
        return INT_MAX;
    }
    for (int i = startIndex; i < stringLength; i++) {
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

char *getFragment(char *inputStr, int strLen, char **nextFragment) {
    char **fragments = calloc(strLen, sizeof(char *));

    int fragmentEndIndex = INT_MAX;
    for (int i = 0; i < SEPARATOR_COUNT; i++) {
        Separator separator = SEPARATORS[i];
        if (separator.type == SingleCharacter) {
            fragmentEndIndex = min(
                min(fragmentEndIndex,
                    indexOf(inputStr, separator.separator.single, 0, strLen)),
                strLen);
        }
        if (separator.type == WrappingCharacters &&
            separator.separator.wrapping[0] == inputStr[0]) {
            fragmentEndIndex =
                indexOf(inputStr, separator.separator.wrapping[1], 0, strLen) +
                1;

            break;
        }
    }
    char *fragment = createFragment(0, fragmentEndIndex, inputStr);
    if (fragment[0] == 0 && strLen - fragmentEndIndex == 0) {
        *nextFragment = NULL;
        return NULL;
    }
    if (fragmentEndIndex == INT_MAX) {
        *nextFragment = NULL;
    } else {
        if (nextFragment != NULL) {
            *nextFragment = inputStr + fragmentEndIndex + 1;
        }
    }

    return fragment;
}

Token tokenize(char *fragment) {
    Token token = {.type = ScalarValueToken, .data.value = 0.0f};
    for (int i = 0; i < OPERATIONS_COUNT; i++) {
        if (strcmp(fragment, OPERATIONS[i]) == 0) {
            token.type = OperandToken;
            token.data.operandData.parameterCount = 2;
            switch (i) {
            case 0:
                token.data.operandData.op = Addition;
                break;
            case 1:
                token.data.operandData.op = Subtraction;
                break;
            case 2:
                token.data.operandData.op = Multiplication;
                break;
            case 3:
                token.data.operandData.op = Division;
                break;
            }
            free(fragment);
            return token;
        }
    }
    for (int i = 0; i < SEPARATOR_COUNT; i++) {
        if (SEPARATORS[i].type == WrappingCharacters &&
            SEPARATORS[i].separator.wrapping[0] == fragment[0]) {
            token.type = VectorValueToken;
            // token.type = ScalarValueToken;
            char *clippedStr = &fragment[1];
            clippedStr[strlen(clippedStr) - 1] = 0;
            // printf("Vec: %s\n", clippedStr);
            char *nextFragment = NULL;
            char *vectorFragment =
                getFragment(clippedStr, strlen(clippedStr), &nextFragment);
            int dimension = 0;
            float *components = calloc(strlen(vectorFragment), sizeof(float));
            while (nextFragment != NULL) {
                if (vectorFragment[0] != 0) {
                    Token component = tokenize(vectorFragment);
                    if (component.type != ScalarValueToken) {
                        printf("Invalid vector field: %s.\n", vectorFragment);
                        continue;
                    }
                    components[dimension] = component.data.value;
                    dimension++;
                }
                vectorFragment = getFragment(nextFragment, strlen(nextFragment),
                                             &nextFragment);
                // printf("vector: %s, next again: %s.\n", vectorFragment,
                // nextFragment);
            }
            token.data.vectorData.dimension = dimension;
            token.data.vectorData.components = components;
            // token.data.value = components[0];

            /* printf("Vector thing:\n");
            printToken(&token); */

            // for (int i = 0; i < dimen)

            /* FileData vectorData = {.string = clippedStr,
                                   .stringLength = strlen(clippedStr)};
            AllFragments vectorFragments = splitStringFragments(vectorData);
            AllTokens vectorTokens = tokenize(vectorFragments);
            token.data.vectorData.dimension = vectorTokens.tokenCount;
            if (vectorTokens.tokenCount == 0) {
                return token;
            }
            float *components = calloc(vectorTokens.tokenCount, sizeof(float));
            for (int i = 0; i < vectorTokens.tokenCount; i++) {
                components[i] = vectorTokens.tokens[i].data.value;
            }
            token.data.vectorData.components = components; */
            free(fragment);
            return token;
        }
    }

    float value = strtof(fragment, NULL);
    token.type = ScalarValueToken;
    token.data.value = value;

    free(fragment);
    return token;
}

void printToken(Token *token) {
    if (token == NULL) {
        printf("NULL\n");
        return;
    }
    switch (token->type) {
    case ScalarValueToken:
        printf("Value: %f\n", token->data.value);
        break;
    case OperandToken: {
        char thisOp;
        switch (token->data.operandData.op) {
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
        default:
            thisOp = 'E';
            break;
        }
        printf("Operand: %c\n", thisOp);
        break;
    }
    case VectorValueToken: {
        int stringSize = 0;
        int componentSizes[token->data.vectorData.dimension];
        for (int i = 0; i < token->data.vectorData.dimension; i++) {
            float component = token->data.vectorData.components[i];
            int componentSize = floatStringSize(component) + 5;
            stringSize += componentSize;
            componentSizes[i] = componentSize;
        }
        char str[stringSize];
        str[0] = 0;
        for (int i = 0; i < token->data.vectorData.dimension; i++) {
            float component = token->data.vectorData.components[i];
            int componentStringSize = componentSizes[i];
            char thisString[componentStringSize];
            snprintf(thisString, componentStringSize, "%0.2f, ", component);
            strcat(str, thisString);
        }
        printf("Vector Value: %s\n", str);
        break;
    }
    case EmptyToken:
        printf("Empty token\n");
        break;
    }
}

void cleanUpTokens(AllTokens tokens) {
    for (int i = 0; i < tokens.tokenCount; i++) {
        if (tokens.tokens[i].type == VectorValueToken) {
            free(tokens.tokens[i].data.vectorData.components);
        }
    }
    free(tokens.tokens);
}
