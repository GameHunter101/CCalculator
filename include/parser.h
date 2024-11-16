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

typedef enum {
    SingleCharacter,
    WrappingCharacters,
} SeparatorType;

typedef struct {
    SeparatorType type;
    union {
        char single;
        char wrapping[2];
    } separator;
} Separator;

typedef struct {
    int index;
    char *str;
} InputFragment;

typedef struct {
    char **fragments;
    int fragmentCount;
} AllFragments;

FileData getData(char *);
AllTokens tokenize(FileData);
void printToken(Token *);
AllFragments splitStringTokens(FileData);
