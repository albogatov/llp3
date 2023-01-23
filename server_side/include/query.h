#ifndef QUERY_H
#define QUERY_H

#include <inttypes.h>
#include <stdlib.h>
#include "relation.h"

#define MAX_RELATION_NAME_LENGTH 20
#define MAX_NAME_LENGTH 20

enum query_op : int {
    SELECT = 0,
    UPDATE,
    DELETE,
};

struct query {
    char** name;

    enum query_op operation;

    int32_t number;

    void** value;

    struct relation* relation;
};

struct query_join {
    struct relation* left;
    struct relation* right;
    char* left_column;
    char* right_column;
};

#endif