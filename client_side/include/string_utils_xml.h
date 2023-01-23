
#ifndef LLP3_XML_STRING_UTILS_XML_H
#define LLP3_XML_STRING_UTILS_XML_H
#include <string>

std::string XmlToString(ast_node* node, int indentation);
std::string ast_to_xml(ast_node* root);

#endif //LLP3_XML_STRING_UTILS_XML_H
