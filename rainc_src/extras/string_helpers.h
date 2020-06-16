#ifndef RAINC_STRING_HELPERS_H
#define RAINC_STRING_HELPERS_H

char *new_string(int size);
char *concat(char *str1, char *str2);
void destroy_string(char *str);
int is_digit(char c);
int is_letter(char c);

#endif