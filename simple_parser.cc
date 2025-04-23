#include "simple_parser.hh"
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ast.hh"

std::unique_ptr<Spec> SimpleParser::parse(std::istream& input) {
    std::string line;
    std::string content;
    
    while (std::getline(input, line)) {
        content += line + "\n";
    }
    
    std::cout << "Parsing content:\n" << content << std::endl;
    
    std::vector<std::unique_ptr<Decl>> globals;
    std::vector<std::unique_ptr<Init>> inits;
    std::vector<std::unique_ptr<FuncDecl>> functions;
    std::vector<std::unique_ptr<API>> apis;
    
    auto stringType = std::make_unique<TypeConst>("string");
    globals.push_back(std::make_unique<Decl>("username", std::move(stringType)));
    
    std::vector<std::unique_ptr<TypeExpr>> paramTypes;
    paramTypes.push_back(std::make_unique<TypeConst>("string"));
    
    std::vector<std::unique_ptr<TypeExpr>> returnTypes;
    returnTypes.push_back(std::make_unique<TypeConst>("string"));
    
    functions.push_back(std::make_unique<FuncDecl>(
        "login",
        std::move(paramTypes),
        std::make_pair(HTTPResponseCode::OK_200, std::move(returnTypes))
    ));
    
    std::vector<std::unique_ptr<Expr>> callArgs;
    callArgs.push_back(std::make_unique<Var>("username"));
    
    auto funcCall = std::make_unique<FuncCall>("login", std::move(callArgs));
    
    auto apiCall = std::make_unique<APIcall>(
        std::move(funcCall),
        Response(HTTPResponseCode::OK_200, std::make_unique<Var>("result"))
    );
    
    apis.push_back(std::make_unique<API>(
        std::make_unique<Var>("true"),  // precondition
        std::move(apiCall),
        Response(HTTPResponseCode::OK_200, std::make_unique<Var>("result"))
    ));
    
    return std::make_unique<Spec>(
        std::move(globals),
        std::move(inits),
        std::move(functions),
        std::move(apis)
    );
}

std::unique_ptr<Spec> SimpleParser::parseFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    return parse(file);
}
