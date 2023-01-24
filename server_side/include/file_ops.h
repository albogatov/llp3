#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include "database.h"

enum content_type : int;
struct database_header;
struct column;
struct relation;
struct query_params;
struct page;
struct relation_header;
struct row;
struct page_header;
struct schema;

enum file_op_status  {
  OK = 0,
  ERROR
};
#ifdef __cplusplus
extern "C" {
#endif
bool is_relation_present(FILE *file, const size_t length, const char* name, struct relation_header* relation_header);

enum file_op_status file_update_last_page(FILE *file, uint32_t old_number, uint32_t new_);
enum file_op_status file_open(FILE **file, const char *const name, const char *const mode);
enum file_op_status file_close(FILE *file);

enum file_op_status page_header_write_real(FILE *file, struct database_header* database_header, struct relation_header* relation_header);

enum file_op_status relation_header_save_changes(FILE *file, struct relation_header* relation_header);
enum file_op_status relation_header_read(FILE *file, const char *const  relation_name, struct relation_header* relation_header, size_t relation_count);

enum file_op_status relation_page_write(FILE *file, struct page_header* page_header, struct schema* schema);

enum file_op_status relation_read_columns(FILE *file, struct relation* relation);

enum file_op_status row_write_to_page(FILE *file, uint32_t page_number, struct row* row);

enum file_op_status database_update_last_page(FILE *file, struct database_header* database_header, uint32_t new_);
enum file_op_status database_header_save_changes(FILE *file, struct database_header* database_header);
enum file_op_status database_header_read(FILE *file, struct database_header* database_header);
enum file_op_status database_save_to_file(FILE *file, struct database_header* database_header, struct page_header* page_header_real);

void row_remove(struct relation *relation, uint32_t ptr, uint32_t page_number);

char * select_execute(FILE *file, struct relation *relation, uint32_t offset, uint16_t column_size, void *value,
                      enum content_type type, int32_t row_count, bool show_output, char *buf);
char * update_execute(FILE *file, struct relation *relation, struct query_params *first, struct query_params *second,
                      void **values, bool show_output, char *buf);
char * delete_execute(FILE *file, struct relation *relation, struct query_params *query, void *value, char *buf);

void update_query(char *begin, void *value, struct query_params *query, struct relation *relation, uint32_t ptr,
                  uint32_t page_number, bool show_output);

char * query_join_attempt(FILE *file, struct relation *left, struct relation *right, struct query_params *left_query,
                          struct query_params *right_query, char *left_row, char *buf);
char * query_join(FILE *file, struct relation *left, struct relation *right, struct query_params *left_query,
                  struct query_params *right_query, char *buf);

char * query_join_output(char *begin_left, char *begin_right, struct relation *left, struct relation *right,
                         uint32_t left_offset, uint32_t right_offset, char *buf);

long int database_get_size(FILE* file);
#ifdef __cplusplus
}
#endif
#endif