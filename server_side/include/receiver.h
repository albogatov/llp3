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

        std::string clean(const std::string &input) {
            std::string res;
            res.reserve(input.size());

            for (const auto &chr: input) {
                if (!std::isspace(chr)) {
                    res.push_back(chr);
                }
            }

            return res;
        }

        content_type type_from_str(std::string &str) {
            if (str == "BOOl" || str == "bool") {
                return BOOLEAN;
            } else if (str == "FLOAT" || str == "float_number") {
                return DOUBLE;
            } else if (str == "STR" || str == "string") {
                return VARCHAR;
            } else if (str == "INT" || str == "number") {
                return INTEGER;
            }
        }

//        compare_by compare_from_str(std::string &str) {
//            if (str == "GREATER") {
//                return GREATER;
//            } else if (str == "LESS") {
//                return LESS;
//            } else if (str == "GREATER_OR_EQUAL") {
//                return GREATER_OR_EQUAL;
//            } else if (str == "LESS_OR_EQUAL") {
//                return LESS_OR_EQUAL;
//            } else if (str == "NOT_EQUAL") {
//                return NOT_EQUAL;
//            } else if (str == "EQUAL") {
//                return EQUAL;
//            } else {
//                return NO_COMPARE;
//            }
//        }

    }


    namespace {
//
//        std::string base_select_or_delete_evaluate(const boost::property_tree::ptree &pt, database* db,
//                                                   bool select) {
//
//            auto name = pt.get<std::string>("table");
//            auto list = pt.get_child("cmp"); // error if filter
//
////            if (list.get<std::string>("nullable", "") == "nullptr") {
////                std::cout << name << std::endl;
////                if (select) {
////                    std::string res;
////                    relation* relation = relation_get(name.c_str(), db);
////                    row* row_one = row_create(relation);
////                    query* select_query = query_make(SELECT, relation, column, value, -1);
////                    auto selected = db.select({name, "", NO_COMPARE, "", STR});
////                    for (const auto &item: selected.rows) {
////                        res += item + "\n";
////                    }
////                    return !res.empty() ? res : "empty";
////                } else {
////                    return "deleted";
////                }
////            }
//
//            list = list.get_child("compare");
//
//            auto column = list.get<std::string>("left_operand"); // .get<std::string>("column_name");
//            int column_len = column.length();
//            char* column_ar = new char[column_len + 1];
//            strcpy(column_ar, column.c_str());
//
//            auto cmp = list.get<std::string>("compare_by");
//            auto value = list.get<std::string>("right_operand");
//            void* value_v = &value;
//
////            type as_type = STR;
////            auto this_type = list.get<std::string>("right_operand.<xmlattr>.type");
////            if (this_type == "number") {
////                as_type = INT;
////            } else if (this_type == "string") {
////                as_type = STR;
////            } else if (this_type == "bool") {
////                as_type = BUL;
////            } else if (this_type == "float") {
////                as_type = FLT;
////            }
//
//            if (select && pt.get_child("join").get<std::string>("nullable", "") != "nullptr") {
//                auto second_name = pt.get<std::string>("join.select.table");
//                std::string res;
//                std::cout << name << " " << second_name << " " << column << " " << value << std::endl;
//                relation* relation = relation_get(name.c_str(), db);
//                row* row_one = row_create(relation);
//                query* select_query = query_make(SELECT, relation, static_cast<char**>(&column_ar), &value_v, -1);
//                query_execute(select_query, true);
//                //auto selected = db.join_select({name, second_name, column_ar, value });
////                for (const auto &item: selected.rows) {
////                    res += item + "\n";
////                }
//                return res;
//            }
//
//            if (select) {
//                std::string res;
//                relation* relation = relation_get(name.c_str(), db);
//                row* row_one = row_create(relation);
//                query* select_query = query_make(SELECT, relation, static_cast<char**>(&column_ar), &value_v, -1);
//                query_execute(select_query, true);
//                //auto selected = db.select({name, column, compare_from_str(cmp), value, as_type });
////                for (const auto &item: selected.rows) {
////                    res += item + "\n";
////                }
//                return !res.empty() ? res : "empty";
//            } else {
////                if (db._delete({name, column, compare_from_str(cmp), value, as_type })) {
////                    return "deleted";
////                } else {
////                    return "error in delete: wrong query";
////                }
//            }
//        }
//
        std::string insert_evaluate(const boost::property_tree::ptree &pt, database* db) {

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
//
//        std::string select_evaluate(const boost::property_tree::ptree &pt, database* db) {
//            return base_select_or_delete_evaluate(pt, db, true);
//        }
//
//        std::string update_evaluate(const boost::property_tree::ptree &pt, database* db) {
//
//        }
//
//        std::string delete_evaluate(const boost::property_tree::ptree &pt, database* db) {
//            return base_select_or_delete_evaluate(pt, db, false);
//        }
//
        std::string create_evaluate(const boost::property_tree::ptree &pt, database* db) {
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
//        std::string drop_evaluate(const boost::property_tree::ptree &pt, database* db) {
//
//            return "table dropped";
//        }
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

    std::string evaluate(std::string& input, database* db) {

        // std::cout << input << std::endl;

        std::istringstream iss (input);

//        xml_schema::properties properties;
//        properties.no_namespace_schema_location("../../server/scheme.xsd");
//
//        try {
//            std::unique_ptr<command_t> ptr(command(iss, 0, properties));
//            auto maybe_select = ptr->select();
//        } catch (xml_schema::exception& e) {
//            return e.what();
//        }

        boost::property_tree::ptree property_tree;
        std::stringstream stream(input);

        boost::property_tree::read_xml(stream, property_tree);

//        print_tree(property_tree, 0);

        property_tree = property_tree.get_child("command");
        BOOST_FOREACH(auto &v, property_tree) {

            auto command = v.first;
            auto children = property_tree.get_child(command);

            if (command == "create") {
                std::cout << "create query" << std::endl;
                return create_evaluate(children, db);
            } else if (command == "select") {
                std::cout << "select query" << std::endl;
                //return select_evaluate(children, db);
            } else if (command == "insert") {
                std::cout << "insert query" << std::endl;
                return insert_evaluate(children, db);
            } else if (command == "update") {
                std::cout << "update query" << std::endl;
                //return update_evaluate(children, db);
            } else if (command == "delete") {
                std::cout << "delete query" << std::endl;
                //return delete_evaluate(children, db);
            } else {
                return "unsupported operation";
            }
        }

    }

};





#endif //LLP3_XML_RECEIVER_H
