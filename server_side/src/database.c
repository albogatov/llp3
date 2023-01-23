#include "../include/database.h"

struct database* database_get(const char *const file, const enum database_state state) {
    if (state == SAVED_IN_FILE)
        return database_get_from_file(file);
    else
        return database_create_in_file(file);
}

void database_close(struct database* database) {
    file_close(database->source_file);
    free(database->database_header);
    free(database);
}

struct database* database_create_in_file(const char *const file) {
    struct database* database = malloc(sizeof(struct database) + sizeof(struct database_header));

    FILE *f = NULL;

    if (file_open(&f, file, "wb+") == OK) {
        printf("File created\n");
    } else {
        printf("Couldn't create file\n");
        return NULL;
    }

    struct database_header* new = malloc(sizeof(struct database_header));
    strncpy(new->name, "", MAX_DATABASE_NAME_LENGTH);
    strncpy(new->name, file, strlen(file));
    new->relation_count = 0;
    new->page_count = 0;
    new->page_size = DEFAULT_PAGE_SIZE_BYTES;

    new->last_page_number = 0;
    new->database = database;

    struct page_header* page_header = page_add_real(new);

    database->source_file = f;
    database->database_header = new;

    database_save_to_file(f, new, page_header);

    return database;
}

struct database* database_get_from_file(const char *const file) {
    struct database* database = malloc(sizeof(struct database));
    struct database_header* header = malloc(sizeof(struct database_header));

    FILE *f = NULL;
    if (file_open(&f, file, "rb+") == OK)
        printf("File opened\n");
    else {
        printf("Could not open file\n");
        return NULL;
    }

    enum file_op_status result = database_header_read(f, header);
    if (result == OK) {
        header->database = database;
        database->source_file = f;
        database->database_header = header;
        return database;
    } else {
        printf("Error while reading existing database");
        return NULL;
    }
}

struct relation* relation_create(struct schema* schema, const char* relation_name, struct database*  database) {
    struct relation_header* header = malloc(sizeof(struct relation_header));
    if (is_relation_present(database->source_file, database->database_header->relation_count, relation_name, header)) {
        printf("Can't create two relations with the same name");
        free(header);
        return NULL;
    }
    struct relation* new = malloc(sizeof(struct relation));
    new->schema = schema;

    struct relation_header* relation_header = malloc(sizeof(struct relation_header));
    strncpy(relation_header->name, "", MAX_NAME_LENGTH);
    strncpy(relation_header->name, relation_name, strlen(relation_name));
    relation_header->page_number_first = 0;
    relation_header->page_number_last = 0;
    relation_header->database = database;
    relation_header->relation = new;
    relation_header->schema = *schema;
    relation_header->page_count = 0;
    relation_header->is_available = true;

    struct page_header* new_page = page_add(relation_header, database->database_header);
    new->relation_header = relation_header;

    database->database_header->relation_count++;

    relation_header->real_number = database->database_header->relation_count;

    page_header_write_real(database->source_file, database->database_header, relation_header);

    database_header_save_changes(database->source_file, database->database_header);

    relation_page_write(database->source_file, new_page, new->schema);

    return new;
}

struct relation* relation_get(const char *const relation_name, struct database* database) {
    struct relation* new = malloc(sizeof(struct relation));
    struct relation_header* relation_header = malloc(sizeof(struct relation_header));
    struct schema* schema = malloc(sizeof(struct schema));

    if (relation_header_read(database->source_file, relation_name, relation_header, database->database_header->relation_count) == OK) {
        new->schema = schema;
        new->relation_header = relation_header;
        relation_read_columns(database->source_file, new);
        new->relation_header->database = database;
        return new;
    } else {
        printf("Could not get relation\n");
        return NULL;
    }
}

void relation_close(struct relation* relation) {
    //free(created_relation->schema->start);
    free(relation->relation_header);
    //free(created_relation->schema);
    free(relation);
}

struct page_header* page_create(struct database_header* database_header, struct relation_header* relation_header) {
    struct page_header* new = malloc(sizeof (struct page_header));
    if (!new) {
        return NULL;
    }

    new->write_ptr = 0;
    new->next_page_number = 0;
    new->is_dirty = false;
    new->remaining_space = DEFAULT_PAGE_SIZE_BYTES;
    new->page_number = database_header->page_count;

    if (!relation_header) {
        strncpy(new->relation_name, "", MAX_RELATION_NAME_LENGTH);
    } else {
        new->real_number = relation_header->real_number;
        strncpy(new->relation_name, "", MAX_RELATION_NAME_LENGTH);
        strncpy(new->relation_name, relation_header->name, MAX_RELATION_NAME_LENGTH);
    }
    
    return new;
}

struct page_header* page_add_real(struct database_header* database_header) {

    database_header->page_count++;

    struct page_header* new = page_create(database_header, NULL);

    if (new) {

        if (database_header->last_page_number != 0) {
            database_update_last_page(database_header->database->source_file, database_header,
                                      new->page_number);
        }

        database_header->last_page_number = new->page_number;

        if (database_header->page_count != 1) {
            database_header_save_changes(database_header->database->source_file, database_header);
        }

        return new;

    } else return NULL;
}

struct page_header* page_add(struct relation_header* relation_header, struct database_header* database_header) {
    database_header->page_count++;
    relation_header->page_count++;

    struct page_header* new = page_create(database_header, relation_header);

    if (new) {
        if (relation_header->page_number_last != 0) {
            file_update_last_page(relation_header->database->source_file, relation_header->page_number_last,
                                  new->page_number);
        } else {
            relation_header->page_number_first = new->page_number;
        }

        relation_header->page_number_last = new->page_number;

        if (relation_header->page_count != 1) {
            relation_header_save_changes(database_header->database->source_file, relation_header);
            database_header_save_changes(database_header->database->source_file, database_header);
        }

        return new;
    }
    return NULL;
}

struct query* query_make(enum query_op operation, struct relation* relations, char* columns[], void* vals[], int32_t cnt) {
    struct query* new = malloc(sizeof(struct query));
    new->operation = operation;
    new->name = columns;
    new->value = vals;
    new->number = cnt;
    new->relation = relations;
    return new;
}

struct query_join* query_join_make(struct relation* left, struct relation* right, char* left_column, char* right_column) {
    struct query_join* new = malloc(sizeof(struct query_join));
    new->left = left;
    new->right = right;
    new->left_column = left_column;
    new->right_column = right_column;
    return new;
}

char * query_execute(struct query *query, bool show_output, char *buf) {

    switch (query->operation) {
        case SELECT:
            return row_select(query, show_output, buf);
            break;
        case DELETE:
            row_delete(query, show_output);
            return NULL;
            break;
        case UPDATE:
            return row_update(query, show_output, buf);
            break;
    }

}

char * query_join_execute(struct query_join *query, char *buf) {
    bool is_column_present_one = false;
    bool is_column_present_two = false;
    char name_one[MAX_NAME_LENGTH];
    char name_two[MAX_NAME_LENGTH];

    enum content_type type_one;
    enum content_type type_two;
    uint16_t size_one;
    uint16_t size_two;

    struct column* col = query->left->schema->start;

    for (size_t i = 0; i < query->left->schema->count; i++) {
        if (strcmp(col->name, query->left_column) == 0) {
            type_one = query->left->schema->start[i].content_type;
            is_column_present_one = true;
            strncpy(name_one, query->left->schema->start[i].name, MAX_NAME_LENGTH);
            size_one = query->left->schema->start[i].size;
            break;
        } else col = col->next;
    }

    col = query->right->schema->start;

    for (size_t i = 0; i < query->right->schema->count; i++) {
        if (strcmp(col->name, query->right_column) == 0) {
            type_two = query->right->schema->start[i].content_type;
            is_column_present_two = true;
            strncpy(name_two, query->right->schema->start[i].name, MAX_NAME_LENGTH);
            size_two = query->right->schema->start[i].size;
            break;
        } else col = col->next;
    }

    if (is_column_present_one && is_column_present_two) {
        struct query_params* query_one = malloc(sizeof(struct query_params));
        struct query_params* query_two = malloc(sizeof(struct query_params));

        uint32_t offset_one = column_get_offset(query->left->schema->start, name_one, query->left->schema->count);
        uint32_t offset_two = column_get_offset(query->right->schema->start,
                                                name_two, query->right->schema->count);

        if (offset_one != -1 && offset_two != -1) {
            query_one->content_type = type_one;
            query_one->size = size_one;
            query_one->offset = offset_one;
            strncpy(query_one->name, "", MAX_NAME_LENGTH);
            strncpy(query_one->name, name_one, MAX_NAME_LENGTH);

            query_two->content_type = type_two;
            query_two->size = size_two;
            query_two->offset = offset_two;
            strncpy(query_two->name, "", MAX_NAME_LENGTH);
            strncpy(query_two->name, name_two, MAX_NAME_LENGTH);

            buf = query_join(query->left->relation_header->database->source_file, query->left, query->right, query_one,
                       query_two, buf);
        }

        free(query_one);
        free(query_two);

    } else printf("Couldn't complete query, attributes not present\n");
    return buf;
}

void query_close(struct query* query) {
    free(query);
}

void query_join_close(struct query_join* query) {
    free(query);
}

bool is_enough_space(struct page_header* page_header, uint32_t required) {
    if (page_header->remaining_space >= required) {
        return true;
    } else return false;
}



