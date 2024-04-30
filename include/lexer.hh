#pragma once

#ifndef LEXER_H
#define LEXER_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstdint>

#include "token.hh"
#include "parser.hh"

class Parser;

class Lexer{
private:

    //for debugging purposes
    std::vector<std::vector<std::string>> input_expressions;
    uint32_t lines = 0;

    //vector of tokens that will be used to build the AST.
    std::vector<std::vector<Token>> tokens;

    //denotes at which level of nesting we are at in a given expression.
    uint32_t expression_depth;

public:
    //constructor that will create an "initial" expression and push it
    //into the expressions vector.
    Lexer() {
        std::vector<std::string> first_expr;
        this->input_expressions.push_back(first_expr);
        this->tokens.push_back(std::vector<Token>());
        this->expression_depth = 0;
    }

    ~Lexer() = default;
    //dec/inc the expression depth.
    inline void pop_expression();

    inline bool push_expression();

    //simple helper function to lex nested expressions.
    inline void lex_nested_expression(std::string& nested_expression);
    //tokenize that will convert the accumulated string into a token.
    inline void lexeme_to_token(const std::string& accumulated_string);

    void lex_input(std::string& raw_input);

    void set_lines_lexed(uint32_t depth){
        this->lines = depth;
    }

    //transfer tokens to the parser.
    void give_tokens(Parser& parser);

    [[nodiscard]] uint32_t get_expression_depth() const { return this->expression_depth; }


    //debug methods
    [[maybe_unused]] [[nodiscard]] inline std::vector<std::vector<std::string>> get_input_expressions() { return this->input_expressions; };
    [[maybe_unused]] [[nodiscard]] inline std::vector<std::vector<Token>> get_tokens(){ return this->tokens;};
    void print();

    void append_eof();

    bool add_sequential_expression();
};
#endif //LEXER_H
