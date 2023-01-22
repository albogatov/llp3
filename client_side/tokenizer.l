%option noyywrap
%option caseless
%option yylineno

%{
    #include <stdio.h>
    #include <string.h>

    #include "include/ast.h"
    #include "parser.tab.h"
    void print_error(char*);
%}

%option noyywrap nounput noinput debug

alpha   [A-Za-z_0-9 \t\r]
id    [a-zA-Z][a-zA-Z_0-9]*
int   ([-+]?[0-9])+
float   [+-]?([0-9]*[.])?[0-9]+
blank [ \t\r]
word ([a-zA-Z_][a-zA-Z0-9_]*)
quoted_string \"{word}*\"

%%

"for"       {return FOR;}
"insert"    {return INSERT;}
"update"    {return UPDATE;}
"remove"    {return REMOVE;}
"in"        {return IN;}
"filter"    {return FILTER;}
"with"      {return WITH;}
"return"    {return RETURN;}

"create"    {return CREATE;}
"drop"      {return DROP;}

"&&"        {yylval.logic_op_type = 1; return AND;}
"||"        {yylval.logic_op_type = 2; return OR;}

">"         {yylval.cmp_op_type = 1; return CMP;}
">="        {yylval.cmp_op_type = 2; return CMP;}
"<"         {yylval.cmp_op_type = 3; return CMP;}
"<="        {yylval.cmp_op_type = 4; return CMP;}
"=="        {yylval.cmp_op_type = 5; return CMP;}
"!="        {yylval.cmp_op_type = 6; return CMP;}

"float"     {yylval.type = 0; return TYPE;}
"string"    {yylval.type = 1; return TYPE;}
"int"       {yylval.type = 2; return TYPE;}
"bool"      {yylval.type = 3; return TYPE;}
"true"      {yylval.boolean_value = 1; return BOOL;}
"false"     {yylval.boolean_value = 0; return BOOL;}

"("         {return START_PARENTHESIS;}
")"         {return CLOSE_PARENTHESIS;}
"{"         {return START_BRACKET;}
"}"         {return CLOSE_BRACKET;}
";"         {return SEMICOLON;}
":"         {return COLON;}
"."         {return DOT;}
","         {return COMMA;}
"\""        {return QUOTE;}

{word}     {
    sscanf(yytext, "%s", yylval.string);
    return (STR);
}
{int}   {
    yylval.number_value = atoi(yytext);
    return (INT);
}
{float}     {
    yylval.float_num_value = atof(yytext);
    return (FLOAT);
}

[ \t]   {   }
[\n]    {   }
.           {
    print_error(yytext);
    return (UNEXPECTED_TOKEN);
}

%%

void print_error(char* token) {
    printf("Error in tokenizer token = %s \n", token);
}


