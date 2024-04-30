#pragma once
#ifndef TOKEN_H
#define TOKEN_H

#include <optional>
#include <variant>
#include <memory>

struct SetElementNode;
struct ExpressionNode;
struct MappingNode;

enum class TokenType{
    NewLine, //TODO: implement new line token for better error messages
    SetElement,
    GroupIdentity,
    CompositionOp,
    MappingOp,
    Eof,
    Define,
    MapCall,
    VariableMap
};

struct Token{
    TokenType type;
    std::optional<std::string> value;
    Token(TokenType t, std::optional<std::string> v) : type(t), value(std::move(v)) {};
};

#endif //TOKEN_H

