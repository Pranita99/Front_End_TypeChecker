<<<<<<< HEAD
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "ASTVis.hpp"
//#include "jsCodeGenerator/visitor.h"
// --------------------- PLEASE READ THIS BEFORE PROCEEDING ---------------------------------------

// ASTvisitor ====> virtual class, printervisitor==> implementation of AST visitor and is used to convert the whole thing into a string 
// ExpoSEVisitor Class for jsCode generation.
using namespace std;
#ifndef AST_HPP
#define AST_HPP

enum class HTTPResponseCode
{
    OK_200,
    CREATED_201,
    BAD_REQUEST_400,
    // Add other response codes as needed
};
enum ExpressionType
{
    MAP,
    VAR,
    STRING,
    NUM,
    TUPLE,
    SET,
    FUNCTIONCALL_EXPR,
    POLYMORPHIC_FUNCTIONCALL_EXPR
};

enum TypeExpression
{
    TYPE_CONST,
    TYPE_VARIABLE,
    FUNC_TYPE,
    MAP_TYPE,
    SET_TYPE,
    TUPLE_TYPE
};

enum StatementType
{
    ASSIGN,
    FUNCTIONCALL_STMT,
    DECL,
};

class fundecl
{
public:
    std::string name;
    std::unique_ptr<TypeExpr> params;
    std::unique_ptr<TypeExpr> outp;
    fundecl(std::string name, std::unique_ptr<TypeExpr> param, std::unique_ptr<TypeExpr> outp) : name(std::move(name)), params(std::move(param)), outp(std::move(outp)) {};
};
class TypeExpr
{
public:
    virtual ~TypeExpr() = default;
    virtual void accept(ASTVisitor &visitor) const = 0;
    virtual std::unique_ptr<TypeExpr> clone() const = 0;
    TypeExpression typeExpression;

protected:
    TypeExpr(TypeExpression typeExpr) : typeExpression(typeExpr) {}
};

// Type Expressions
class TypeConst : public TypeExpr
{
public:
    explicit TypeConst(std::string name) : TypeExpr(TypeExpression::TYPE_CONST), name(std::move(name)) {}
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
    }
    std::unique_ptr<TypeExpr> clone() const override
    {
        return std::make_unique<TypeConst>(name);
    }
    std::string name;
};

class FuncType : public TypeExpr
{
public:
    FuncType(std::vector<std::unique_ptr<TypeExpr>> params, std::unique_ptr<TypeExpr> returnType)
        : TypeExpr(TypeExpression::FUNC_TYPE), params(std::move(params)), returnType(std::move(returnType)) {}
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        for (auto &param : params)
        {
            // visitor->visitTypeExpr(*param);
        }
        // visitor->visitTypeExpr(*returnType);
    }
    std::unique_ptr<TypeExpr> clone() const override
    {
        std::vector<std::unique_ptr<TypeExpr>> clonedParams;
        for (const auto &param : params)
        {
            clonedParams.push_back(param->clone());
        }
        auto clonedReturnType = returnType ? returnType->clone() : nullptr;
        return std::make_unique<FuncType>(std::move(clonedParams), std::move(clonedReturnType));
    }

    std::vector<std::unique_ptr<TypeExpr>> params;
    std::unique_ptr<TypeExpr> returnType;
};

class MapType : public TypeExpr
{
public:
    MapType(std::unique_ptr<TypeExpr> domain, std::unique_ptr<TypeExpr> range)
        : TypeExpr(TypeExpression::MAP_TYPE), domain(std::move(domain)), range(std::move(range)) {}
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        // visitor->visitTypeExpr(*domain);
        // visitor->visitTypeExpr(*range);
    }
    std::unique_ptr<TypeExpr> clone() const override
    {
        auto clonedDomain = domain ? domain->clone() : nullptr;
        auto clonedRange = range ? range->clone() : nullptr;
        return std::make_unique<MapType>(std::move(clonedDomain), std::move(clonedRange));
    }

    std::unique_ptr<TypeExpr> domain;
    std::unique_ptr<TypeExpr> range;
};

class TupleType : public TypeExpr
{
public:
    explicit TupleType(std::vector<std::unique_ptr<TypeExpr>> elements)
        : TypeExpr(TypeExpression::TUPLE_TYPE), elements(std::move(elements)) {}
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        for (auto &e : elements)
        {
            // visitor->visitTypeExpr(*e);
        }
    }
    // Clone implementation for TupleType
    std::unique_ptr<TypeExpr> clone() const override
    {
        std::vector<std::unique_ptr<TypeExpr>> clonedElements;
        for (const auto &element : elements)
        {
            clonedElements.push_back(element->clone());
        }
        return std::make_unique<TupleType>(std::move(clonedElements));
    }

    std::vector<std::unique_ptr<TypeExpr>> elements;
};

class SetType : public TypeExpr
{
public:
    explicit SetType(std::unique_ptr<TypeExpr> elementType)
        : TypeExpr(TypeExpression::SET_TYPE), elementType(std::move(elementType)) {}
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        // visitor->visitTypeExpr(*elementType);
    }
    std::unique_ptr<TypeExpr> clone() const override
    {
        auto clonedElementType = elementType ? elementType->clone() : nullptr;
        return std::make_unique<SetType>(std::move(clonedElementType));
    }

    std::unique_ptr<TypeExpr> elementType;
};
class Decl
{
public:
    Decl(std::string name, std::unique_ptr<TypeExpr> typeExpr)
        : name(std::move(name)), type(std::move(typeExpr)) {}
    virtual ~Decl() = default;
    virtual void accept(ASTVisitor &visitor) {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        // visitor->visitTypeExpr(*type);
    }
    // Copy constructor for deep copying.
    Decl(const Decl &other)
        : name(other.name)
    {
        if (other.type)
        {
            // This assumes that TypeExpr has a clone() method.
            type = other.type->clone();
        }
    }
    virtual std::unique_ptr<Decl> clone() const
    {
        return std::make_unique<Decl>(*this);
    }

    std::string name;
    std::unique_ptr<TypeExpr> type;
};
// Expressions
class Expr
{
public:
    virtual ~Expr() = default;
    virtual void accept(ASTVisitor& visitor) const = 0;

    ExpressionType expressionType;

protected:
    Expr(ExpressionType exprType) : expressionType(exprType) {}
};
// class PolymorphicFuncCall : public Expr
// {
// public:
//     PolymorphicFuncCall(
//         std::string name,
//         std::vector<std::unique_ptr<TypeExpr>> typeArgs,
//         std::vector<std::unique_ptr<Expr>> args) : 
//         Expr(ExpressionType::POLYMORPHIC_FUNCTIONCALL_EXPR), name(std::move(name)),
//                                                    typeArgs(std::move(typeArgs)),
//                                                    args(std::move(args)) {}

// //     std::string name;                                // Name of the polymorphic function
// //     std::vector<std::unique_ptr<TypeExpr>> typeArgs; // Type arguments for polymorphism
// //     std::vector<std::unique_ptr<Expr>> args;         // Regular arguments
// //     void accept(ASTVisitor &visitor) const override
// //     {
// //         visitor.visit(*this);
// //     }

//     void accept(ExpoSEVisitor *visitor)
//     {
//         for (auto &typeArg : typeArgs)
//         {
//             // visitor->visitTypeExpr(*typeArg);
//         }

//         for (auto &arg : args)
//         {
//             visitor->visitExpr(*arg);
//         }
//     }
// };
class Var : public Expr
{
public:
    explicit Var(std::string name) : Expr(ExpressionType::VAR), name(std::move(name)) {}
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }
    bool operator<(const Var &other) const
    {
        return name < other.name; // Lexicographical comparison
    }

    void accept(ExpoSEVisitor *visitor)
    {
    }
    std::string name;
};

class FuncCall : public Expr
{
public:
    FuncCall(std::string name, std::vector<std::unique_ptr<Expr>> args)
        : Expr(ExpressionType::FUNCTIONCALL_EXPR), name(std::move(name)), args(std::move(args)) {}
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        for (auto &arg : args)
        {
            visitor->visitExpr(*arg);
        }
    }
    std::string name;
    std::vector<std::unique_ptr<Expr>> args;
};

class Num : public Expr
{
public:
    explicit Num(int value) : Expr(ExpressionType::NUM), value(value) {}
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        visitor->visitNum(*this);
    }

    int value;
};

class String : public Expr
{
public:
    explicit String(string value) : Expr(ExpressionType::STRING), value(value) {}
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }

    string value;
};

class Set : public Expr
{
public:
    explicit Set(std::vector<std::unique_ptr<Expr>> elements)
        : Expr(ExpressionType::SET), elements(std::move(elements)) {}
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        for (auto &e : elements)
        {
            visitor->visitExpr(*e);
        }
    }
    std::vector<std::unique_ptr<Expr>> elements;
};

class Map : public Expr
{
public:
    explicit Map(std::vector<std::pair<std::unique_ptr<Var>, std::unique_ptr<Expr>>>) : Expr(ExpressionType::MAP), value(std::move(value)) {}
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        for (auto &v : value)
        {
            visitor->visitVar(*(v.first));
            visitor->visitExpr(*(v.second));
        }
    }

    std::vector<std::pair<std::unique_ptr<Var>, std::unique_ptr<Expr>>> value;
};

class Tuple : public Expr
{
public:
    explicit Tuple(std::vector<std::unique_ptr<Expr>> exprs) : Expr(ExpressionType::TUPLE), expr(std::move(expr)) {}
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        for (auto &e : expr)
        {
            visitor->visitExpr(*e);
        }
    }
    std::vector<std::unique_ptr<Expr>> expr;
};

// Function Declaration
class FuncDecl
{
public:
    FuncDecl(std::string name,
             std::vector<std::unique_ptr<TypeExpr>> params,
             std::pair<HTTPResponseCode, vector<std::unique_ptr<TypeExpr>>> returnType)
        : name(std::move(name)), params(std::move(params)), returnType(std::move(returnType)) {}
    void accept(ASTVisitor &visitor) const
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        // for(auto& param: params){
        //     visitor->visitTypeExpr(*param);
        // }
        // visitor->visitHTTPResponseCode(returnType.first);
        for(auto& te: returnType.second){
            // visitor->visitTypeExpr(*te);
        }
    }

    
    std::string name;
    std::vector<std::unique_ptr<TypeExpr>> params;
    std::pair<HTTPResponseCode, vector<std::unique_ptr<TypeExpr>>> returnType;
};

// Initialization
class Init
{
public:
    Init(std::string varName, std::unique_ptr<Expr> expression)
        : varName(std::move(varName)), expr(std::move(expression)) {}
    void accept(ASTVisitor &visitor) const
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        visitor->visitExpr(*expr);
    }

    std::string varName;
    std::unique_ptr<Expr> expr;
};
class Response
{
public:
    HTTPResponseCode code;
    std::unique_ptr<Expr> expr;
    Response(HTTPResponseCode code, std::unique_ptr<Expr> expr) : code(code), expr(std::move(expr)) {};
    void accept(ASTVisitor &visitor) const
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        // visitor->visitHTTPResponseCode(code);
        visitor->visitExpr(*expr);
    }
};
class APIcall
{
public:
    std::unique_ptr<FuncCall> call;
    Response response;
    void accept(ASTVisitor &visitor) const
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        visitor->visitFuncCall(*call);
        // visitor->visitResponse(response);
    }

    APIcall(std::unique_ptr<FuncCall> call, Response response) : call(std::move(call)), response(std::move(response)) {};
};
// API
class API
{
public:
    API(std::unique_ptr<Expr> precondition,
        std::unique_ptr<APIcall> functionCall,
        Response response)
        : pre(std::move(precondition)), call(std::move(functionCall)), response(std::move(response)) {}
    void accept(ASTVisitor &visitor) const
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        visitor->visitExpr(*pre);
        // visitor->visitAPIcall(*call);
        // visitor->visitResponse(response);
    }
    std::unique_ptr<Expr> pre;
    std::unique_ptr<APIcall> call;
    Response response;
};

// Block class (placeholder as it wasn't fully specified in the grammar)
// Top-level Spec class
class Spec
{
public:
    Spec(std::vector<std::unique_ptr<Decl>> globals,
         //  std::vector<std::unique_ptr<TypeDecl>> types,
         std::vector<unique_ptr<Init>> init,
         std::vector<std::unique_ptr<FuncDecl>> functions,
         std::vector<std::unique_ptr<API>> blocks)
        : globals(std::move(globals)), init(std::move(init)), functions(std::move(functions)), blocks(std::move(blocks))
    {
    }
    void accept(ASTVisitor &visitor) const
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        // for(auto& global: globals) {
        //     visitor->visitDecl(*global);
        // }

        for(auto& i: init){
            visitor->visitInit(*i);
        }

        for(auto& function: functions){
            // visitor->visitFuncDecl(*function);
        }

        for(auto& block: blocks){
            // visitor->visitAPI(*block);
        }
    }

    std::vector<std::unique_ptr<Decl>> globals;
    // std::vector<std::unique_ptr<TypeDecl>> types;
    std::vector<unique_ptr<Init>> init;
    std::vector<std::unique_ptr<FuncDecl>> functions;
    std::vector<std::unique_ptr<API>> blocks;
};

class Stmt
{
public:
    virtual ~Stmt() = default;
    virtual void accept(ASTVisitor &visitor) const = 0;

    StatementType statementType;

protected:
    Stmt(StatementType type) : statementType(type) {}
};

// Assignment statement: l = r
class Assign : public Stmt
{
public:
    Assign(std::unique_ptr<Var> left, std::unique_ptr<Expr> right)
        : Stmt(StatementType::ASSIGN), left(std::move(left)), right(std::move(right)) {}
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        visitor->visitVar(*left);
        visitor->visitExpr(*right);
    }
    std::unique_ptr<Var> left;
    std::unique_ptr<Expr> right;
};

// Function call statement
class FuncCallStmt : public Stmt
{
public:
    explicit FuncCallStmt(std::unique_ptr<FuncCall> call)
        : Stmt(StatementType::FUNCTIONCALL_STMT), call(std::move(call)) {}
    void accept(ASTVisitor &visitor) const override
    {
        visitor.visit(*this);
    }

    void accept(ExpoSEVisitor *visitor)
    {
        visitor->visitFuncCall(*call);
    }

    std::unique_ptr<FuncCall> call;
};

// Program is the root of our AST
class Program
{
public:
    explicit Program(std::vector<std::unique_ptr<Stmt>> statements)
        : statements(std::move(statements)) {}
    void accept(ASTVisitor &visitor)
    {
        visitor.visit(*this);
    }
    void accept(ExpoSEVisitor *visitor)
    {

        // for (auto &decl : declarations)
        // {
        //     visitor->visitDecl(*decl);
        // }

        for (auto &stmt : statements)
        { // Use const reference to avoid unnecessary copies
            visitor->visitStmt(*stmt);
        }
    }

    std::vector<std::unique_ptr<Stmt>> statements;
};

#endif
=======
#ifndef AST_HH
#define AST_HH

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "ASTVis.hpp"

using namespace std;

enum class HTTPResponseCode {
    OK_200,
    CREATED_201,
    BAD_REQUEST_400,
};

enum ExpressionType {
    MAP,
    VAR,
    STRING,
    NUM,
    TUPLE,
    SET,
    FUNCTIONCALL_EXPR,
    POLYMORPHIC_FUNCTIONCALL_EXPR
};

enum TypeExpression {
    TYPE_CONST,
    TYPE_VARIABLE,
    FUNC_TYPE,
    MAP_TYPE,
    SET_TYPE,
    TUPLE_TYPE
};

enum StatementType {
    ASSIGN,
    FUNCTIONCALL_STMT,
    DECL,
};

// Forward declarations
class TypeExpr;
class Expr;
class Var;
class FuncCall;
class Num;
class String;
class Set;
class Map;
class Tuple;
class Decl;
class FuncDecl;
class Init;
class Response;
class APIcall;
class API;
class Spec;
class Stmt;
class Assign;
class FuncCallStmt;
class Program;

class TypeExpr {
public:
    virtual ~TypeExpr() = default;
    virtual void accept(ASTVisitor &visitor) const = 0;
    virtual std::unique_ptr<TypeExpr> clone() const = 0;
    TypeExpression typeExpression;

protected:
    explicit TypeExpr(TypeExpression typeExpr) : typeExpression(typeExpr) {}
};

class TypeConst : public TypeExpr {
public:
    explicit TypeConst(string name) : TypeExpr(TYPE_CONST), name(move(name)) {}
    void accept(ASTVisitor &visitor) const override { visitor.visit(*this); }
    std::unique_ptr<TypeExpr> clone() const override { return make_unique<TypeConst>(name); }
    
    string name;
};

class FuncType : public TypeExpr {
public:
    FuncType(vector<unique_ptr<TypeExpr>> params, unique_ptr<TypeExpr> returnType)
        : TypeExpr(FUNC_TYPE), params(move(params)), returnType(move(returnType)) {}
    
    void accept(ASTVisitor &visitor) const override { visitor.visit(*this); }
    std::unique_ptr<TypeExpr> clone() const override {
        vector<unique_ptr<TypeExpr>> clonedParams;
        for (const auto &param : params) {
            clonedParams.push_back(param->clone());
        }
        return make_unique<FuncType>(move(clonedParams), returnType ? returnType->clone() : nullptr);
    }
    
    vector<unique_ptr<TypeExpr>> params;
    unique_ptr<TypeExpr> returnType;
};

class MapType : public TypeExpr {
public:
    MapType(unique_ptr<TypeExpr> domain, unique_ptr<TypeExpr> range)
        : TypeExpr(MAP_TYPE), domain(move(domain)), range(move(range)) {}
    
    void accept(ASTVisitor &visitor) const override { visitor.visit(*this); }
    std::unique_ptr<TypeExpr> clone() const override {
        return make_unique<MapType>(domain ? domain->clone() : nullptr, 
                                  range ? range->clone() : nullptr);
    }
    
    unique_ptr<TypeExpr> domain;
    unique_ptr<TypeExpr> range;
};

    /*class TupleType : public TypeExpr {
public:
    explicit TupleType(vector<unique_ptr<TypeExpr>> elements)
        : TypeExpr(TUPLE_TYPE), elements(move(elements)) {}
    
    void accept(ASTVisitor &visitor) const override { visitor.visit(*this); }
    std::unique_ptr<TypeExpr> clone() const override {
        vector<unique_ptr<TypeExpr>> clonedElements;
        for (const auto &elem : elements) {
            clonedElements.push_back(elem->clone());
        }
        return make_unique<TupleType>(move(clonedElements));
    }
    
    vector<unique_ptr<TypeExpr>> elements;
};*/

class TupleType : public TypeExpr {
    public:
        explicit TupleType(std::vector<std::unique_ptr<TypeExpr>> elements)
            : TypeExpr(TypeExpression::TUPLE_TYPE), elements(std::move(elements)) {}
        
        void accept(ASTVisitor &visitor) const override { visitor.visit(*this); }
        
        std::vector<std::unique_ptr<TypeExpr>> elements;
    };

class SetType : public TypeExpr {
public:
    explicit SetType(unique_ptr<TypeExpr> elementType)
        : TypeExpr(SET_TYPE), elementType(move(elementType)) {}
    
    void accept(ASTVisitor &visitor) const override { visitor.visit(*this); }
    std::unique_ptr<TypeExpr> clone() const override {
        return make_unique<SetType>(elementType ? elementType->clone() : nullptr);
    }
    
    unique_ptr<TypeExpr> elementType;
};

class Expr {
public:
    virtual ~Expr() = default;
    virtual void accept(ASTVisitor &visitor) const = 0;
    ExpressionType expressionType;

protected:
    explicit Expr(ExpressionType type) : expressionType(type) {}
};

class Var : public Expr {
public:
    explicit Var(string name) : Expr(VAR), name(move(name)) {}
    void accept(ASTVisitor &visitor) const override { visitor.visit(*this); }
    
    string name;
};

class FuncCall : public Expr {
public:
    FuncCall(string name, vector<unique_ptr<Expr>> args)
        : Expr(FUNCTIONCALL_EXPR), name(move(name)), args(move(args)) {}
    
    void accept(ASTVisitor &visitor) const override { visitor.visit(*this); }
    
    string name;
    vector<unique_ptr<Expr>> args;
};

class Num : public Expr {
public:
    explicit Num(int value) : Expr(NUM), value(value) {}
    void accept(ASTVisitor &visitor) const override { visitor.visit(*this); }
    
    int value;
};

class String : public Expr {
public:
    explicit String(string value) : Expr(STRING), value(move(value)) {}
    void accept(ASTVisitor &visitor) const override { visitor.visit(*this); }
    
    string value;
};

class Set : public Expr {
public:
    explicit Set(vector<unique_ptr<Expr>> elements)
        : Expr(SET), elements(move(elements)) {}
    
    void accept(ASTVisitor &visitor) const override { visitor.visit(*this); }
    
    vector<unique_ptr<Expr>> elements;
};

class Map : public Expr {
public:
    explicit Map(vector<pair<unique_ptr<Var>, unique_ptr<Expr>>> value)
        : Expr(MAP), value(move(value)) {}
    
    void accept(ASTVisitor &visitor) const override { visitor.visit(*this); }
    
    vector<pair<unique_ptr<Var>, unique_ptr<Expr>>> value;
};

class Tuple : public Expr {
    public:
        explicit Tuple(std::vector<std::unique_ptr<Expr>> elements)
            : Expr(ExpressionType::TUPLE), elements(std::move(elements)) {}
        
        void accept(ASTVisitor &visitor) const override { visitor.visit(*this); }
        
        std::vector<std::unique_ptr<Expr>> elements;
    };

class Decl {
public:
    Decl(string name, unique_ptr<TypeExpr> type)
        : name(move(name)), type(move(type)) {}
    
    virtual ~Decl() = default;
    virtual void accept(ASTVisitor &visitor) const { visitor.visit(*this); }
    
    string name;
    unique_ptr<TypeExpr> type;
};

class FuncDecl {
public:
    FuncDecl(string name,
             vector<unique_ptr<TypeExpr>> params,
             pair<HTTPResponseCode, vector<unique_ptr<TypeExpr>>> returnType)
        : name(move(name)), params(move(params)), returnType(move(returnType)) {}
    
    void accept(ASTVisitor &visitor) const { visitor.visit(*this); }
    
    string name;
    vector<unique_ptr<TypeExpr>> params;
    pair<HTTPResponseCode, vector<unique_ptr<TypeExpr>>> returnType;
};

class Init {
public:
    Init(string varName, unique_ptr<Expr> expr)
        : varName(move(varName)), expr(move(expr)) {}
    
    void accept(ASTVisitor &visitor) const { visitor.visit(*this); }
    
    string varName;
    unique_ptr<Expr> expr;
};

class Response {
public:
    Response(HTTPResponseCode code, unique_ptr<Expr> expr)
        : code(code), expr(move(expr)) {}
    
    void accept(ASTVisitor &visitor) const { visitor.visit(*this); }
    
    HTTPResponseCode code;
    unique_ptr<Expr> expr;
};

class APIcall {
public:
    APIcall(unique_ptr<FuncCall> call, Response response)
        : call(move(call)), response(move(response)) {}
    
    void accept(ASTVisitor &visitor) const { visitor.visit(*this); }
    
    unique_ptr<FuncCall> call;
    Response response;
};

class API {
public:
    API(unique_ptr<Expr> pre, unique_ptr<APIcall> call, Response response)
        : pre(move(pre)), call(move(call)), response(move(response)) {}
    
    void accept(ASTVisitor &visitor) const { visitor.visit(*this); }
    
    unique_ptr<Expr> pre;
    unique_ptr<APIcall> call;
    Response response;
};

class Spec {
public:
    Spec(vector<unique_ptr<Decl>> globals,
         vector<unique_ptr<Init>> init,
         vector<unique_ptr<FuncDecl>> functions,
         vector<unique_ptr<API>> blocks)
        : globals(move(globals)), init(move(init)), 
          functions(move(functions)), blocks(move(blocks)) {}
    
    void accept(ASTVisitor &visitor) const { visitor.visit(*this); }
    
    vector<unique_ptr<Decl>> globals;
    vector<unique_ptr<Init>> init;
    vector<unique_ptr<FuncDecl>> functions;
    vector<unique_ptr<API>> blocks;
};

class Stmt {
public:
    virtual ~Stmt() = default;
    virtual void accept(ASTVisitor &visitor) const = 0;
    StatementType statementType;

protected:
    explicit Stmt(StatementType type) : statementType(type) {}
};

class Assign : public Stmt {
public:
    Assign(unique_ptr<Var> left, unique_ptr<Expr> right)
        : Stmt(ASSIGN), left(move(left)), right(move(right)) {}
    
    void accept(ASTVisitor &visitor) const override { visitor.visit(*this); }
    
    unique_ptr<Var> left;
    unique_ptr<Expr> right;
};

class FuncCallStmt : public Stmt {
public:
    explicit FuncCallStmt(unique_ptr<FuncCall> call)
        : Stmt(FUNCTIONCALL_STMT), call(move(call)) {}
    
    void accept(ASTVisitor &visitor) const override { visitor.visit(*this); }
    
    unique_ptr<FuncCall> call;
};

class Program {
public:
    explicit Program(vector<unique_ptr<Stmt>> statements)
        : statements(move(statements)) {}
    
    void accept(ASTVisitor &visitor) const { visitor.visit(*this); }
    
    vector<unique_ptr<Stmt>> statements;
};

#endif // AST_HH
>>>>>>> master
