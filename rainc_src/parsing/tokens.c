#include "tokens.h"
#include <stdlib.h>
#include <string.h>
#include <extras/string_helpers.h>
#include <stdio.h>

int bump_tokbuf(tok_buf_t *buf) {
    int ret = buf->size;

    buf->size++;
    buf->buffer = realloc(buf->buffer, buf->size * sizeof(token_t));

    return ret;
}

void add_single_char_token(tok_buf_t *buf, char c, token_type_t type) {
    token_t tok = {new_string(1), type};
    tok.contents[0] = c;
    int index = bump_tokbuf(buf);
    buf->buffer[index] = tok;
}

void add_token(tok_buf_t *buf, char *c, token_type_t type) {
    token_t tok = {new_string(strlen(c)), type};
    strcpy(tok.contents, c);
    int index = bump_tokbuf(buf);
    buf->buffer[index] = tok;
}

void replace_previous_token(tok_buf_t *buf, char *c, token_type_t type) {
    destroy_string(buf->buffer[buf->size - 1].contents);

    token_t tok = {new_string(strlen(c)), type};
    strcpy(tok.contents, c);
    buf->buffer[buf->size - 1] = tok;
}

tok_buf_t grab_tokens(char *string) {
    tok_buf_t tok_buf = {0, 0};

    int len = strlen(string);

    /* State */
    char *current_iden_str;
    char *current_number_str;
    int last_was_number = 0;
    int last_was_iden = 0;

    /* Loop */
    for (int i = 0; i < len; i++) {
        if (is_letter(string[i]) || string[i] == '_' || string[i] == '-' || (last_was_iden && is_digit(string[i]))) {
            if (last_was_iden) {
                int old_len = strlen(current_iden_str);
                current_iden_str = realloc(current_iden_str, old_len + 2);
                current_iden_str[old_len] = string[i];
            } else {
                current_iden_str = new_string(1);
                current_iden_str[0] = string[i];
            }
            last_was_iden = 1;
        } else if (is_digit(string[i])) {
            if (last_was_number) {
                int old_len = strlen(current_number_str);
                current_number_str = realloc(current_number_str, old_len + 2);
                current_number_str[old_len] = string[i];
            } else {
                current_number_str = new_string(1);
                current_number_str[0] = string[i];
            }
            last_was_number = 1;
        } else {
            /* Add tokens that existed previously */
            if (last_was_iden) {
                last_was_iden = 0;
                token_t tok = {current_iden_str, TOKEN_IDENTIFIER};
                int index = bump_tokbuf(&tok_buf);
                tok_buf.buffer[index] = tok;
            }
            if (last_was_number) {
                last_was_number = 0;
                token_t tok = {current_number_str, TOKEN_NUMBER};
                int index = bump_tokbuf(&tok_buf);
                tok_buf.buffer[index] = tok;
            }

            /* Switch */
            switch (string[i]) {
                case '\t':
                    printf("got a tab\n");

                    add_single_char_token(&tok_buf, '\t', TOKEN_WHITESPACE);
                    break;
                case ' ':
                    printf("got a space\n");

                    add_single_char_token(&tok_buf, ' ', TOKEN_WHITESPACE);
                    break;
                case '\n':
                    printf("got a newline\n");

                    add_single_char_token(&tok_buf, '\n', TOKEN_WHITESPACE);
                    break;
                case '>':
                    printf("got a >\n");
                    if (tok_buf.size > 0 && tok_buf.buffer[tok_buf.size - 1].contents[0] == '>' && tok_buf.buffer[tok_buf.size - 1].type == TOKEN_CONDITIONAL) {
                        printf("last token was a >, changing to an operator\n");

                        replace_previous_token(&tok_buf, ">>", TOKEN_OPERATOR);
                    } else {
                        add_single_char_token(&tok_buf, '>', TOKEN_CONDITIONAL);
                    }
                    break;
                case '<':
                    printf("got a <\n");
                    if (tok_buf.size > 0 && tok_buf.buffer[tok_buf.size - 1].contents[0] == '<' && tok_buf.buffer[tok_buf.size - 1].type == TOKEN_CONDITIONAL) {
                        printf("last token was a <, changing to an operator\n");

                        replace_previous_token(&tok_buf, "<<", TOKEN_OPERATOR);
                    } else {
                        add_single_char_token(&tok_buf, '<', TOKEN_CONDITIONAL);
                    }
                    break;
                case '/':
                    if (tok_buf.size > 0 && tok_buf.buffer[tok_buf.size - 1].contents[0] == '/' && tok_buf.buffer[tok_buf.size - 1].type == TOKEN_OPERATOR) {
                        replace_previous_token(&tok_buf, "//", TOKEN_COMMENT_INDICATOR);
                    } else if (tok_buf.size > 0 && tok_buf.buffer[tok_buf.size - 1].contents[0] == '*' && tok_buf.buffer[tok_buf.size - 1].type == TOKEN_OPERATOR) {
                        replace_previous_token(&tok_buf, "*/", TOKEN_COMMENT_INDICATOR);
                    } else {
                        add_single_char_token(&tok_buf, '/', TOKEN_OPERATOR);
                    }
                    break;
                case '*':
                    if (tok_buf.size > 0 && tok_buf.buffer[tok_buf.size - 1].contents[0] == '/' && tok_buf.buffer[tok_buf.size - 1].type == TOKEN_OPERATOR) {
                        replace_previous_token(&tok_buf, "/*", TOKEN_COMMENT_INDICATOR);
                    } else {
                        add_single_char_token(&tok_buf, '*', TOKEN_OPERATOR);
                    }
                    break;
                case '+':
                    add_single_char_token(&tok_buf, '+', TOKEN_OPERATOR);
                    break;
                case '-':
                    add_single_char_token(&tok_buf, '-', TOKEN_OPERATOR);
                    break;
                case '"':
                    add_single_char_token(&tok_buf, '"', TOKEN_SEPERATOR);
                    break;
                case ',':
                    add_single_char_token(&tok_buf, ',', TOKEN_SEPERATOR);
                    break;
                case '&':
                    if (tok_buf.size > 0 && tok_buf.buffer[tok_buf.size - 1].contents[0] == '&' && tok_buf.buffer[tok_buf.size - 1].type == TOKEN_OPERATOR) {
                        replace_previous_token(&tok_buf, "&&", TOKEN_CONDITIONAL);
                    } else {
                        add_single_char_token(&tok_buf, '&', TOKEN_OPERATOR);
                    }
                    break;
                case '|':
                    if (tok_buf.size > 0 && tok_buf.buffer[tok_buf.size - 1].contents[0] == '|' && tok_buf.buffer[tok_buf.size - 1].type == TOKEN_OPERATOR) {
                        replace_previous_token(&tok_buf, "||", TOKEN_CONDITIONAL);
                    } else {
                        add_single_char_token(&tok_buf, '|', TOKEN_OPERATOR);
                    }
                    break;
                case '{':
                    add_single_char_token(&tok_buf, '{', TOKEN_SEPERATOR);
                    break;
                case '}':
                    add_single_char_token(&tok_buf, '}', TOKEN_SEPERATOR);
                    break;
                case '(':
                    add_single_char_token(&tok_buf, '(', TOKEN_SEPERATOR);
                    break;
                case ')':
                    add_single_char_token(&tok_buf, ')', TOKEN_SEPERATOR);
                    break;
                case '~':
                    add_single_char_token(&tok_buf, '~', TOKEN_OPERATOR);
                    break;
                case '^':
                    add_single_char_token(&tok_buf, '^', TOKEN_OPERATOR);
                    break;
                case ';':
                    add_single_char_token(&tok_buf, ';', TOKEN_SEPERATOR);
                    break;
                case ':':
                    add_single_char_token(&tok_buf, ':', TOKEN_SEPERATOR);
                    break;
                case '=':
                    if (tok_buf.size > 0 && tok_buf.buffer[tok_buf.size - 1].type == TOKEN_OPERATOR) {
                        char c = tok_buf.buffer[tok_buf.size - 1].contents[0];
                        if (c == '+' || c == '|' || c == '&' || c == '-' || c == '*' || c == '/' || c == '~' || c == '^') {
                            char temp_buf[3] = {c, '=', '\0'};
                            replace_previous_token(&tok_buf, temp_buf, TOKEN_OPERATOR);
                            break;
                        }
                    }
                    add_single_char_token(&tok_buf, '=', TOKEN_OPERATOR);
                    break;
                default:
                    break;
            }
        }
    }

    return tok_buf;
}