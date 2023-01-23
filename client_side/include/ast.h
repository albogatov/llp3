#ifndef AST_H
#define AST_H
#include <stdbool.h>
#include <string.h>

enum ast_node_type {
    NAME_NODE,
    STRING_NODE,
    NUMBER_NODE,
    FLOAT_NUMBER_NODE,
    BOOLEAN_NODE,
    LIST_NODE,
    PAIR_NODE,
    FOR_NODE,
    INSERT_NODE,
    REMOVE_NODE,
    UPDATE_NODE,
    CREATE_NODE,
    DROP_NODE,
    FILTER_NODE,
    COMPARE_NODE,
    TYPE_NODE
};

enum type {
    STR_DATA,
    NUMERIC_DATA,
    FLOAT_NUMERIC_DATA,
    BOOLEAN_DATA
};

static const char* type_repr_[] = {
        "FLOAT_NUMBER",
        "STRING",
        "NUMBER",
        "BOOLEAN"
};

enum logic_op_type {
    AND_OP,
    OR_OP
};

static const char* logic_repr_[] = {
        "&&",
        "||"
};

enum compare_op_type {
    GREATER = 1,
    GREATER_OR_EQUAL,
    LESS,
    LESS_OR_EQUAL,
    EQUAL,
    NOT_EQUAL,
    NO_COMPARE
};

enum compare_op_type switch_cmp_mode(enum compare_op_type val);

static const char* cmp_op_repr_[] = {
        "GREATER",
        "GREATER_OR_EQUAL",
        "LESS",
        "LESS_OR_EQUAL",
        "NO_COMPARE",
        "EQUAL",
        "NOT_EQUAL",
        "NO_COMPARE"
};

union fields {
    char* string;
    enum logic_op_type logic_op_type_type;
    enum compare_op_type comp_op_type;
    int number;
    float float_number;
    bool boolean;
    enum type data_type;
};

typedef struct ast_node ast_node;
typedef union fields fields;
typedef enum ast_node_type ast_node_type;

struct ast_node {
    ast_node_type type;
    fields fields_one;
    fields fields_two;
    ast_node* first;
    ast_node* second;
    ast_node* third;
};

ast_node* new_name(const char* v_first, const char* v_second);
ast_node* new_string(ast_node *first, const char *v_second);
ast_node* new_number(int v_first);
ast_node* new_float_number(float v_first);
ast_node* new_bool(bool v_first);
ast_node* new_type(enum type v_first);
ast_node* new_list(ast_node* first, ast_node* second);
ast_node* new_pair(const char* v_first, ast_node* second);
ast_node* new_select(const char* v_first, ast_node* first, const char* v_second, ast_node* second, ast_node* third);
ast_node *new_delete(const char *v_first, ast_node *first, ast_node *second);
ast_node* new_insert(const char* v_first, ast_node* first);
ast_node* new_update(const char* v_first, ast_node* first, ast_node* second);
ast_node* new_create(const char* v_first, ast_node* first);
ast_node* new_drop(const char* v_first);
ast_node* new_where(enum logic_op_type v_first, ast_node* first, ast_node* second);
ast_node* new_compare(enum compare_op_type v_first, ast_node* first, ast_node* second);

void close_tree(ast_node* root);

#endif