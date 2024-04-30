#pragma once
#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <utility>

class CompositionNode;
class SetElementNode;
class MappingNode;
class GroupIdentityNode;
class AST;
class DefineNode;
class MapCallNode;

enum class NodeType {
    GroupIdentity,
    Mapping,
    SetElement,
    Composition
};

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(GroupIdentityNode& node) = 0;
    virtual void visit(MappingNode& node) = 0;
    virtual void visit(SetElementNode& node) = 0;
    virtual void visit(CompositionNode& node) = 0;
    virtual void visit(DefineNode& node) = 0;
    virtual void visit(MapCallNode& node) = 0;
};

class ExpressionNode {
public:
    virtual ~ExpressionNode() = default;
    virtual void accept(Visitor& visitor) = 0;
    [[nodiscard]] virtual NodeType getNodeType() const = 0;

};

class GroupIdentityNode : public ExpressionNode {
public:
    Token GroupIdentity;
    explicit GroupIdentityNode(Token t) : GroupIdentity(std::move(t)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }

    [[nodiscard]] NodeType getNodeType() const override {
        return NodeType::GroupIdentity;
    }
};

class MappingNode : public ExpressionNode {
public:
    std::shared_ptr<GroupIdentityNode> lhs;
    std::shared_ptr<ExpressionNode> rhs;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }

    [[nodiscard]] NodeType getNodeType() const override {
        return NodeType::Mapping;
    }
};

class DefineNode : public GroupIdentityNode {

public:
    //Token Define;
    explicit DefineNode(Token t) : GroupIdentityNode(std::move(t)) {}
    std::shared_ptr<GroupIdentityNode> lhs;
    std::shared_ptr<ExpressionNode> rhs;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }

    [[nodiscard]] NodeType getNodeType() const override {
        return NodeType::Mapping;
    }

};

class SetElementNode : public ExpressionNode {
public:
    Token token;
    explicit SetElementNode(Token t) : token(std::move(t)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }

    [[nodiscard]] NodeType getNodeType() const override {
        return NodeType::SetElement;
    }
};

class MapCallNode : public ExpressionNode {
public:
    Token token;
    explicit MapCallNode(Token t) : token(std::move(t)) {}

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }

    [[nodiscard]] NodeType getNodeType() const override {
        return NodeType::SetElement;
    }
};

class CompositionNode : public ExpressionNode {
public:
    std::shared_ptr<ExpressionNode> lhs;
    std::shared_ptr<ExpressionNode> rhs;

    void accept(Visitor& visitor) override {
        visitor.visit(*this);
    }

    [[nodiscard]] NodeType getNodeType() const override {
        return NodeType::Composition;
    }
};


#endif //NODE_H
