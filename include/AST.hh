#pragma once

#ifndef AST_H
#define AST_H

#include <vector>
#include <memory>

#include "token.hh"
#include "node.hh"
#include "asm_gen.hh"

class AST : public Visitor {

private:

public:

    AST() : Visitor() {};

    void visit(GroupIdentityNode& node) override {
        std::cout << "GroupIdentityNode: " << node.GroupIdentity.value.value() << std::endl;
    }

    void visit(MappingNode& node) override {
        std::cout << "MappingNode" << std::endl;
        std::cout<< "Mapping node's rhs: ";
        node.rhs->accept(*this);
        std::cout<< "Mapping node's lhs: ";
        node.lhs->accept(*this);
    }

    void visit(SetElementNode& node) override {
        std::cout << "SetElementNode: " << node.token.value.value_or("no_val") << std::endl;
    }

    void visit(CompositionNode& node) override {
        std::cout << "CompositionNode" << std::endl;
        std::cout << "Comp node's lhs: ";
        if(node.lhs != nullptr){
            node.lhs->accept(*this);
        } else {
            std::cout << "nullptr" << std::endl;
        }
        std::cout << "Comp node's rhs: ";
        if(node.rhs != nullptr){
            node.rhs->accept(*this);
        } else {
            std::cout << "nullptr" << std::endl;
        }
    }

    void visit(MapCallNode& node) override {
        std::cout << "MapCallNode: " << node.token.value.value() << std::endl;
    }

    void visit(DefineNode& node) override {
        std::cout << "DefineNode: " << node.GroupIdentity.value.value() << std::endl;

        /*
        std::cout << "DefineNode" << std::endl;
        std::cout << "Define node's lhs: ";
        if(node.lhs != nullptr){
            node.lhs->accept(*this);
        } else {
            std::cout << "nullptr" << std::endl;
        }
        std::cout << "Define node's rhs: ";
        if(node.rhs != nullptr){
            node.rhs->accept(*this);
        } else {
            std::cout << "nullptr" << std::endl;
        }
         */
    }

    void visit(const std::shared_ptr<ExpressionNode>& node) {
        node->accept(*this);
    }

    void print_tree(const std::shared_ptr<ExpressionNode>& node) {
        node->accept(*this);
    }

    void traverse_and_print(const std::shared_ptr<ExpressionNode>& node, int depth);
};

#endif //AST_H
