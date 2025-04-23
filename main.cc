#include "simple_parser.hh"
#include "ast.hh"
#include "ASTVis.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


int main(int argc, char* argv[]) {
    try {
        SimpleParser parser;
        std::unique_ptr<Spec> spec;

        if (argc > 1) {
            std::cout << "Parsing from file: " << argv[1] << std::endl;
            spec = parser.parseFile(argv[1]);
        } else {
            std::cout << "Using default sample API spec" << std::endl;
            std::istringstream sampleInput(
                "username: string\n"
                "login: string -> string\n"
                "LoginAPI:\n"
                "PRECONDITION: username\n"
                "login(username) ==> (OK, username)\n"
                "POSTCONDITION: username\n"
            );
            spec = parser.parse(sampleInput);
        }

        if (!spec) {
            std::cerr << "Error: Parser returned a null specification\n";
            return 1;
        }

        std::cout << "Parsing successful!\n";

        // Print the AST using PrintVisitor
        PrintVisitor printer;
        spec->accept(printer);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
