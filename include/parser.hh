#pragma once

#ifndef PARSER_H
#define PARSER_H
#include "lexer.hh"
#include "AST.hh"
#include "error.hh"
#include <optional>
#include <variant>

class Parser{

private:

    std::vector<Token> tokens;
    std::shared_ptr<MappingNode> root;
    Error error;

public:

    explicit Parser(){ this->error = Error(this->tokens); tokens.reserve(256);};

    std::shared_ptr<ExpressionNode> parse_program(const std::vector<Token>& tokens);

    void rec_tokens(std::vector<Token> rec_tokens){
        this->tokens = std::move(rec_tokens);
    }

    void consume(const uint32_t &amt);
    //overload consume to consume one token at a time.
    void consume();
    Token peek();
    Token peek(const uint32_t &amt);
};
#endif //PARSER_H