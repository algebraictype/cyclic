#pragma once

#ifndef ERROR_HH
#define ERROR_HH

#include <cstdint>
#include <iostream>
#include <vector>
#include "token.hh"


template<typename T, typename... Args>
void debugln(T first, Args... args) {
    std::cout << first << '\n';
    debugln(args...);
};
class Error {

private:
    uint32_t m_index = 0;
    std::vector<Token> tokens;
public:
    explicit Error(std::vector<Token>& tokens) : tokens(tokens) { this->m_index = tokens.size(); };
    uint32_t new_line_counter = 1;

    Error() = default;

        void set_m_index(const uint32_t& num_tokens) {
            this->m_index = num_tokens;
        }

        void inc_m_index(const uint32_t& inc){
            this->m_index += inc;
        }

        [[nodiscard]] uint32_t get_m_index() const {
            return m_index;
        }

        void print_err_tok(){
            //TODO FIND WAY TO THROW ACCURATE ERROR THAT CORRESPONDS TO VALUE AT TOKEN (PEEK() THROWS THIS OFF)
            std::cout << "Error at token: " << this->tokens[this->m_index].value.value_or("null-valued") << std::endl;
            exit(1);
        }
};

#endif //ERROR_HH
