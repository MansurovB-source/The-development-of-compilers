#ifndef ABSTRACT_TREE_UTIL_H
#define ABSTRACT_TREE_UTIL_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    Root,
    Variables_Declaration,
    Variables,
    Identifier,
    Program_Body,
    Expression,
    Operators,
    Operator,
    Const,
    If,
    If_Else,
    Condition_Expression,
    Loop,
    Undefined
} Node_Type;

typedef struct Abstract_Tree {
    Node_Type type;
    char *value;
    struct Abstract_Tree *left;
    struct Abstract_Tree *right;

} Abstract_Tree;

Abstract_Tree *add_Node(Node_Type type, char *value, Abstract_Tree *left, Abstract_Tree *right);

Abstract_Tree *add_Variable(char *value, Abstract_Tree *variables);

Abstract_Tree *add_Assignment(char *name, Abstract_Tree *expression);

Abstract_Tree *
add_Condition(Node_Type type, Abstract_Tree *condition, Abstract_Tree *condition_block, Abstract_Tree *else_block);

Abstract_Tree *get_variable(char *name);

Abstract_Tree *get_constant(char *value);

#endif //ABSTRACT_TREE_UTIL_H
