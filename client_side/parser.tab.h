/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    FOR = 258,                     /* FOR  */
    REMOVE = 259,                  /* REMOVE  */
    IN = 260,                      /* IN  */
    FILTER = 261,                  /* FILTER  */
    WITH = 262,                    /* WITH  */
    RETURN = 263,                  /* RETURN  */
    INSERT = 264,                  /* INSERT  */
    UPDATE = 265,                  /* UPDATE  */
    CREATE = 266,                  /* CREATE  */
    DROP = 267,                    /* DROP  */
    UNEXPECTED_TOKEN = 268,        /* UNEXPECTED_TOKEN  */
    START_PARENTHESIS = 269,       /* "("  */
    CLOSE_PARENTHESIS = 270,       /* ")"  */
    START_BRACKET = 271,           /* "{"  */
    CLOSE_BRACKET = 272,           /* "}"  */
    SEMICOLON = 273,               /* ";"  */
    COLON = 274,                   /* ":"  */
    COMMA = 275,                   /* ","  */
    DOT = 276,                     /* "."  */
    AND = 277,                     /* "&&"  */
    OR = 278,                      /* "||"  */
    TYPE = 279,                    /* TYPE  */
    QUOTE = 280,                   /* QUOTE  */
    CMP = 281,                     /* CMP  */
    BOOL = 282,                    /* BOOL  */
    INT = 283,                     /* INT  */
    FLOAT = 284,                   /* FLOAT  */
    STR = 285                      /* STR  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 81 "parser.y"

    bool boolean_value;
    char string[100];
    float float_num_value;
    int cmp_op_type;
    int logic_op_type;
    int type;
    int number_value;
    ast_node* nonterminal;

#line 105 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


#ifdef __cplusplus
extern "C" ast_node* yyparse ();
#else
extern ast_node* yyparse();
#endif


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
