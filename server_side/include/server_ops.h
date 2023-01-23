//
// Created by Alexandr Bogatov on 23.01.2023.
//

#ifndef LLP3_XML_SERVER_OPS_H
#define LLP3_XML_SERVER_OPS_H
#include <string>
#include <vector>
enum compare_by {
    GREATER, GREATER_OR_EQUAL, LESS, LESS_OR_EQUAL, EQUAL, NOT_EQUAL, NO_COMPARE
};

enum type {
    INT,
    STR,
    BUL,
    FLT
};

enum class command_type {
    SELECT,
    DELETE
};

struct command {
    std::string table_name;
    explicit command() = default;
    command(std::string table_name)
            : table_name(std::move(table_name)) {}
};

struct insert_command : virtual command {
    std::vector<std::string> columns;
    insert_command(const std::string& table_name, std::vector<std::string>  columns)
            : command(table_name)
            , columns(std::move(columns))
    {}
};

struct select_delete_command : virtual command {

    std::string column;
    compare_by cmp = NO_COMPARE;
    std::string param;
    type as_type = STR;

    select_delete_command() = default;
    select_delete_command(const std::string& table_name, std::string  column, compare_by cmp, std::string  param, type as_type)
            : command(table_name)
            , column(std::move(column))
            , cmp(cmp)
            , param(std::move(param))
            , as_type(as_type)
    {}

    select_delete_command(const std::string& table_name, std::string  column, std::string  param)
            : command(table_name)
            , column(std::move(column))
            , param(std::move(param))
    {}

};

struct select_command : virtual select_delete_command {

    select_command(select_command&& sc) = default;

    select_command(const std::string& table_name, const std::string& column, compare_by cmp, const std::string& param, type as_type)
            : command(table_name)
            , select_delete_command(table_name, column, cmp, param, as_type) {}

    select_command(const std::string& table_name, const std::string& column, const std::string& param)
            : command(table_name)
            , select_delete_command(table_name, column, param) {}

};

// тут параметр для первой команды -- это условие, а для второй --
struct join_select_command : command {

    std::string second_table_name;
    std::string first_column;
    std::string second_column;

};

struct delete_command : virtual select_delete_command {
    delete_command(const std::string& table_name, const std::string& column, compare_by cmp, const std::string& param, type as_type)
            : command(table_name)
            , select_delete_command(table_name, column, cmp, param, as_type) {}
};

struct update_command : insert_command, delete_command {

    update_command(const std::string& table_name, const std::string& column,
                   compare_by cmp, const std::string& param, type as_type,
                   const std::vector<std::string>& columns)

            : delete_command(table_name, column, cmp, param, as_type)
            , insert_command(table_name, columns)
    {}
};

struct create_command : command {
    std::vector< std::pair<std::string, type> > columns;

    create_command(const std::string& table_name, std::vector< std::pair<std::string, type> >&&  columns)
            : command(table_name)
            , columns(std::move(columns))
    {}

};

struct drop_command : command {};

struct returned_data {
    std::vector<std::string> rows;
    bool is_last;
};

struct check_and_return_result {
    bool is_ok;
    std::string full_str;
    std::string this_str;
};

#endif //LLP3_XML_SERVER_OPS_H
