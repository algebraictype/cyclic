#include <vector>
#include <sstream>
#include <gtest/gtest.h>

#include "../include/lexer.hh"

//(currently) arbitrary limit to the depth of expressions.
constexpr int MAX_EXPRESSION_DEPTH = 1024;


void check_parenthesis_balance(const uint64_t& left_parenthesis, const uint64_t right_parenthesis){

    std::cout<<right_parenthesis<<" "<<left_parenthesis<<std::endl;
        if(right_parenthesis > left_parenthesis){
            std::cerr<<"Expected: '('\n";
            std::exit(EXIT_FAILURE);
        } else if (right_parenthesis < left_parenthesis){
            std::cerr<<"Expected: ')'\n";
            std::exit(EXIT_FAILURE);
        }
}

inline bool Lexer::push_expression(){
    if(this->expression_depth < MAX_EXPRESSION_DEPTH){
        //push an expression s.t lexing inner input_expressions can be done.
        std::vector<std::string> expression;
        this->input_expressions.emplace_back(expression);
        // Also add a new std::vector<Token> to tokens
        this->expression_depth += 1;
        return true;
    } else {
        return false;
    }
}

inline bool Lexer::add_sequential_expression(){
    if(this->expression_depth < MAX_EXPRESSION_DEPTH){
        //push an expression s.t lexing inner input_expressions can be done.
        std::vector<std::string> expression;
        this->input_expressions.emplace_back(expression);
        // Also add a new std::vector<Token> to tokens
        this->tokens.emplace_back(std::vector<Token>{});
        this->lines += 1;
        return true;
    } else {
        return false;
    }
}

inline void Lexer::pop_expression(){
    //simple ternary assignment to avoid popping when expression depth is already 0.
    (this->expression_depth > 0) ? this->expression_depth -= 1 : this->expression_depth = 0;
}

void Lexer::give_tokens(Parser& parser){
    //parser.rec_tokens(std::move(this->tokens));
}

void Lexer::lex_input(std::string &raw_input){

    static std::string accumulated_string;

    //size_t denoting number of right, and left, parentheses in lex expression.
    static size_t   right_parenthesis = 0;
    static size_t   left_parenthesis = 0;
    static bool     decrement_expr = false;

    for(uint64_t i = 0; i < raw_input.length(); i++){
        //.at() instead of [] because of implicit bounds checking.
        char character = raw_input.at(i);

        if(character == '('){
            left_parenthesis += 1;

            if(accumulated_string.empty()){
                std::string nested_expression = raw_input.substr(i+1);
                lex_nested_expression(nested_expression);
                break;
            }
        
        }

        #ifdef verbose_debug
            std::cout<< "is_space(acum): " << accumulated_string << "  ::depth: "<< this->expression_depth << std::endl;
        #endif

        if(character == ')'){
                right_parenthesis += 1;

                if(this->expression_depth > 0){
                    decrement_expr = true;
                }
        }

        if(character == '/'){
            while(raw_input.at(i) != '\n'){
                i++;
            }
        continue;
        }

        if(character == '\n' && !(accumulated_string.empty()) && i != raw_input.length()-1){
            this->input_expressions[this->expression_depth].emplace_back(accumulated_string);
            accumulated_string.clear();
            //push_expression();
            add_sequential_expression();
            continue;
        }

        if(!isspace(character)){
            accumulated_string += character;
            if(i != raw_input.length()-1) continue;
        }

        //not pushing 'nothing' into vector
        if(!accumulated_string.empty()){

            #ifdef verbose_debug
                std::cout<< "**PUSHED**(acum): " << accumulated_string << "  ::depth: "<<
                this->expression_depth << std::endl;
            #endif

            lexeme_to_token(accumulated_string);
            //push accumulated lexeme into the vector.
            this->input_expressions[this->expression_depth].emplace_back(accumulated_string);
            //decrement the depth IF decrementing is to be done.
            if(decrement_expr) this->expression_depth > 0 ? this->expression_depth -= 1 : this->expression_depth = 0;

            accumulated_string.clear();
            decrement_expr = false;
            continue;
        }
    }
    //unbalanced parentheses
    check_parenthesis_balance(left_parenthesis, right_parenthesis);
}


void Lexer::lexeme_to_token(const std::string& buf){

    //if token[0] is a letter and token[1] is an exponent operator, and token[2] is a number, then it's a set element.
    if(std::isalpha(buf[0]) && buf[1] == '^' && std::isdigit(buf[2])){
        tokens[expression_depth].emplace_back( Token( TokenType::SetElement, buf) );
        return;
    }

    //this will scan for a lexeme that is a group identity, which is essentially a alias for a constant expression.
    if(std::isalpha(buf[0]) && buf[1] == '^' && std::isalpha(buf[2])){
        tokens[this->lines].emplace_back( Token (TokenType::GroupIdentity, buf ));
        return;
    }

    //define a mapping entry definition ([a..z]_[a..z])
    if(std::isalpha(buf[0]) && buf[1] == '_' && std::isalpha(buf[2])){
        tokens[this->lines].emplace_back( Token (TokenType::Define, buf) );
        return;
    }

    //define a mapping call ([a..z]_[0..9])
    if(std::isalpha(buf[0]) && buf[1] == '_' && std::isdigit(buf[2])){
        tokens[this->lines].emplace_back( Token (TokenType::MapCall, buf) );
        return;
    }

    //if token is the mapping operator, push it into the tokens vector.
    if(buf == "|->"){
        tokens[this->lines].emplace_back( Token( TokenType::MappingOp, std::nullopt) );
        return;
    }

    //if token is the composition operator, push it into the tokens vector.
    if(buf == "*"){
        tokens[this->lines].emplace_back( Token( TokenType::CompositionOp, std::nullopt) );
    }

}

inline void Lexer::lex_nested_expression(std::string& nested_expression){

    if(push_expression()){
#ifdef verbose_debug
        std::cout<<nested_expression<<std::endl;
#endif
        lex_input(nested_expression);
        pop_expression();
    } else {
        std::cerr << "Maximum expressions exceeded (1024)\n";
        std::exit(EXIT_FAILURE);
    }
}

void Lexer::append_eof(){
    tokens[lines].emplace_back( TokenType::Eof, "END_OF_FILE" );
}

void Lexer::print(){

    //print lexemes
    for(uint64_t i = 0; i < this->input_expressions.size(); i++){

        //iterates over tokens in the input_expressions
        for(uint64_t j = 0; j < this->input_expressions[i].size(); j++){

            //debug output;
            std::cout << "Expression Vector ["<< i << "] : " << this->input_expressions[i][j] << " || Number of tokens: [" <<
            this->input_expressions[i].size() << "] \n";

        }
    
    std::cout<<'\n';
    }

    //print tokens
    for(const auto& t : this->tokens){
        for(const auto& token : t){
            std::cout<< static_cast<int>(token.type)<< " " << token.value.value_or("0") << '\n';
        }
    }
    std::cout<<'\n';
}
