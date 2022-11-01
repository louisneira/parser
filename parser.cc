#include "lexer.h"
#include "inputbuf.h"
#include "parser.h"
#include <vector>

std::vector<Variable> global_vars;
std::vector<Scope> scopes;
std::vector<std::string> statements;
LexicalAnalyzer lexer;

//Program parses to global_vars and then to scope per this grammar
//Though global_vars can parse to epsilon, meaning that this can parse to only scope
bool parse_program() {

    //LexicalAnalyzer lexer;
    Token token, token2;

    Scope* current_scope = new Scope();
    current_scope->name = "::";

    scopes.push_back(*current_scope);

    //token = lexer.GetToken();

    parse_global_vars();
    
    if (!parse_scope()) {
        syntax_error(0);
        return false;
	}

	if(lexer.GetToken().token_type != END_OF_FILE) {
		syntax_error(0);
		return false;
	}
	
	print_statements();
	
    return true;
}

bool parse_global_vars() {

    //std::cout << "parse_global_vars \n";
    //LexicalAnalyzer lexer;
    Token token;

    if (parse_var_list(0)) {

        token = lexer.GetToken();
        //std::cout << "parse_global_vars(), Token: " << token.lexeme << ", Token Type: " << token.token_type << "\n";

        if (token.token_type == SEMICOLON)
            return true;
        else
            syntax_error(1);
        
    }

    return false;
}

bool parse_var_list(int list_type) {
    //std::cout << "parse_var_list \n";
    //LexicalAnalyzer lexer;
    Token token, token2;
	
	token = lexer.GetToken();
	lexer.UngetToken(token);

    if(parse_var(list_type)) {

        token2 = lexer.GetToken();
        //std::cout << "parse_var_list(), list type: " << list_type << " Token: " << token2.lexeme << ", Token type: " << token2.token_type << "\n";

        if (token2.token_type == COMMA)
            return parse_var_list(list_type);

        else if (token2.token_type == SEMICOLON) {
            lexer.UngetToken(token2);
            return true;
        }
		else {
			lexer.UngetToken(token2);
			lexer.UngetToken(token);
            return false;
		}
    }

    return false;
}

bool parse_var(int var_type) {
    //std::cout << "parse_var \n";
    //LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    //std::cout << "parse_var() - Token: " << token.lexeme << ", Token Type: " << token.token_type << "\n";

    if (token.token_type == ID) {

        Variable var;
        var.name = token.lexeme;
        var.token_type = token.token_type;
        var.line_no = token.line_no;

        if (var_type == 1)
            scopes.back().public_vars.push_back(var);

        else if(var_type == 2)
            scopes.back().private_vars.push_back(var);
        
        else
            scopes.back().vars.push_back(var);

        return true;
    }

    return false;
}

bool parse_scope() {
    //std::cout << "parse_scope \n";
    //LexicalAnalyzer lexer;
    Token token, token2, token3;

    token = lexer.GetToken();
    //std::cout << "parse_scope() , Token: " << token.lexeme << ", Token type: " << token.token_type << "\n";
    
    if (token.token_type == ID) {
        token2 = lexer.GetToken();
        //std::cout << "parse_scope() , Token2: " << token2.lexeme << ", Token type: " << token2.token_type << "\n";

        if (token2.token_type == LBRACE) {

            Scope* current_scope = new Scope();
			current_scope->name = token.lexeme + ".";
            scopes.push_back(*current_scope);

            parse_public_vars();
            parse_private_vars();
            parse_stmt_list();

            token3 = lexer.GetToken();
            //std::cout << "parse_scope() , Token3: " << token3.lexeme << ", Token type: " << token3.token_type << "\n";

            if (token3.token_type == RBRACE) {
				scopes.pop_back();
                return true;
			}
			
			else {
				syntax_error(2);
				return false;
			}
        }

        else {
            syntax_error(2);
            return false;
        }
    }

    else {
        syntax_error(2);
        return false;
    }
}

bool parse_public_vars() {
    //std::cout << "parse_public_vars \n";
    //LexicalAnalyzer lexer;
    Token token, token2, token3;

    token = lexer.GetToken();
    //std::cout << "parse_public_vars() , Token: " << token.lexeme << ", Token type: " << token.token_type << "\n";

    if (token.token_type == PUBLIC) {
        token2 = lexer.GetToken();
        //std::cout << "parse_public_vars() , Token2: " << token2.lexeme << ", Token type: " << token2.token_type << "\n";

        if (token2.token_type == COLON) {
            
            if (parse_var_list(1)) {
                token3 = lexer.GetToken();
                //std::cout << "parse_public_vars() , Token3: " << token3.lexeme << ", Token type: " << token3.token_type << "\n";

                if (token3.token_type == SEMICOLON) 
                    return true;

                else
                    syntax_error(3);
            }

            else
                syntax_error(3);
        }

        else
            syntax_error(3);
    }

    else {
        lexer.UngetToken(token);
        return false;
    }

    return false;
}

bool parse_private_vars() {
    //std::cout << "parse_private_vars \n";
    //LexicalAnalyzer lexer;
    Token token, token2, token3;

    token = lexer.GetToken();
    //std::cout << "parse_private_vars() , Token: " << token.lexeme << ", Token type: " << token.token_type << "\n";

    if (token.token_type == PRIVATE) {
        token2 = lexer.GetToken();
        //std::cout << "parse_private_vars() , Token2: " << token2.lexeme << ", Token type: " << token2.token_type << "\n";

        if (token2.token_type == COLON) {

            if (parse_var_list(2)) {
                token3 = lexer.GetToken();
                //std::cout << "parse_private_vars() , Token3: " << token3.lexeme << ", Token type: " << token3.token_type << "\n";

                if (token3.token_type == SEMICOLON)
                    return true;

                else
                    syntax_error(4);
            }

            else
                syntax_error(4);
        }

        else
            syntax_error(4);
    }

    else {
        lexer.UngetToken(token);
        return false;
    }

    return false;
}

bool parse_stmt_list() {
    //std::cout << "parse_stmt_list \n";
    //LexicalAnalyzer lexer;
    Token token;

    if(parse_stmt()) {

        token = lexer.GetToken();
        //std::cout << "parse_stmt_list() , Token: " << token.lexeme << ", Token type: " << token.token_type << "\n";

        if (token.token_type == RBRACE) {
            lexer.UngetToken(token);
            return true;
        }

        else {
            lexer.UngetToken(token);
            return parse_stmt_list();
        }
    }

    else {
		//std::cout << "parse_stmt_list() , Token: " << token.lexeme << ", Token type: " << token.token_type << "\n";
		syntax_error(5);
		return false;
	}
}

bool parse_stmt() {
    //std::cout << "parse_stmt \n";
    //LexicalAnalyzer lexer;
    Token token, token2, token3, token4;
    std::string left_var = "";
    std::string right_var = "";

    token = lexer.GetToken();
    //std::cout << "parse_stmt() , Token: " << token.lexeme << ", Token type: " << token.token_type << "\n";

    if (token.token_type == ID) {
        token2 = lexer.GetToken();
        //std::cout << "parse_stmt() , Token2: " << token2.lexeme << ", Token type: " << token2.token_type << "\n";

        if (token2.token_type == EQUAL) {
            token3 = lexer.GetToken();
            //std::cout << "parse_stmt() , Token3: " << token3.lexeme << ", Token type: " << token3.token_type << "\n";

            if (token3.token_type == ID) {
                token4 = lexer.GetToken();
                //std::cout << "parse_stmt() , Token4: " << token4.lexeme << ", Token type: " << token4.token_type << "\n";

                if (token4.token_type == SEMICOLON) {
                    left_var = token.lexeme;
                    right_var = token3.lexeme;

                    std::string stmt = token.lexeme + "=" + token3.lexeme;
                    scopes.back().l_vals.push_back(left_var);
                    scopes.back().r_vals.push_back(right_var);
                    resolve_scopes(left_var, right_var);
                    return true;
                }

                else
                    syntax_error(6);
            }

            else
                syntax_error(6);
        }

        else if (token2.token_type == LBRACE) {
            lexer.UngetToken(token2);
            lexer.UngetToken(token);
            return parse_scope();
        }
    }

    return false;
}

void resolve_scopes(std::string l_val, std::string r_val) {
    
    std::string left_scope = "?.";
    std::string right_scope = "?.";

    for (int i = 0; i < scopes.size(); i++) {

		for (int j = 0; j < scopes[i].vars.size(); j++) {

            if (l_val == scopes[i].vars[j].name) {
                left_scope = scopes[i].name;
				//std::cout << "1 Current Scope: " << scopes.back().name << ", Var Scope: " << left_scope << ", Variable: " << l_val << "\n";
			}
            if (r_val == scopes[i].vars[j].name) {
				right_scope = scopes[i].name;
				//std::cout << "2 Current Scope: " << scopes.back().name << ", Var Scope: " << right_scope << ", Variable: " << r_val << "\n";
			}
        }

        if (i == scopes.size() - 1) {
            
            for (int j = 0; j < scopes[i].private_vars.size(); j++) {

                if (l_val == scopes[i].private_vars[j].name)
				{
                    left_scope = scopes[i].name;
					//std::cout << "3 Current Scope: " << scopes.back().name << ", Var Scope: " << left_scope << ", Variable: " << l_val << "\n";
				}
                if (r_val == scopes[i].private_vars[j].name)
				{
                    right_scope = scopes[i].name;
					//std::cout << "4 Current Scope: " << scopes.back().name << ", Var Scope: " << right_scope << ", Variable: " << r_val << "\n";
				}
            }
        }

        for (int j = 0; j < scopes[i].public_vars.size(); j++) {

            if (l_val == scopes[i].public_vars[j].name)
			{
                left_scope = scopes[i].name;
				//std::cout << "5 Current Scope: " << scopes.back().name << ", Var Scope: " << left_scope << ", Variable: " << l_val << "\n";
			}
            if (r_val == scopes[i].public_vars[j].name)
			{
                right_scope = scopes[i].name;
				//std::cout << "6 Current Scope: " << scopes.back().name << ", Var Scope: " << right_scope << ", Variable: " << r_val << "\n";
			}
        }
    }

    statements.push_back(left_scope + l_val + " = " + right_scope + r_val);
}

void print_statements() {
	for (int i = 0; i < statements.size(); i++)
		std::cout << statements[i] << "\n";
}

void syntax_error(int num) {

	std::cout << "Syntax Error\n";
	exit(1);
}

