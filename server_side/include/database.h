#ifndef DATABASE_HEADER
#define DATABASE_HEADER

#include <inttypes.h>
#include <stdio.h>
#include "file_ops.h"
#include "relation.h"

#define MAX_DATABASE_NAME_LENGTH 50
#define DEFAULT_PAGE_SIZE_BYTES 4096
#define MAX_RELATION_NAME_LENGTH 20

struct schema;
struct relation_header;

enum query_op;

enum database_state {
    SAVED_IN_FILE = 0,
    NEW
};

struct page_header {
    bool is_dirty;

    char relation_name[MAX_RELATION_NAME_LENGTH];

    uint16_t remaining_space;

    uint32_t page_number;
    uint32_t write_ptr;
    uint32_t real_number;
    uint32_t next_page_number;
};

struct database_header {
    char name[MAX_DATABASE_NAME_LENGTH];

    struct database* database;

    uint32_t relation_count;
    uint32_t page_count;
    uint32_t page_size;
    uint32_t last_page_number;
};

struct database {
    struct database_header* database_header;
    FILE* source_file;
};

bool is_enough_space(struct page_header* page_header, uint32_t required);

struct page_header* page_create(struct database_header* database_header, struct relation_header* relation_header);
struct page_header* page_add_real(struct database_header* database_header);
struct page_header* page_add(struct relation_header* relation_header, struct database_header* database_header);

struct database* database_get(const char *const file, const enum database_state state);
struct database* database_create_in_file(const char *const file);
struct database* database_get_from_file(const char *const file);

void database_close(struct database* database);

struct relation* relation_create(struct schema* schema, const char* relation_name, struct database*  database);
struct relation* relation_get(const char *const relation_name, struct database*  database);

void relation_close(struct relation* relation);

struct query* query_make(enum query_op operation, struct relation* relations, char* columns[], void* vals[], int32_t cnt);
struct query_join* query_join_make(struct relation* left, struct relation* right, char* left_column, char* right_column);

void query_execute(struct query *query, bool show_output);
void query_join_execute(struct query_join* query);

void query_close(struct query* query);
void query_join_close(struct query_join* query);

#endif