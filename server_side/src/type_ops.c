#include "../include/type_ops.h"

bool integer_compare(char* row_ptr, void* value, uint32_t offset) {
    int32_t comparing = (int32_t) *(row_ptr + offset);
    int32_t expecting = *((int32_t*) value);
    if (comparing == expecting) {
        return true;
    }
    return false;
}

bool boolean_compare(char* row_ptr, void* value, uint32_t offset) {
    bool comparing = (bool) *(row_ptr + offset);
    bool expecting = *((bool*) value);
    if (comparing == expecting) {
        return true;
    }
    return false;
}


bool double_compare(char* row_ptr, void* value, uint32_t offset) {
    double* comparing = (double*) (row_ptr + offset);
    double expecting = *((double*) value);
    if (*comparing == expecting) {
        return true;
    }
    return false;
}

bool varchar_compare(char* row_ptr, void* value, uint32_t offset, uint16_t column_size) {
    char* comparing = (char*) (row_ptr + offset);
    char* expecting = *((char**) value);
    if (strcmp(comparing, expecting) == 0) {
        return true;
    }
    return false;
}

void integer_update(char* row_ptr, void* value, uint32_t offset) {
    int32_t* original = (int32_t*) (row_ptr + offset);
    int32_t expecting = *((int32_t*) value);
    *original = expecting;
}

void boolean_update(char* row_ptr, void* value, uint32_t offset) {
    bool* original = (bool*) (row_ptr + offset);
    bool expecting = *((bool*) value);
    *original = expecting;
}

void double_update(char* row_ptr, void* value, uint32_t offset) {
    double* original = (double*) (row_ptr + offset);
    double expecting = *((double*) value);
    *original = expecting;
}

void varchar_update(char* row_ptr, void* value, uint32_t offset, uint16_t column_size) {
    char* original = (char*) row_ptr + offset;
    char* expecting = *((char**) value);
    strcpy(original, expecting);
}

char * integer_output(char *begin, uint32_t offset, char *buf) {
    int32_t* value = (int32_t*) (begin + offset);
    printf("%" PRId32 " ", *value);
    char buffer[128];
    sprintf(buffer, "%d", *value);
    if (buf) {
        safe_string_concatenation(&buf, buffer);
    }
    return buf;
}

char * boolean_output(char *begin, uint32_t offset, char *buf) {
    bool* value = (bool*) (begin + offset);
    printf("%s ", *value ? "T" : "F");
    char buffer[128];
    sprintf(buffer, "%s ", *value ? "T" : "F");
    if (buf) {
        safe_string_concatenation(&buf, buffer);
    }
    return buf;
}

char * double_output(char *begin, uint32_t offset, char *buf) {
    double* value = (double*) (begin + offset);
    printf("%f ", *value);
    char buffer[128];
    sprintf(buffer, "%f ", *value);
    if (buf) {
        safe_string_concatenation(&buf, buffer);
    }
    return buf;
}

char * varchar_output(char *begin, uint32_t offset, char *buf) {
    char* value = (char*) (begin + offset);
    printf("%s ", value);
    char buffer[128];
    sprintf(buffer, "%s ", value);
    if (buf) {
        safe_string_concatenation(&buf, buffer);
    }
    return buf;
}

char * data_output(char *begin, struct column *columns, uint16_t length, char *buf) {
    uint16_t cursor = 0;

    for (size_t i = 0; i < length; i++) {
        switch (columns[i].content_type) {
            case INTEGER:
                buf = integer_output(begin, cursor, buf);
                break;
            case BOOLEAN:
                buf = boolean_output(begin, cursor, buf);
                break;
            case VARCHAR:
                buf = varchar_output(begin, cursor, buf);
                break;
            case DOUBLE:
                buf = double_output(begin, cursor, buf);
                break;
        }
        cursor += columns[i].size;
    }
    safe_string_concatenation(&buf, "\n");
    printf("\n");
    return buf;
}

bool integer_query_join_compare(char* left_row, char* right_row, struct query_params* left_query, struct query_params* right_query, struct relation* left, struct relation* right) {
    int32_t value_one = (int32_t) *(left_row + left_query->offset);
    int32_t value_two = (int32_t) *(right_row + right_query->offset);
    if (value_one == value_two) {
        query_join_output(left_row, right_row, left, right, left_query->offset, right_query->offset);
        return true;
    } else return false;
}

bool boolean_query_join_compare(char* left_row, char* right_row, struct query_params* left_query, struct query_params* right_query, struct relation* left, struct relation* right) {
    bool value_one = (bool) *(left_row + left_query->offset);
    bool value_two = (bool) *(right_row + right_query->offset);
    if (value_one == value_two) {
        query_join_output(left_row, right_row, left, right, left_query->offset, right_query->offset);
        return true;
    } else return false;
}

bool double_query_join_compare(char* left_row, char* right_row, struct query_params* left_query, struct query_params* right_query, struct relation* left, struct relation* right) {
    double value_one = (double) *(left_row + left_query->offset);
    double value_two = (double) *(right_row + right_query->offset);
    if (value_one == value_two) {
        query_join_output(left_row, right_row, left, right, left_query->offset, right_query->offset);
        return true;
    } else return false;
}


bool varchar_query_join_compare(char* left_row, char* right_row, struct query_params* left_query, struct query_params* right_query, struct relation* left, struct relation* right) {
    char* value_one = (char*) (left_row + left_query->offset);
    char* value_two = (char*) (right_row + right_query->offset);
    if (strcmp(value_one, value_two) == 0) {
        query_join_output(left_row, right_row, left, right, left_query->offset, right_query->offset);
        return true;
    } else return false;
}

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
    //free(str1);
    *str = new_str;
}