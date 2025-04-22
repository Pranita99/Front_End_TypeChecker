#include "ast.hh"
#include "ASTVis.hpp"
#include <iostream>
#include <sstream>
#include <string>

// Implementation for the Print Visitor
class PrintVisitor : public ASTVisitor {
public:
    void visit(const TypeConst& node) override {
        std::cout << "TypeConst: " << node.name << std::endl;
    }
    
    void visit(const FuncType& node) override {
        std::cout << "FuncType: " << std::endl;
        std::cout << "  Parameters: " << node.params.size() << std::endl;
        for (const auto& param : node.params) {
            std::cout << "    ";
            param->accept(*this);
        }
        std::cout << "  Return Type: ";
        if (node.returnType) {
            node.returnType->accept(*this);
        } else {
            std::cout << "void" << std::endl;
        }
    }
    
    void visit(const MapType& node) override {
        std::cout << "MapType: " << std::endl;
        std::cout << "  Domain: ";
        if (node.domain) {
            node.domain->accept(*this);
        }
        std::cout << "  Range: ";
        if (node.range) {
            node.range->accept(*this);
        }
    }
    
    void visit(const TupleType& node) override {
        std::cout << "TupleType: " << node.elements.size() << " elements" << std::endl;
        for (const auto& elem : node.elements) {
            std::cout << "  ";
            elem->accept(*this);
        }
    }
    
    void visit(const SetType& node) override {
        std::cout << "SetType: " << std::endl;
        std::cout << "  Element Type: ";
        if (node.elementType) {
            node.elementType->accept(*this);
        }
    }
    
    void visit(const Decl& node) override {
        std::cout << "Declaration: " << node.name << std::endl;
        if (node.type) {
            std::cout << "  Type: ";
            node.type->accept(*this);
        }
    }
    
    void visit(const Var& node) override {
        std::cout << "Variable: " << node.name << std::endl;
    }
    
    void visit(const FuncCall& node) override {
        std::cout << "Function Call: " << node.name << std::endl;
        std::cout << "  Arguments: " << node.args.size() << std::endl;
        for (const auto& arg : node.args) {
            std::cout << "    ";
            arg->accept(*this);
        }
    }
    
    void visit(const Num& node) override {
        std::cout << "Number: " << node.value << std::endl;
    }
    
    void visit(const String& node) override {
        std::cout << "String: " << node.value << std::endl;
    }
    
    void visit(const Set& node) override {
        std::cout << "Set with " << node.elements.size() << " elements:" << std::endl;
        for (const auto& elem : node.elements) {
            std::cout << "  ";
            elem->accept(*this);
        }
    }
    
    void visit(const Map& node) override {
        std::cout << "Map with " << node.value.size() << " entries:" << std::endl;
        for (const auto& entry : node.value) {
            std::cout << "  ";
            entry.first->accept(*this);
            std::cout << " -> ";
            entry.second->accept(*this);
            std::cout << std::endl;
        }
    }
    
    void visit(const Tuple& node) override {
        std::cout << "Tuple with " << node.expr.size() << " elements:" << std::endl;
        for (const auto& elem : node.expr) {
            std::cout << "  ";
            elem->accept(*this);
        }
    }
    
    void visit(const FuncDecl& node) override {
        std::cout << "Function Declaration: " << node.name << std::endl;
        std::cout << "  Parameters: " << node.params.size() << std::endl;
        
        // Print HTTP response code
        std::cout << "  Return HTTP Code: ";
        switch(node.returnType.first) {
            case HTTPResponseCode::OK_200: 
                std::cout << "OK_200"; 
                break;
            case HTTPResponseCode::CREATED_201: 
                std::cout << "CREATED_201"; 
                break;
            case HTTPResponseCode::BAD_REQUEST_400: 
                std::cout << "BAD_REQUEST_400"; 
                break;
            default: 
                std::cout << "Unknown"; 
                break;
        }
        std::cout << std::endl;
        
        // Print return types
        std::cout << "  Return Types: " << node.returnType.second.size() << std::endl;
        for (const auto& rt : node.returnType.second) {
            std::cout << "    ";
            rt->accept(*this);
        }
    }
    
    void visit(const Init& node) override {
        std::cout << "Initialization: " << node.varName << std::endl;
        if (node.expr) {
            std::cout << "  Value: ";
            node.expr->accept(*this);
        }
    }
    
    void visit(const Response& node) override {
        std::cout << "Response:" << std::endl;
        
        // Print HTTP response code
        std::cout << "  HTTP Code: ";
        switch(node.code) {
            case HTTPResponseCode::OK_200: 
                std::cout << "OK_200"; 
                break;
            case HTTPResponseCode::CREATED_201: 
                std::cout << "CREATED_201"; 
                break;
            case HTTPResponseCode::BAD_REQUEST_400: 
                std::cout << "BAD_REQUEST_400"; 
                break;
            default: 
                std::cout << "Unknown"; 
                break;
        }
        std::cout << std::endl;
        
        // Print expression
        if (node.expr) {
            std::cout << "  Expression: ";
            node.expr->accept(*this);
        }
    }
    
    void visit(const APIcall& node) override {
        std::cout << "API Call:" << std::endl;
        if (node.call) {
            std::cout << "  Function: ";
            node.call->accept(*this);
        }
        
        std::cout << "  Response: " << std::endl;
        visit(node.response);
        // call the visit method for respponse
    }
    
    void visit(const API& node) override {
        std::cout << "API Specification:" << std::endl;
        
        std::cout << "  Precondition: ";
        if (node.pre) {
            node.pre->accept(*this);
        } else {
            std::cout << "none" << std::endl;
        }
        
        std::cout << "  API Call: ";
        if (node.call) {
            node.call->accept(*this);
        } else {
            std::cout << "none" << std::endl;
        }
        
        std::cout << "  Response: " << std::endl;
        visit(node.response); // call the visit method for response
        
    }
    
    void visit(const Spec& node) override {
        std::cout << "Specification:" << std::endl;
        
        std::cout << "  Globals (" << node.globals.size() << "):" << std::endl;
        for (const auto& global : node.globals) {
            std::cout << "    ";
            global->accept(*this);
        }
        
        std::cout << "  Initializations (" << node.init.size() << "):" << std::endl;
        for (const auto& init : node.init) {
            std::cout << "    ";
            init->accept(*this);
        }
        
        std::cout << "  Functions (" << node.functions.size() << "):" << std::endl;
        for (const auto& func : node.functions) {
            std::cout << "    ";
            func->accept(*this);
        }
        
        std::cout << "  API Blocks (" << node.blocks.size() << "):" << std::endl;
        for (const auto& block : node.blocks) {
            std::cout << "    ";
            block->accept(*this);
        }
    }
    
    void visit(const Assign& node) override {
        std::cout << "Assignment:" << std::endl;
        std::cout << "  Left: ";
        node.left->accept(*this);
        std::cout << "  Right: ";
        node.right->accept(*this);
    }
    
    void visit(const FuncCallStmt& node) override {
        std::cout << "Function Call Statement:" << std::endl;
        if (node.call) {
            node.call->accept(*this);
        }
    }
    
    void visit(const Program& node) override {
        std::cout << "Program with " << node.statements.size() << " statements:" << std::endl;
        for (const auto& stmt : node.statements) {
            stmt->accept(*this);
        }
    }
};