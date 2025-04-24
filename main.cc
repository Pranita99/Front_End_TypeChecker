<<<<<<< HEAD
#include "type_checker.hh"
#include <iostream>

int main() {
    try {
        //Parser parser;
        //Spec program = parser.parse();            // or Program, if that’s what parser gives
        TypeChecker checker;
        //checker.typecheckSpec(&program);          // passing pointer, since that's what your checker expects
        std::cout << "success!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Typechecking failed: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
=======
#include "ast.hh"
#include "ASTVis.hpp"
#include <fstream>

extern FILE* yyin;
extern int yyparse();
extern Spec* astRoot;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        std::cerr << "Could not open file: " << argv[1] << std::endl;
        return 1;
    }

    if (yyparse() != 0) {
        std::cerr << "Parsing failed!" << std::endl;
        return 1;
    }

    if (astRoot) {
        PrintVisitor printer;
        std::cout << "Generated AST:" << std::endl;
        astRoot->accept(printer);
    }

    fclose(yyin);
    return 0;
}
>>>>>>> master
