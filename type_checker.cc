#include "type_checker.hh"
#include <iostream>
#include <stdexcept>

using namespace std;

// Helper to compare type expressions
bool TypeChecker::matchTypes(const TypeExpr* expected, const TypeExpr* actual) {
    if (expected->typeExpression != actual->typeExpression)
        return false;

    if (expected->typeExpression == TypeExpression::TYPE_CONST) {
        auto* ec = dynamic_cast<const TypeConst*>(expected);
        auto* ac = dynamic_cast<const TypeConst*>(actual);
        return ec && ac && ec->name == ac->name;
    }

    if (expected->typeExpression == TypeExpression::FUNC_TYPE) {
        auto* ef = dynamic_cast<const FuncType*>(expected);
        auto* af = dynamic_cast<const FuncType*>(actual);
        if (!ef || !af || ef->params.size() != af->params.size()) return false;

        for (size_t i = 0; i < ef->params.size(); ++i)
            if (!matchTypes(ef->params[i].get(), af->params[i].get()))
                return false;

        return matchTypes(ef->returnType.get(), af->returnType.get());
    }

    return true;
}

void TypeChecker::addGlobal(const Decl* decl) {
    globals[decl->name] = decl->type.get();
}

void TypeChecker::addFunction(const FuncDecl* func) {
    vector<unique_ptr<TypeExpr>> paramClones;
    for (const auto& p : func->params) paramClones.push_back(p->clone());
    functions[func->name] = make_unique<FuncType>(move(paramClones), func->returnType.second[0]->clone());
}

void TypeChecker::typecheckExpr(const Expr* expr) {
    if (auto* v = dynamic_cast<const Var*>(expr)) {
        if (globals.find(v->name) == globals.end()) throw runtime_error("Undeclared variable: " + v->name);
    }
    else if (auto* f = dynamic_cast<const FuncCall*>(expr)) {
        if (functions.find(f->name) == functions.end()) throw runtime_error("Unknown function: " + f->name);

        const FuncType* declared = dynamic_cast<const FuncType*>(functions[f->name].get());
        if (!declared) throw runtime_error("Expected function type for: " + f->name);

        if (f->args.size() != declared->params.size()) throw runtime_error("Arity mismatch in call: " + f->name);
    }
}

void TypeChecker::typecheckAPI(const API* api) {
    if (api->pre) typecheckExpr(api->pre.get());
    if (api->call) typecheckExpr(api->call->call.get());
    if (api->response.expr) typecheckExpr(api->response.expr.get());
}

void TypeChecker::typecheckSpec(const Spec* spec) {
    for (const auto& g : spec->globals) addGlobal(g.get());
    for (const auto& f : spec->functions) addFunction(f.get());
    for (const auto& api : spec->blocks) typecheckAPI(api.get());
}