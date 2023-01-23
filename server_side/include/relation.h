#ifndef RELATION_H
#define RELATION_H

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "file_ops.h"
#include "query.h"

#define MAX_NAME_LENGTH 20

struct query;

enum content_type : int {
    INTEGER = 0,
    BOOLEAN,
    DOUBLE,
    VARCHAR
};

struct column {
    char name[MAX_NAME_LENGTH];
    enum content_type content_type;
    struct column* next;
    uint32_t size;
};

struct query_params {
    char name[MAX_NAME_LENGTH];
    enum content_type content_type;
    uint16_t size;
    uint32_t offset;
};


struct schema {
    struct column* start;
    struct column* end;
    uint64_t length;
    uint16_t count;
};

struct relation_header {
    bool is_available;

    char name[MAX_NAME_LENGTH];

    struct database* database;
    struct relation* relation;
    struct schema schema;

    uint32_t page_number_first;
    uint32_t page_number_last;
    uint32_t page_count;
    uint32_t real_number;
};

struct relation {
    struct relation_header* relation_header;
    struct schema* schema;
};

struct row_header {
    bool is_available;
};

struct row {
    struct row_header* row_header;
    struct relation* relation;
    void** data;
};

struct column* column_delete(struct column* current, const char* name, struct schema* schema);
struct column* column_create(const char* name, enum content_type content_type);
struct column* column_create_varchar(const char* name, enum content_type content_type, uint16_t size);

void column_list_delete(struct column* list);
void column_add(struct schema* schema, const char* name, enum content_type content_type);
void column_add_varchar(struct schema* schema, const char* name, enum content_type content_type, uint16_t size);

int32_t column_length_varchar(const struct column* list, const size_t length, const char* name);
int32_t column_get_offset(const struct column *list, char *name, const size_t length);

uint32_t column_presence_count(const struct column* list, const size_t length, const char* name);

void attribute_add(struct row* row, char* name, enum content_type content_type, void* value);

struct schema* schema_create();
struct schema* schema_add_column(struct schema* schema, const char* name, enum content_type content_type);
struct schema* schema_add_column_varchar(struct schema* schema, const char* name, enum content_type content_type, uint16_t size);

void schema_close(struct schema* schema);

struct row* row_create(struct relation* relation);

void row_close(struct row* row);
void row_insert(struct row* row);
void row_select(struct query *query, bool show_output);
void row_update(struct query *query, bool show_output);
void row_delete(struct query *query, bool show_output);

void integer_add(struct row* row, int32_t value, uint32_t offset);
void boolean_add(struct row* row, bool value, uint32_t offset);
void varchar_add(struct row* row, char* value, uint32_t offset, uint32_t length);
void double_add(struct row* row, double value, uint32_t offset);

#endif
