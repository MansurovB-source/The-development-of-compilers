%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/util.h"
extern int yylex();
extern char* yyval;
extern int line_cnt;
void yyerror(char* str);

%}


%token VAR BBEGIN EEND
%token SEMICOLON COMMA LPARENTHENSIS RPARENTHENSIS
%token IF ELSE
%token REPEAT UNTIL
%token MINUS
%token NOT
%token PLUS MUL DIV LESS MORE EQUALS
%token ASSIGN
%token ID CONST


%start program

%right ASSIGN
%left PLUS MINUS
%left MUL DIV

%union {
    struct Abstract_Tree *tree;
    char *str;
}

%type<tree> program program_body variables_declaration variables operators operator expression sub_expression
%type<str> ID CONST
%%
program:
    variables_declaration program_body { $$ = add_Node(Root, NULL, $1, $2); }
    ;

program_body:
    BBEGIN operators EEND { $$ = add_Node(Program_Body, NULL, $2, NULL); }
    ;

variables_declaration:
    VAR variables SEMICOLON { $$ = add_Node(Variables_Declaration, NULL, NULL, $2); }
    ;

variables:
    ID { $$ = add_Variable($1, NULL); }
    | ID COMMA variables { $$ = add_Variable($1, $3); }
    ;

operators:
    operator { $$ = $1; }
    | operator operators { $$ = add_Node(Operators, NULL, $1, $2); }
    ;

operator:
    ID ASSIGN expression SEMICOLON { $$ = add_Assignment($1, $3); }
    | BBEGIN operators EEND { $$ = add_Node(Operators, NULL, $2, NULL); }
    | IF LPARENTHENSIS expression RPARENTHENSIS operator { $$ = add_Condition(If, $3, $5, NULL); }
    | IF LPARENTHENSIS expression RPARENTHENSIS operator ELSE operator { $$ = add_Condition(If_Else, $3, $5, $7); }
    | REPEAT operators UNTIL expression { $$ = add_Node(Loop, NULL, $2, $4); }
    ;

expression:
    MINUS sub_expression { $$ = add_Node(Expression, "MINUS", NULL, $2); }
    | NOT sub_expression { $$ = add_Node(Expression, "NOT", NULL, $2); }
    | sub_expression { $$ = $1; }
    ;

sub_expression:
    LPARENTHENSIS expression RPARENTHENSIS { $$ = $2; }
    | ID { $$ = get_variable($1); }
    | CONST { $$ = get_constant($1); }
    | sub_expression MINUS sub_expression { $$ = add_Node(Expression, "-", $1, $3); }
    | sub_expression PLUS sub_expression { $$ = add_Node(Expression, "+", $1, $3); }
    | sub_expression MUL sub_expression { $$ = add_Node(Expression, "*", $1, $3); }
    | sub_expression DIV sub_expression { $$ = add_Node(Expression, "/", $1, $3); }
    | sub_expression MORE sub_expression { $$ = add_Node(Expression, ">", $1, $3); }
    | sub_expression LESS sub_expression { $$ = add_Node(Expression, "<", $1, $3); }
    | sub_expression EQUALS sub_expression { $$ = add_Node(Expression, "==", $1, $3); }
    ;

%%

void yyerror(char* str) {
    fprintf(stderr, "Syntax error at line %d\n", line_cnt);
    exit(1);
}

int main(int argc, char **argv) {
    return yyparse();
}

