#ifndef LLP3_XML_RECEIVER_H
#define LLP3_XML_RECEIVER_H

#pragma once

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#include "relation.h"

enum type : int {
    INT,
    STR,
    BUL,
    FLT
};

namespace receiver {

    namespace {

//        std::string clean(const std::string &input) {
//            std::string res;
//            res.reserve(input.size());
//
//            for (const auto &chr: input) {
//                if (!std::isspace(chr)) {
//                    res.push_back(chr);
//                }
//            }
//
//            return res;
//        }

        content_type type_from_str(std::string &str) {
            if (str == "BOOLEAN" || str == "bool") {
                return BOOLEAN;
            } else if (str == "FLOAT_NUMBER" || str == "float_number") {
                return DOUBLE;
            } else if (str == "STRING" || str == "string") {
                return VARCHAR;
            } else if (str == "NUMBER" || str == "number") {
                return INTEGER;
            }
        }

    }


    namespace {

        std::string select_op(const boost::property_tree::ptree &pt, database* db) {

            auto name = pt.get<std::string>("table");
            auto list = pt.get_child("cmp");


            list = list.get_child("filter");

            auto column = list.get<std::string>("left_operand");
            int column_len = column.length();
            char* column_ar = new char[column_len + 1];
            strcpy(column_ar, column.c_str());

            auto cmp = list.get<std::string>("compare_by");
            auto value = list.get<std::string>("right_operand");
            void* value_v = &value;
            std::string value_c = value;
            int help_int1;
            std::string help_string;
            bool help_bool1;
            double help_double1;

            auto as_type = list.get<std::string>("right_operand.<xmlattr>.type");
            if (as_type == "number") {
                as_type = INTEGER;
                help_int1 = list.get<int>("right_operand");
                value_v = &help_int1;
            } else if (as_type == "string") {
                as_type = VARCHAR;
                value_v = &value;
            } else if (as_type == "bool") {
                as_type = BOOLEAN;
                help_bool1 = list.get<bool>("right_operand");
                value_v = &help_bool1;
            } else if (as_type == "float") {
                as_type = DOUBLE;
                help_double1 = list.get<double>("right_operand");
                value_v = &help_double1;
            }

            if (select && pt.get_child("join").get<std::string>("nullable", "") != "nullptr") {

                auto second_name = pt.get<std::string>("join");
                std::string res;
                relation* relation = relation_get(name.c_str(), db);
                struct relation* relation_join = relation_get(second_name.c_str(), db);
                struct query_join* select_query_7 = query_join_make(relation, relation_join, column_ar,
                                                                    value_c.data());
                char* res1 = res.data();
                res1 = query_join_execute(select_query_7, res1);
                if (res1) {
                    std::string res(res1);
                    return !res.empty() ? res : "empty";
                } else return "Something went very wrong";
            }

                char* res1 = "";
                relation* relation = relation_get(name.c_str(), db);
                query* select_query = query_make(SELECT, relation, static_cast<char**>(&column_ar), &value_v, -1);
                res1 = query_execute(select_query, true, res1);
                if (res1) {
                    std::string res(res1);
                    return !res.empty() ? res : "empty";
                } else return "Something went very wrong";
            
        }

        std::string insert_op(const boost::property_tree::ptree &pt, database* db) {

            auto name = pt.get<std::string>("name");
            auto list = pt.get_child("list");

            struct relation* relation = relation_get(name.c_str(), db);
            row* row_one = row_create(relation);
            std::vector<std::pair<std::string, content_type>> values;
            BOOST_FOREACH(const auto &v, list) {
                            std::string str;
                            std::string tp;
                            int help_int;
                            std::string help_string;
                            bool help_bool;
                            double help_double;
                            void* val;

                            BOOST_FOREACH(const auto &u, v.second) {
                                            auto this_tp = u.second.get<std::string>("<xmlattr>.type");
                                            if (this_tp == "column_name") {
                                                str = u.second.get<std::string>("");
                                            } else {
                                                tp = u.second.get<std::string>("<xmlattr>.type");
                                                switch (type_from_str(tp)) {
                                                    case INTEGER:
                                                        help_int = u.second.get<int>("");
                                                        val = &help_int;
                                                        break;
                                                    case VARCHAR:
                                                        help_string = u.second.get<std::string>("");
                                                        val = &help_string;
                                                        break;
                                                    case DOUBLE:
                                                        help_double = u.second.get<double>("");
                                                        val = &help_double;
                                                        break;
                                                    case BOOLEAN:
                                                        help_bool = u.second.get<bool>("");
                                                        val = &help_bool;
                                                        break;
                                                }

                                            }
                                        }

                                        attribute_add(row_one, (char *)str.c_str(), type_from_str(tp),
                                                      val);

                        }
            std::reverse(values.begin(), values.end());
            row_insert(row_one);
            row_close(row_one);
            return "inserted into " + name;
        }

        std::string update_op(const boost::property_tree::ptree &pt, database* db) {
            auto name = pt.get<std::string>("table");
            auto list = pt.get_child("cmp");
            list = list.get_child("filter");

            auto column = list.get<std::string>("left_operand");
            int column_len = column.length();
            char* column_ar = new char[column_len + 1];
            strcpy(column_ar, column.c_str());
            std::string init_buf;

            auto cmp = list.get<std::string>("compare_by");
            auto value = list.get<std::string>("right_operand");
            void* value_v = &value;
            std::string value_c = value;
            int help_int1;
            std::string help_string;
            bool help_bool1;
            double help_double1;
            void* query_vals[2];
            content_type as_type = VARCHAR;
            auto this_type = list.get<std::string>("right_operand.<xmlattr>.type");
            if (this_type == "number") {
                as_type = INTEGER;
                help_int1 = list.get<int>("right_operand");
                value_v = &help_int1;
            } else if (this_type == "string") {
                as_type = VARCHAR;
                value_v = &value;
            } else if (this_type == "bool") {
                as_type = BOOLEAN;
                help_bool1 = list.get<bool>("right_operand");
                value_v = &help_bool1;
            } else if (this_type == "float") {
                as_type = DOUBLE;
                help_double1 = list.get<double>("right_operand");
                value_v = &help_double1;
            }


            query_vals[0] = value_v;

            auto list_vals = pt.get_child("list_values").get_child("list");

            std::vector<std::pair<std::string, void*>> values;
            BOOST_FOREACH(const auto &v, list_vals) {
                            std::string str;
                            std::string tp;
                            void* val;
                            int help_int;
                            bool help_bool;
                            double help_double;

                            BOOST_FOREACH(const auto &u, v.second) {
                                            auto this_tp = u.second.get<std::string>("<xmlattr>.type");
                                            if (this_tp == "column_name") {
                                                str = u.second.get<std::string>("");
                                            } else {
                                                tp = u.second.get<std::string>("");
                                                tp = u.second.get<std::string>("<xmlattr>.type");
                                                switch (type_from_str(tp)) {
                                                    case INTEGER:
                                                        help_int = u.second.get<int>("");
                                                        val = &help_int;
                                                        break;
                                                    case VARCHAR:
                                                        help_string = u.second.get<std::string>("");
                                                        val = &help_string;
                                                        break;
                                                    case DOUBLE:
                                                        help_double = u.second.get<double>("");
                                                        val = &help_double;
                                                        break;
                                                    case BOOLEAN:
                                                        help_bool = u.second.get<bool>("");
                                                        val = &help_bool;
                                                        break;
                                            }
                                        }

                            values.emplace_back(str, val);
                        }
            std::reverse(values.begin(), values.end());

            query_vals[1] = values[1].second;
            void* cmp_vals[2] = {value_v, values[1].second};
            char* columns_vals[2] = { column.data() ,  values[0].first.data() };

            char* res1 = init_buf.data();
            relation* relation = relation_get(name.c_str(), db);
            query* select_query = query_make(UPDATE, relation, columns_vals, cmp_vals, -1);
            res1 = query_execute(select_query, true, res1);
            if (res1) {
                std::string res(res1);
                return !res.empty() ? res : "empty";
            } else return "Something went very wrong";
        }
        }

        std::string delete_op(const boost::property_tree::ptree &pt, database* db) {
            auto name = pt.get<std::string>("table");
            auto list = pt.get_child("cmp");


            list = list.get_child("filter");

            auto column = list.get<std::string>("left_operand");
            int column_len = column.length();
            char* column_ar = new char[column_len + 1];
            strcpy(column_ar, column.c_str());

            auto cmp = list.get<std::string>("compare_by");
            auto value = list.get<std::string>("right_operand");
            void* value_v = &value;
            std::string value_c = value;
            std::string init_data;

            content_type as_type = VARCHAR;
            auto this_type = list.get<std::string>("right_operand.<xmlattr>.type");
            if (this_type == "number") {
                as_type = INTEGER;
            } else if (this_type == "string") {
                as_type = VARCHAR;
            } else if (this_type == "bool") {
                as_type = BOOLEAN;
            } else if (this_type == "float") {
                as_type = DOUBLE;
            }

            char* res1 = init_data.data();
            relation* relation = relation_get(name.c_str(), db);
            query* select_query = query_make(DELETE, relation, static_cast<char**>(&column_ar), &value_v, -1);
            res1 = query_execute(select_query, true, res1);
            if (res1) {
                std::string res(res1);
                return !res.empty() ? res : "empty";
            } else return "Something went very wrong";

        }

        std::string create_op(const boost::property_tree::ptree &pt, database* db) {
            auto name = pt.get<std::string>("name");
            auto list = pt.get_child("list_values").get_child("list");


            struct schema* schema_one = schema_create();

            std::vector<std::pair<std::string, content_type>> values;
            BOOST_FOREACH(const auto &v, list) {
                std::string str;
                std::string tp;

                BOOST_FOREACH(const auto &u, v.second) {
                    auto this_tp = u.second.get<std::string>("<xmlattr>.type");
                    if (this_tp == "column_name") {
                        str = u.second.get<std::string>("");
                    } else {
                        tp = u.second.get<std::string>("");
                    }
                }

                values.emplace_back(str, type_from_str(tp));
            }
            std::reverse(values.begin(), values.end());

            for(std::pair<std::string, content_type> i : values) {
                if (i.second != VARCHAR)
                    schema_one = schema_add_column(schema_one, i.first.c_str(), i.second);
                else schema_one = schema_add_column_varchar(schema_one, i.first.c_str(), i.second, 40);
            }

            relation* relation_one = relation_create(schema_one, name.c_str(), db);

            return "created " + name;
        }
//
//        void print_tree(const boost::property_tree::ptree &pt, int level) {
//            const std::string sep(2 * level, ' ');
//            BOOST_FOREACH(const auto &v, pt) {
//                std::cout
//                        << sep
//                        << v.first << " : " << v.second.data() << "\n";
//                print_tree(v.second, level + 1);
//            }
        }

//    }

    std::string request_op(std::string& input, database* db) {

        std::istringstream iss (input);

        boost::property_tree::ptree property_tree;
        std::stringstream stream(input);

        boost::property_tree::read_xml(stream, property_tree);

        property_tree = property_tree.get_child("command");
        BOOST_FOREACH(auto &v, property_tree) {

            auto command = v.first;
            auto children = property_tree.get_child(command);

            if (command == "create") {
                std::cout << "create query" << std::endl;
                return create_op(children, db);
            } else if (command == "select") {
                std::cout << "select query" << std::endl;
                return select_op(children, db);
            } else if (command == "insert") {
                std::cout << "insert query" << std::endl;
                return insert_op(children, db);
            } else if (command == "update") {
                std::cout << "update query" << std::endl;
                return update_op(children, db);
            } else if (command == "delete") {
                std::cout << "delete query" << std::endl;
                return delete_op(children, db);
            } else {
                return "unsupported operation";
            }
        }

    }

};





#endif //LLP3_XML_RECEIVER_H
