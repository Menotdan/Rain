#include "file_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <extras/string_helpers.h>

char *read_file(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        return (void *) 0;
    }
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char *ret = new_string(size);
    fread(ret, size, 1, fp);
    fclose(fp);

    return ret;
}