#ifndef RAINC_TOKENS_H
#define RAINC_TOKENS_H

typedef enum {
    TOKEN_WHITESPACE,
    TOKEN_OPERATOR,
    TOKEN_CONDITIONAL,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_COMMENT_INDICATOR,
    TOKEN_SEPERATOR,
} token_type_t;

typedef struct {
    char *contents;
    token_type_t type;
} token_t;

typedef struct {
    token_t *buffer;
    int size;
} tok_buf_t;

tok_buf_t grab_tokens(char *string);

#endif