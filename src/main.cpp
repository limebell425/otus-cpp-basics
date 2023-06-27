// OTUS C++ Basic course homework skeleton.
// Lexer using example

#include <iostream>
#include <exception>

#include "astnode.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main() {
    Lexer lexer(std::cin);
    Parser parser(lexer);

    try{
       ASTNode *ast = parser.parse();
        if (ast) {
         ast->print(std::cout);
        }
    }
    catch (const std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}
