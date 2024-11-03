#include "individuals.h"

typedef struct {
    char *string;
    int stringLength;
} FileData;

typedef struct {
    Token *tokens;
    int tokenCount;
} AllTokens;

typedef struct {
    char **strings;
    int tokenCount;
} StringTokens;

FileData getData(char *);
AllTokens tokenize(FileData);
void printToken(Token *);
AllTokens splitStringTokens(FileData, int);
