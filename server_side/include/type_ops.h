#ifndef TYPE_OPS_H
#define TYPE_OPS_H

#include <stdbool.h>
#include <inttypes.h>
#include "database.h"
#ifdef __cplusplus
extern "C" {
#endif
void integer_update(char* row_ptr, void* value, uint32_t offset);
void boolean_update(char* row_ptr, void* value, uint32_t offset);
void varchar_update(char* row_ptr, void* value, uint32_t offset, uint16_t column_size);
void double_update(char* row_ptr, void* value, uint32_t offset);

char * integer_output(char *begin, uint32_t offset, char *buf);
char * boolean_output(char *begin, uint32_t offset, char *buf);
char * varchar_output(char *begin, uint32_t offset, char *buf);
char * double_output(char *begin, uint32_t offset, char *buf);
char * data_output(char *begin, struct column *columns, uint16_t length, char *buf);

bool integer_compare(char* row_ptr, void* value, uint32_t offset);
bool boolean_compare(char* row_ptr, void* value, uint32_t offset);
bool varchar_compare(char* row_ptr, void* value, uint32_t offset, uint16_t column_size);
bool double_compare(char* row_ptr, void* value, uint32_t offset);

bool integer_query_join_compare(char* left_row, char* right_row, struct query_params* left_query, struct query_params* right_query, struct relation* left, struct relation* right);
bool boolean_query_join_compare(char* left_row, char* right_row, struct query_params* left_query, struct query_params* right_query, struct relation* left, struct relation* right);
bool varchar_query_join_compare(char* left_row, char* right_row, struct query_params* left_query, struct query_params* right_query, struct relation* left, struct relation* right);
bool double_query_join_compare(char* left_row, char* right_row, struct query_params* left_query, struct query_params* right_query, struct relation* left, struct relation* right);

char* safe_string_copy (const char* from);
void safe_string_concatenation(char** str, const char * str2);
#ifdef __cplusplus
}
#endif
#endif
