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
