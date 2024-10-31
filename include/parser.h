#include "individuals.h"

typedef struct {
    char *string;
    int stringLength;
} FileData;

typedef struct {
    Token *tokens;
    int tokenCount;
} AllTokens;

FileData getData(char *);
AllTokens tokenize(FileData);
void printToken(Token *);
