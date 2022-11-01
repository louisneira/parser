#include <iostream>
#include "lexer.h"
#include <vector>

struct Variable {
    std::string name;
    TokenType token_type;
    int line_no;
};

struct Scope {
    std::string name;
    std::vector<Variable> public_vars;
    std::vector<Variable> private_vars;
    std::vector<Variable> vars;
    std::vector<std::string> l_vals;
    std::vector<std::string> r_vals;
    std::vector<std::string> stmts;
    Scope* parent = NULL;
};

bool parse_program();

bool parse_scope();

bool parse_global_vars();

bool parse_public_vars();

bool parse_private_vars();

bool parse_var_list(int);

bool parse_var(int);

bool parse_stmt_list();

bool parse_stmt();

void parse_error();

void syntax_error(int);

void print_statements();

void resolve_scopes(std::string, std::string);