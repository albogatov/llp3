#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/string_utils.h"

char* safe_string_copy (const char* from) {
    int count = strlen(from);
    char* ret = malloc(sizeof(char) * (count + 1));
    strcpy(ret, from);
    return ret;
}

void safe_string_concatenation (char** str, const char * str2) {
    char* str1 = *str;
    int first_len = strlen(str1), second_len = strlen(str2);
    char * new_str = malloc(sizeof(char) * (first_len + second_len + 1));
    strcat(new_str, str1);
    strcat(new_str, str2);
    free(str1);
    *str = new_str;
}

void indent(int indentation, char** src) {
    for (int i = 0; i < indentation; i++) {
        safe_string_concatenation(src, "   ");
    }
}


char* print_string_node(ast_node* node, int indentation) {
    char* output = malloc(sizeof(char));
    indent(indentation, &output);
    safe_string_concatenation(&output, "STRING { ");
    safe_string_concatenation(&output, node->fields_one.string);
    safe_string_concatenation(&output, " }");
    return output;
}

char* print_column_node(ast_node* node, int indentation) {
    char* output = malloc(sizeof(char));
    indent(indentation, &output);
    safe_string_concatenation(&output, "COLUMN {\n");
    indent(indentation + 1, &output);
    if (node->fields_one.string) {
        safe_string_concatenation(&output, "table: ");
        safe_string_concatenation(&output, node->fields_one.string);
        safe_string_concatenation(&output, "\n");
        indent(indentation + 1, &output);
    }
    if (node->fields_two.string) {
        safe_string_concatenation(&output, "column_name: ");
        safe_string_concatenation(&output, node->fields_two.string);
        safe_string_concatenation(&output, "\n");
        indent(indentation, &output);
    }
    safe_string_concatenation(&output, "}");
    return output;
}

char* print_integer_node(ast_node* node, int indentation) {
    char* output = malloc(sizeof(char));
    indent(indentation, &output);
    safe_string_concatenation(&output, "NUMBER { ");
    int length = snprintf(NULL, 0, "%d", node->fields_one.number);
    char* str = malloc( length + 1 );
    snprintf(str, length + 1, "%d", node->fields_one.number);
    safe_string_concatenation(&output, str);
    free(str);
    safe_string_concatenation(&output, " }");
    return output;
}

char* print_float_node(ast_node* node, int indentation) {
    char* output = malloc(sizeof(char));
    indent(indentation, &output);
    safe_string_concatenation(&output, "FLOAT { ");
    int length = snprintf(NULL, 0, "%f", node->fields_one.float_number);
    char* str = malloc( length + 1 );
    snprintf(str, length + 1, "%f", node->fields_one.float_number);
    safe_string_concatenation(&output, str);
    free(str);
    safe_string_concatenation(&output, " }");
    return output;
}

char* print_boolean_node(ast_node* node, int indentation) {
    char* output = malloc(sizeof(char));
    indent(indentation, &output);
    safe_string_concatenation(&output, "BOOLEAN { ");
    safe_string_concatenation(&output, node->fields_one.boolean ? "TRUE" : "FALSE");
    safe_string_concatenation(&output, " }");
    return output;
}

char* print_type_node(ast_node* node, int indentation) {
    char* output = malloc(sizeof(char));
    indent(indentation, &output);
    safe_string_concatenation(&output, "TYPE { ");
    safe_string_concatenation(&output, type_repr_[node->fields_one.data_type]);
    safe_string_concatenation(&output, " }");
    return output;
}

char* print_list_node(ast_node* node, int indentation) {
    char* output = malloc(sizeof(char));
    indent(indentation, &output);
    safe_string_concatenation(&output, "VALUE_LIST {\n");
    ast_node* next = node;
    char* ptr;
    while (next != NULL) {
        ptr = to_string_general(next->first, indentation + 1);
        safe_string_concatenation(&output, ptr);
        free(ptr);
        safe_string_concatenation(&output, ";\n");
        next = next->second;
    }
    indent(indentation, &output);
    safe_string_concatenation(&output, "}");
    return output;
}

char* print_pair_node(ast_node* node, int indentation) {
    char* output = malloc(sizeof(char));
    indent(indentation, &output);
    safe_string_concatenation(&output, "PAIR {\n");
    indent(indentation + 1, &output);
    safe_string_concatenation(&output, "column_name: ");
    safe_string_concatenation(&output, node->fields_one.string);
    safe_string_concatenation(&output, "\n");
    char* pair = to_string_general(node->second, indentation + 1);
    safe_string_concatenation(&output, pair);
    free(pair);
    safe_string_concatenation(&output, "\n");
    indent(indentation, &output);
    safe_string_concatenation(&output, "}");
    return output;
}

char* print_select_node(ast_node* node, int indentation) {
    char* output = malloc(sizeof(char));
    indent(indentation, &output);
    safe_string_concatenation(&output, "FOR {\n");
    char* ptr;
    indent(indentation + 1, &output);
    safe_string_concatenation(&output, "COLUMNS: ");
    if (node->third != NULL) {
        safe_string_concatenation(&output, "\n");
        ptr = to_string_general(node->third, indentation + 2);
        safe_string_concatenation(&output, ptr);
        free(ptr);
    }
    safe_string_concatenation(&output, "\n");
    indent(indentation + 1, &output);
    safe_string_concatenation(&output, "TABLE: ");
    safe_string_concatenation(&output, node->fields_one.string);
    safe_string_concatenation(&output, "\n");
    indent(indentation + 1, &output);
    safe_string_concatenation(&output, "JOIN: ");
    if (node->fields_two.string != NULL) {
        safe_string_concatenation(&output, node->fields_two.string);
    } else {
        safe_string_concatenation(&output, "NULL");
    }
    safe_string_concatenation(&output, "\n");
    indent(indentation + 1, &output);
    safe_string_concatenation(&output, "FOR: ");
    if (node->second != NULL) {
        safe_string_concatenation(&output, "\n");
        ptr = to_string_general(node->second, indentation + 2);
        safe_string_concatenation(&output, ptr);
        free(ptr);
    } else {
        safe_string_concatenation(&output, "NULL");
    }
    safe_string_concatenation(&output, "\n");
    indent(indentation + 1, &output);
    safe_string_concatenation(&output, "FILTER: ");
    if (node->first != NULL) {
        safe_string_concatenation(&output, "\n");
        ptr = to_string_general(node->first, indentation + 2);
        safe_string_concatenation(&output, ptr);
        free(ptr);
    } else {
        safe_string_concatenation(&output, "NULL");
    }
    safe_string_concatenation(&output, "\n");
    indent(indentation, &output);
    safe_string_concatenation(&output, "}");
    return output;
}

char* print_delete_node(ast_node* node, int indentation) {
    char* output = malloc(sizeof(char));
    indent(indentation, &output);
    safe_string_concatenation(&output, "REMOVE {\n");
    indent(indentation + 1, &output);
    safe_string_concatenation(&output, "TABLE: ");
    safe_string_concatenation(&output, node->fields_one.string);
    safe_string_concatenation(&output, "\n");
    char* ptr;
    indent(indentation + 1, &output);
    safe_string_concatenation(&output, "KEYS: ");
    if (node->third != NULL) {
        safe_string_concatenation(&output, "\n");
        ptr = to_string_general(node->third, indentation + 2);
        safe_string_concatenation(&output, ptr);
        free(ptr);
    }
    safe_string_concatenation(&output, "\n");
    indent(indentation + 1, &output);
    safe_string_concatenation(&output, "CONDITION: ");
    if (node->first != NULL) {
        safe_string_concatenation(&output, "\n");
        ptr = to_string_general(node->first, indentation + 2);
        safe_string_concatenation(&output, ptr);
        free(ptr);
    } else {
        safe_string_concatenation(&output, "NULL");
    }
    safe_string_concatenation(&output, "\n");
    indent(indentation, &output);
    safe_string_concatenation(&output, "}");
    return output;
}

char* print_insert_node(ast_node* node, int indentation) {
    char* output = malloc(sizeof(char));
    indent(indentation, &output);
    safe_string_concatenation(&output, "INSERT {\n");
    indent(indentation + 1, &output);
    safe_string_concatenation(&output, "name: ");
    safe_string_concatenation(&output, node->fields_one.string);
    safe_string_concatenation(&output, "\n");
    char* ptr = to_string_general(node->first, indentation + 1);
    safe_string_concatenation(&output, ptr);
    free(ptr);
    safe_string_concatenation(&output, "\n");
    indent(indentation, &output);
    safe_string_concatenation(&output, "}");
    return output;
}

char* print_update_node(ast_node* node, int indentation) {
    char* output = malloc(sizeof(char));
    indent(indentation, &output);
    safe_string_concatenation(&output, "UPDATE {\n");
    indent(indentation + 1, &output);
    safe_string_concatenation(&output, "TABLE: ");
    safe_string_concatenation(&output, node->fields_one.string);
    safe_string_concatenation(&output, "\n");
    indent(indentation + 1, &output);
    char* ptr;
    safe_string_concatenation(&output, "VALUE_LIST:");
    if (node->second != NULL) {
        safe_string_concatenation(&output, "\n");
        ptr = to_string_general(node->second, indentation + 2);
        safe_string_concatenation(&output, ptr);
        free(ptr);
    } else {
        safe_string_concatenation(&output, "NULL");
    }
    safe_string_concatenation(&output, "\n");
    indent(indentation + 1, &output);
    safe_string_concatenation(&output, "CONDITION: ");

    if (node->first != NULL) {
        safe_string_concatenation(&output, "\n");
        ptr = to_string_general(node->first, indentation + 2);
        safe_string_concatenation(&output, ptr);
        free(ptr);
    } else {
        safe_string_concatenation(&output, "NULL");
    }
    safe_string_concatenation(&output, "\n");
    indent(indentation, &output);
    safe_string_concatenation(&output, "}");
    return output;
}

char* print_create_node(ast_node* node, int indentation) {
    char* output = malloc(sizeof(char));
    indent(indentation, &output);
    safe_string_concatenation(&output, "CREATE {\n");
    indent(indentation + 1, &output);
    safe_string_concatenation(&output, "NAME: ");
    safe_string_concatenation(&output, node->fields_one.string);
    safe_string_concatenation(&output, "\n");
    char* ptr = to_string_general(node->first, indentation + 1);
    safe_string_concatenation(&output, ptr);
    free(ptr);
    safe_string_concatenation(&output, "\n");
    indent(indentation, &output);
    safe_string_concatenation(&output, "}");
    return output;
}

char* print_drop_node(ast_node* node, int indentation) {
    char* output = malloc(sizeof(char));
    indent(indentation, &output);
    safe_string_concatenation(&output, "DROP { ");
    safe_string_concatenation(&output, "table: ");
    safe_string_concatenation(&output, node->fields_one.string);
    safe_string_concatenation(&output, " }");
    return output;
}

char* print_where_node(ast_node* node, int indentation) {
    char* output = malloc(sizeof(char));
    indent(indentation, &output);
    safe_string_concatenation(&output, "WHERE {\n");
    char* ptr = to_string_general(node->first, indentation + 1);
    safe_string_concatenation(&output, ptr);
    free(ptr);
    safe_string_concatenation(&output, "\n");
    indent(indentation + 1, &output);
    safe_string_concatenation(&output, logic_repr_[node->fields_one.logic_op_type_type]);
    safe_string_concatenation(&output, "\n");
    ptr = to_string_general(node->second, indentation + 1);
    safe_string_concatenation(&output, ptr);
    free(ptr);
    safe_string_concatenation(&output, "\n");
    indent(indentation, &output);
    safe_string_concatenation(&output, "}");
    return output;
}

char* print_compare_node(ast_node* node, int indentation) {
    char* output = malloc(sizeof(char));
    indent(indentation, &output);
    safe_string_concatenation(&output, "COMPARE {\n");
    char* ptr = to_string_general(node->first, indentation + 1);
    safe_string_concatenation(&output, ptr);
    free(ptr);
    safe_string_concatenation(&output, "\n");
    indent(indentation + 1, &output);
    safe_string_concatenation(&output, cmp_op_repr_[node->fields_one.comp_op_type]);
    safe_string_concatenation(&output, "\n");
    ptr = to_string_general(node->second, indentation + 1);
    safe_string_concatenation(&output, ptr);
    free(ptr);
    safe_string_concatenation(&output, "\n");
    indent(indentation, &output);
    safe_string_concatenation(&output, "}");
    return output;
}

typedef char*(*node_to_string)(ast_node*, int);

node_to_string node_to_string_functions[] = {
        print_column_node,
        print_string_node,
        print_integer_node,
        print_float_node,
        print_boolean_node,
        print_list_node,
        print_pair_node,
        print_select_node,
        print_insert_node,
        print_delete_node,
        print_update_node,
        print_create_node,
        print_drop_node,
        print_where_node,
        print_compare_node,
        print_type_node
};

char* to_string_general(ast_node* node, int indentation) {
    return node_to_string_functions[node->type](node, indentation);
}
