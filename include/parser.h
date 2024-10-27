typedef enum {
    Addition,
    Subtraction,
    Multiplication,
    Division,
} Operand;

typedef enum {
    ValueToken,
    OperandToken,
} TokenType;

typedef struct {
    TokenType type;
    union {
        int value;
        Operand op;
    } data;
} Token;

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
