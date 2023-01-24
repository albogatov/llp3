#include "../include/ast.h"
#include "../include/string_utils_xml.h"


static const std::string tab = " ";

void indent(int indentation, std::string src) {
    for (int i = 0; i < indentation; ++i) {
        src += tab;
    }
}

std::string ast_to_xml(ast_node* root) {
    std::string res = "<?xml version=\"1.0\"?>\n";
    return res + "<command>\n" + XmlToString(root, 0) + "\n</command>";
}

std::string XmlStringNode(ast_node* node, int indentation) {
    std::string res;
    return res + "\"string\">" + node->fields_one.string;
}

std::string XmlColumnNode(ast_node* node, int indentation) {
    std::string res;
    return res + "\"column_name\">" + node->fields_two.string;
}

std::string XmlNumberNode(ast_node* node, int indentation) {
    std::string res;
    return res + "\"number\">" + std::to_string(node->fields_one.number);
}


std::string XmlFloatNumberNode(ast_node* node, int indentation) {
    std::string res;
    return res + "\"float\">" + std::to_string(node->fields_one.float_number);
}


std::string XmlBooleanNode(ast_node* node, int indentation) {
    std::string res;
    return res + "\"bool\">" + std::to_string(node->fields_one.boolean);
}


std::string XmlTypeNode(ast_node* node, int indentation) {
    std::string res;
    return res + "\"type\">" + type_repr_[node->fields_one.data_type];
}


std::string XmlListNode(ast_node* node, int indentation) {
    std::string res;
    indent(indentation, res);
    res += "<list>\n";
    ast_node* next = node;
    while (next != NULL) {
        res = res + XmlToString(next->first, indentation + 1) + "\n";
        next = next->second;
    }
    indent(indentation, res);
    return res + "</list>";
}

std::string XmlPairNode(ast_node* node, int indentation) {
    std::string res;
    indent(indentation, res);
    res += "<pair>\n";
    indent(indentation + 1, res);
    res = res + "<value type=\"column_name\">" + node->fields_one.string + "</value>\n";
    indent(indentation + 1, res);
    res += "<value type=" + XmlToString(node->second, indentation + 1) + "</value>\n";
    indent(indentation, res);
    return res + "</pair>";
}


std::string XmlSelectNode(ast_node* node, int indentation) {

    std::string res;
    indent(indentation, res);
    res += "<select>\n";
    indent(indentation + 1, res);
    res = res + "<table>" + node->fields_one.string + "</table>\n";
    indent(indentation + 1, res);
    res += "<cmp>";

    if (node->first != nullptr) {
        res += "\n";
        res += XmlToString(node->first, indentation + 2);
    } else {
        res += "<nullable>nullptr</nullable>";
    }
    res += "</cmp>\n";
    indent(indentation + 1, res);
    res += "<join>";
    if (node->fields_two.string) {
        res += "<value type=\"table_name\">";
        res += node->fields_two.string ;
        res += "</value>";
    } else {
        res += "<nullable>nullptr</nullable>";
    }
    res += "</join>\n";
    indent(indentation, res);
    return res + "</select>";
}


std::string XmlDeleteNode(ast_node* node, int indentation) {

    std::string res;
    indent(indentation, res);
    res += "<delete>\n";
    indent(indentation + 1, res);
    res = res + "<table>" + node->fields_one.string + "</table>\n";
    indent(indentation + 1, res);
    res += "<cmp>";

    if (node->first != nullptr) {
        res += "\n";
        res += XmlToString(node->first, indentation + 2);
    } else {
        res += "nullptr";
    }
    res += "</cmp>\n";
    indent(indentation + 1, res);
    res += "\n";
    indent(indentation, res);
    return res + "</delete>";
}

std::string XmlInsertNode(ast_node* node, int indentation) {
    std::string res;
    indent(indentation, res);
    res += "<insert>\n";
    indent(indentation + 1, res);
    res = res + "<name>" + node->fields_one.string + "</name>\n" + XmlToString(node->first, indentation + 1) + "\n";
    indent(indentation, res);
    return res + "</insert>";
}


std::string XmlUpdateNode(ast_node* node, int indentation) {

    std::string res;
    indent(indentation, res);
    res += "<update>\n";
    indent(indentation + 1, res);
    res = res + "<table>" + node->fields_one.string + "</table>\n";
    indent(indentation + 1, res);
    res += "<cmp>";

    if (node->first != nullptr) {
        res += "\n";
        res += XmlToString(node->first, indentation + 2);
    } else {
        res += "nullptr";
    }
    res += "</cmp>\n";
    indent(indentation + 1, res);
    res += "<list_values>\n" + XmlToString(node->second, indentation + 2) + "\n</list_values>\n";
    indent(indentation, res);
    return res + "</update>";
}

std::string XmlCreateNode(ast_node* node, int indentation) {
    std::string res;
    indent(indentation, res);
    res += "<create>\n";
    indent(indentation + 1, res);
    res = res + "<name>" + node->fields_one.string + "</name>\n<list_values>" + XmlToString(node->first, indentation + 1) + "</list_values>\n";
    indent(indentation, res);
    return res + "</create>";
}

std::string XmlDropNode(ast_node* node, int indentation) {

    std::string res;
    indent(indentation, res);
    res = res + "<drop>" + node->fields_one.string + "</drop>";

    return res;
}

std::string XmlFilterNode(ast_node* node, int indentation) {
    std::string res;
    indent(indentation, res);
    res += "<filter><left_filter>\n" + XmlToString(node->first, indentation + 1) + "</left_filter>\n";
    indent(indentation + 1, res);
    res = res + "<operation>" + logic_repr_[node->fields_one.logic_op_type_type] + "</operation>\n<right_filter>" + XmlToString(node->second, indentation + 1) + "</right_filter>\n";
    indent(indentation, res);
    return res + "</filter>";
}


std::string XmlCmpNode(ast_node* node, int indentation) {
    std::string res;
    indent(indentation, res);
    res += "<filter><left_operand type=" + XmlToString(node->first, indentation + 1) + "</left_operand>\n";
    indent(indentation + 1, res);
    res = res + "<compare_by>" + cmp_op_repr_[node->fields_one.comp_op_type] + "</compare_by"">\n";
    indent(indentation, res);
    res += "<right_operand type=" + XmlToString(node->second, indentation + 1) + "</right_operand>\n";
    indent(indentation, res);
    return res + "</filter>";
}

std::string XmlToString(ast_node* node, int indentation) {
    switch(node->type) {
        case NAME_NODE:
            return XmlColumnNode(node, indentation);
            break;
        case STRING_NODE:
            return XmlStringNode(node, indentation);
            break;
        case NUMBER_NODE:
            return XmlNumberNode(node, indentation);
            break;
        case FLOAT_NUMBER_NODE:
            return XmlFloatNumberNode(node, indentation);
            break;
        case BOOLEAN_NODE:
            return XmlBooleanNode(node, indentation);
            break;
        case LIST_NODE:
            return XmlListNode(node, indentation);
            break;
        case PAIR_NODE:
            return XmlPairNode(node, indentation);
            break;
        case FOR_NODE:
            return XmlSelectNode(node, indentation);
            break;
        case INSERT_NODE:
            return XmlInsertNode(node, indentation);
            break;
        case REMOVE_NODE:
            return XmlDeleteNode(node, indentation);
            break;
        case UPDATE_NODE:
            return XmlUpdateNode(node, indentation);
            break;
        case CREATE_NODE:
            return XmlCreateNode(node, indentation);
            break;
        case DROP_NODE:
            return XmlDropNode(node, indentation);
            break;
        case FILTER_NODE:
            return XmlFilterNode(node, indentation);
            break;
        case COMPARE_NODE:
            return XmlCmpNode(node, indentation);
            break;
        case TYPE_NODE:
            return XmlTypeNode(node, indentation);
            break;
        default:
            break;
    }
}



