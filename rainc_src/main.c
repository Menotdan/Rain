#include <stdio.h>

#include <extras/string_helpers.h>
#include <extras/file_io.h>
#include <parsing/tokens.h>

int main(int argc, char **argv) {
    printf("rainc version 0.001\n");

    if (argc != 2) {
        printf("expected one argument\n");
        return 1;
    }

    char *code = read_file(argv[1]);
    if (!code) {
        printf("failed to read file %s\n", argv[1]);
        return 1;
    }

    tok_buf_t tokens = grab_tokens(code);
    for (int i = 0; i < tokens.size; i++) {
        if (tokens.buffer[i].type != TOKEN_WHITESPACE) {
            printf("Token %s with type %d\n", tokens.buffer[i].contents, tokens.buffer[i].type);
        }
    }
    
    return 0;
}