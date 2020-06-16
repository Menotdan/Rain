#ifndef RAIN_PARSER_H
#define RAIN_PARSER_H
#include <parsing/tokens.h>

typedef struct {
    int type_id_in_scope;
    char *name;
    int is_local;
    int local_rbp_offset;
} variable_t;

typedef struct {
    int type_id;
    int type_size;
    char *type_name;
} type_t;

typedef struct {
    char *name;
    type_t type;
} class_member_t;

typedef struct {
    class_member_t *members;
    type_t class_type;
} class_t;

typedef struct scope {
    struct scope *parent_scope;
    type_t *types;
    variable_t *variables;
    class_t *classes;
} scope_t;

#endif