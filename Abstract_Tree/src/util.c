#include "util.h"

#define STEP 5

static void add_variable_to_list(char *name);

static char *get_node_type(Abstract_Tree *node);

static void print_assembly(Abstract_Tree *root);

static void print_assembly_operators(Abstract_Tree *node);

static void print_assembly_assign(Abstract_Tree *node);

static void print_assembly_expression(Abstract_Tree *node, char *name);

static void print_assembly_if(Abstract_Tree *node);

static void print_assembly_loop(Abstract_Tree *node);

static void print_tree(Abstract_Tree *node, int offset);

static char *tmp();

static char *label();

static char **variable_list;
static uint64_t counter = 0;

Abstract_Tree *add_Node(Node_Type type, char *value, Abstract_Tree *left, Abstract_Tree *right) {
    Abstract_Tree *node = malloc(sizeof(Abstract_Tree));
    node->type = type;
    if (value == NULL) {
        node->value = NULL;
    } else {
        node->value = strdup(value);
    }
    node->left = left;
    node->right = right;

    if (type == Root) {
        print_tree(node, 0);
        print_assembly(node);
    }
    return node;
}

Abstract_Tree *add_Variable(char *value, Abstract_Tree *variables) {
    Abstract_Tree *node = malloc(sizeof(Abstract_Tree));
    node->type = Variables;
    if (value == NULL) {
        node->value = NULL;
    } else {
        node->value = strdup(value);
    }
    node->right = variables;

    add_variable_to_list(node->value);

    return node;
}

Abstract_Tree *add_Assignment(char *name, Abstract_Tree *expression) {
    Abstract_Tree *node = malloc(sizeof(Abstract_Tree));
    node->type = Operator;
    node->value = "=";
    node->left = get_variable(name);
    node->right = expression;
    return node;
}

Abstract_Tree *
add_Condition(Node_Type type, Abstract_Tree *condition, Abstract_Tree *condition_block, Abstract_Tree *else_block) {
    Abstract_Tree *node = malloc(sizeof(Abstract_Tree));
    node->type = type;
    node->value = NULL;
    node->left = condition;

    if (else_block) {
        node->right = add_Node(Condition_Expression, NULL, condition_block, else_block);
    } else {
        node->right = add_Node(Condition_Expression, NULL, condition_block, NULL);
    }
    return node;
}

static void add_variable_to_list(char *name) {
    static uint64_t current_size = 0;
    if (current_size == 0) {
        variable_list = (malloc(sizeof(char *) * 16));
        current_size += 16;
    }

    if (current_size == counter - 1) {
        variable_list = (realloc(variable_list, sizeof(uint64_t) * (current_size + 16)));
        current_size += 16;
    }

    variable_list[counter] = name;
    counter++;

}

//get variable by name
Abstract_Tree *get_variable(char *name) {
    for (uint64_t i = 0; i < counter; i++) {
        if (strcmp(variable_list[i], name) == 0) {
            Abstract_Tree *node = malloc(sizeof(Abstract_Tree));
            node->type = Identifier;
            node->value = name;
            node->left = NULL;
            node->right = NULL;
            return node;

        }
    }
    fprintf(stderr, "Variable %s was nod declared!\n", name);
    exit(EXIT_FAILURE);
}

Abstract_Tree *get_constant(char *value) {
    Abstract_Tree *node = malloc(sizeof(Abstract_Tree));
    node->type = Identifier;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void print_tree(Abstract_Tree *node, int offset) {
    if (node == NULL) {
        return;
    }

    offset += STEP;
    print_tree(node->right, offset);
    for (int i = STEP; i < offset; i++) {
        if (offset > STEP && i >= offset - STEP) {
            if (i == offset - STEP) {
                printf("+");
            } else {
                printf("-");
            }
        } else {
            printf(" ");
        }
    }

    char *node_type = get_node_type(node);

    if (node_type) {
        printf("[%s]", node_type);
    } else {
        printf("Happened something wrong");
    }
    if (node->value) {
        printf("(%s)\n", node->value);
    } else {
        printf("(NULL)\n");
    }

    print_tree(node->left, offset);
}


void print_assembly(Abstract_Tree *root) {
    if (!root) {
        return;
    }

    if (!root->right) {
        printf("Empty body\n");
        return;
    }

    print_assembly_operators(root->right->left);
}

void print_assembly_operators(Abstract_Tree *node) {
    if (!node) {
        return;
    }

    switch (node->type) {
        case Operators:
            print_assembly_operators(node->left);
            print_assembly_operators(node->right);
            break;

        case Operator:
            print_assembly_assign(node);
            break;

        case If:
        case If_Else:
            print_assembly_if(node);
            break;
        case Loop:
            print_assembly_loop(node);
            break;
        default:
            printf("OPERATORS ERROR\n");
            break;
    }
}

static void print_assembly_loop(Abstract_Tree *node) {
    if (!node) {
        return;
    }

    Abstract_Tree *operators = node->left;
    Abstract_Tree *expression = node->right;


    char *l = label();
    char *tmp_name = tmp();
    printf("%s:\n", l);
    print_assembly_operators(operators);
    print_assembly_expression(expression, tmp_name);
    printf("cmp_true %s %s\n", tmp_name, l);

}

void print_assembly_assign(Abstract_Tree *node) {
    if (!node) {
        return;
    }

    Abstract_Tree *variable = node->left;
    Abstract_Tree *expression = node->right;

    char *tmp_name;

    switch (expression->type) {
        case Const:
        case Identifier:
            printf("%s %s %s\n", variable->value, node->value, expression->value);
            break;

        case Expression:
            tmp_name = tmp();
            print_assembly_expression(expression, tmp_name);
            printf("%s %s %s\n", variable->value, node->value, tmp_name);
            break;

        default:
            printf("ASSIGN ERROR\n");
            break;
    }
}

void print_assembly_expression(Abstract_Tree *node, char *name) {
    if (!node) {
        return;
    }

    Abstract_Tree *left = node->left;
    Abstract_Tree *right = node->right;

    char *tmp_name = NULL;

    if (left) {
        switch (left->type) {
            case Expression:
                tmp_name = tmp();
                print_assembly_expression(left, tmp_name);
                break;

            case Const:
            case Identifier:
                tmp_name = left->value;
                break;

            default:
                printf("EXPRESSION ERROR!\n");
                tmp_name = "EXPRESSION ERROR!";
                break;
        }
    }
    char *tmp_name_2 = NULL;

    if (right) {
        switch (right->type) {
            case Expression:
                tmp_name_2 = tmp();
                print_assembly_expression(right, tmp_name_2);
                break;

            case Const:
            case Identifier:
                tmp_name_2 = right->value;
                break;

            default:
                printf("EXPRESSION ERROR!\n");
                tmp_name_2 = "EXPRESSION ERROR!";
                break;
        }
    }

    if (strcmp(node->value, "-") == 0
        || strcmp(node->value, "+") == 0
        || strcmp(node->value, "*") == 0
        || strcmp(node->value, "/") == 0
        || strcmp(node->value, ">") == 0
        || strcmp(node->value, "<") == 0
        || strcmp(node->value, "==") == 0) {
        printf("%s = %s %s %s\n", name, tmp_name, node->value, tmp_name_2);
    } else if (strcmp(node->value, "MINUS") == 0 || strcmp(node->value, "NOT") == 0) {
        printf("%s = %s %s\n", name, node->value, tmp_name_2);
    } else {
        printf("EXPRESSION ERROR!\n");
    }

}

void print_assembly_if(Abstract_Tree *node) {
    if (!node) {
        return;
    }

    char *tmp_name = tmp();
    char *l = label();
    char *end_l = label();

    print_assembly_expression(node->left, tmp_name);
    printf("cmp_true %s %s\n", tmp_name, l);
    print_assembly_operators(node->right->left);
    printf("goto %s\n", end_l);
    printf("%s:\n", l);
    if (node->right->right) {
        print_assembly_operators(node->right->right);
    }
    printf("%s:\n", end_l);
}

char *tmp() {
    static int tmp_count = 0;
    char *name = malloc(sizeof(char) * 16);
    snprintf(name, 16, "tmp_%d", tmp_count);
    tmp_count += 1;
    return name;
}

char *label() {
    static int label_count = 0;
    char *l = malloc(sizeof(char) * 16);
    snprintf(l, 16, "label_%d", label_count);
    label_count += 1;
    return l;
}

char *get_node_type(Abstract_Tree *node) {
    if (node == NULL) {
        return NULL;
    }

    switch (node->type) {
        case Root:
            return "Root";

        case Variables_Declaration:
            return "Variables_Declaration";

        case Variables:
            return "Variables";

        case Identifier:
            return "Identifier";

        case Program_Body:
            return "Program_Body";

        case Expression:
            return "Expression";

        case Operators:
            return "Operators";

        case Operator:
            return "Operator";

        case Const:
            return "Const";

        case If:
            return "If";

        case If_Else:
            return "If_Else";

        case Condition_Expression:
            return "Condition_Expression";

        case Loop:
            return "Loop";

        default:
            return NULL;
    }
}


