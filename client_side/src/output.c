#include "../include/output.h"

void output(ast_node* root) {
    if (root == NULL) {
        printf("NULL\n");
        return;
    }
    char* res = to_string_general(root, 0);
    printf("%s\n", res);
    free(res);
    close_tree(root);
}

