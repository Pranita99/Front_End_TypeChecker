#ifndef SIMPLE_PARSER_HH
#define SIMPLE_PARSER_HH

#include <memory>
#include <istream>
#include <string>
#include "ast.hh"

class SimpleParser {
public:
    std::unique_ptr<Spec> parse(std::istream& input = std::cin);
    std::unique_ptr<Spec> parseFile(const std::string& filename);
};

#endif
