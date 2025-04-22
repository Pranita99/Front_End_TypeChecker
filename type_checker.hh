#ifndef TYPE_CHECKER_HH
#define TYPE_CHECKER_HH

#include "ast.hh"
#include <unordered_map>
#include <memory>
#include <string>


class TypeChecker {
    std::unordered_map<std::string, const TypeExpr*> globals;
    std::unordered_map<std::string, std::unique_ptr<TypeExpr>> functions;

    bool matchTypes(const TypeExpr* expected, const TypeExpr* actual);
    void addGlobal(const Decl* decl);
    void addFunction(const FuncDecl* func);
    void typecheckAPI(const API* api);

public:
    void typecheckExpr(const Expr* expr);
    void typecheckSpec(const Spec* spec);
};

#endif
