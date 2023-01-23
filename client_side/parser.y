
%{
    #include "include/ast.h"
    #include "include/output.h"

    #define YYERROR_VERBOSE 1

    extern int yylex();
    extern int yylineno;
    void yyerror(const char*);
%}

%define parse.error verbose

%token FOR REMOVE IN FILTER WITH RETURN INSERT UPDATE CREATE DROP UNEXPECTED_TOKEN

%token

    START_PARENTHESIS         "("
    CLOSE_PARENTHESIS         ")"
    START_BRACKET             "{"
    CLOSE_BRACKET             "}"
    SEMICOLON                 ";"
    COLON                     ":"
    COMMA                     ","
    DOT                       "."
    AND                       "&&"
    OR                        "||"
;
%token TYPE QUOTE

%token CMP BOOL INT FLOAT STR

%type <number_value> CMP
%type <number_value> TYPE
%type <boolean_value> BOOL
%type <number_value> INT
%type <float_num_value> FLOAT
%type <string> STR

%type <number_value> AND
%type <number_value> OR

%type <nonterminal> ast_root
%type <nonterminal> query

%type <nonterminal> select_query
%type <nonterminal> select_simple

%type <nonterminal> select_with_filter
%type <nonterminal> select_join_simple
%type <nonterminal> select_join_with_filter

%type <nonterminal> update_query
%type <nonterminal> update_simple
%type <nonterminal> update_with_filter

%type <nonterminal> delete_query
%type <nonterminal> delete_simple
%type <nonterminal> delete_with_filter

%type <nonterminal> create_query
%type <nonterminal> drop_query

%type <nonterminal> insert_query

%type <nonterminal> filter_condition
%type <nonterminal> logic_statement

%type <nonterminal> column
%type <nonterminal> row_alias

%type <nonterminal> terminal

%type <nonterminal> values_list
%type <nonterminal> values_pair

%type <nonterminal> string_list


%union {
    bool boolean_value;
    char string[100];
    float float_num_value;
    int cmp_op_type;
    int logic_op_type;
    int type;
    int number_value;
    ast_node* nonterminal;
}

%%

ast_root:
|   ast_root query SEMICOLON { output($2); printf("$> "); }
;

terminal:
|   TYPE { $$ = new_type($1); }
|   INT { $$ = new_number($1); }
|   FLOAT { $$ = new_float_number($1); }
|   BOOL { $$ = new_bool($1); }
|   QUOTE string_list QUOTE { $$ = new_string($2, NULL); }
;

query:
|   select_query
|   insert_query
|   delete_query
|   drop_query
|   update_query
|   create_query
;

select_query:
|   select_simple
|   select_with_filter
|   select_join_simple
|   select_join_with_filter
;
select_simple:
|   FOR row_alias IN STR RETURN STR { $$ = new_select($4, NULL, NULL, NULL, $2); }
;
select_with_filter:
|   FOR row_alias IN STR FILTER filter_condition RETURN STR { $$ = new_select($4, $6, NULL, NULL, $2); }
;

select_join_simple:
|   FOR row_alias IN STR FOR row_alias IN STR RETURN STR "," STR { $$ = new_select($4, NULL, $8, $6, $2); }
;

select_join_with_filter:
|   FOR row_alias IN STR FOR row_alias IN STR FILTER filter_condition RETURN STR "," STR { $$ = new_select($4, $10, $8, $6, $2); }
;

update_query:
|   update_simple
|   update_with_filter
;
update_simple:
|   FOR row_alias IN STR UPDATE STR WITH "{" values_list "}" IN STR { $$ = new_update($4, NULL, $9); }
;
update_with_filter:
|   FOR row_alias IN STR FILTER filter_condition UPDATE STR WITH "{" values_list "}" IN STR { $$ = new_update($4, $6, $11); }
;

delete_query:
|   delete_simple
|   delete_with_filter
;
delete_simple:
|   FOR row_alias IN STR REMOVE STR IN STR { $$ = new_delete($4, NULL, $2); }
;
delete_with_filter:
|   FOR row_alias IN STR FILTER filter_condition REMOVE STR IN STR { $$ = new_delete($4, $6, $2); }
;

insert_query:
|   INSERT "{" values_list "}" IN STR { $$ = new_insert($6, $3); }
;

create_query:
|   CREATE STR "{" values_list "}" { $$ = new_create($2, $4); }
;

drop_query:
|   DROP STR { $$ = new_drop($2); }
;

values_list:
|   values_list "," values_pair { $$ = new_list($3, $1); }
|   values_pair { $$ = new_list($1, NULL); }
;

values_pair:
|   STR ":" terminal { $$ = new_pair($1, $3); }
;

filter_condition:
|   filter_condition "&&" filter_condition { $$ = new_where($2, $1, $3); }
|   filter_condition "||" filter_condition { $$ = new_where($2, $1, $3); }
|   "(" filter_condition ")" { $$ = $2; }
|   logic_statement
;

logic_statement:
|   column CMP terminal { $$ = new_compare($2, $1, $3); }
|   terminal CMP column { $$ = new_compare(switch_cmp_mode($2), $1, $3); }
|   column CMP column { $$ = new_compare($2, $1, $3); }
;

column:
|   STR DOT STR { $$ = new_name($1, $3); }
;

row_alias:
|   STR { $$ = new_name(NULL, $1); }
;

string_list:
|   string_list STR { $$ = new_string($1, $2); }
|   STR { $$ = new_string(NULL, $1); }
;


%left OR;
%left AND;

%%

void yyerror (const char *s) {
   fprintf (stderr, "%s on line number %d", s, yylineno);
}

int main() {
	printf("$> ");
	yyparse();
	return 0;
}



