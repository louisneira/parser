#include <iostream>
#include <string>
#include <vector>

#include "lexer.h"
#include "inputbuf.h"
#include "parser.h"


int main() {

    LexicalAnalyzer lexer;
    Token token;
    
    
    /*
    token = lexer.GetToken();
    token.Print();

    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
    */

    parse_program();
 
	return 0;
}