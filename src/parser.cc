#include "../include/parser.hh"

//consume a certain amount of tokens from the token vector
void Parser::consume(const uint32_t &amt){
    this->tokens.erase(tokens.begin(), tokens.begin() + amt);
}

//overload consume to consume one token at a time.
void Parser::consume(){
    this->tokens.erase(tokens.begin(), tokens.begin() + 1);
}

Token Parser::peek(){
    return tokens.size() > 1 ? tokens[1] : Token(TokenType::Eof, "EOF");
}

Token Parser::peek(const uint32_t& amt){
    return tokens.size() > 1 ? tokens[amt] : Token(TokenType::Eof, "EOF");
}

//TODO: PRODUCE MEANINGFUL ERROR MESSAGES WITH RESPECT TO SYNTAX ERRORS
std::shared_ptr<ExpressionNode> Parser::parse_program(const std::vector<Token>& t) {

    this->tokens = t;

    if (tokens.empty()) {
        return nullptr;
    }

    for(uint32_t i = 0; i < tokens.size(); i++) {

        switch (tokens[i].type) {

            case TokenType::Define:
                if (peek().type == TokenType::MappingOp) {
                    auto node = std::make_shared<MappingNode>(MappingNode{});
                    node->lhs = std::make_shared<DefineNode>(DefineNode{tokens[0]});
                    consume(2); //consume two tokens
                    node->rhs = parse_program(tokens);
                    return node;
                } else {
                    std::cerr << "cyclic ~ Compilation error: Expected MappingOp after Define" << std::endl;
                    exit(1);
                }

            case TokenType::GroupIdentity:
                if (peek().type == TokenType::MappingOp) {
                    auto node = std::make_shared<MappingNode>(MappingNode{});
                    node->lhs = std::make_shared<GroupIdentityNode>(GroupIdentityNode{tokens[0]});
                    consume(2); //consume two tokens
                    node->rhs = parse_program(tokens);
                    return node;
                } else if(peek().type == TokenType::CompositionOp){
                    auto node = std::make_shared<CompositionNode>();
                    node->lhs = std::make_shared<GroupIdentityNode>(GroupIdentityNode{tokens[0]});
                    consume(2); //consume two tokens
                    node->rhs = parse_program(tokens);
                    return node;
                }else{
                    std::cerr << "cyclic ~ Compilation error: Expected MappingOp after GroupIdentity" << std::endl;
                    exit(1);
                }

            case TokenType::SetElement:

                if(tokens.size() == 2 && tokens[1].type == TokenType::Eof){
                    return std::make_shared<SetElementNode>(SetElementNode{tokens[0]});
                }

                if (peek().type == TokenType::CompositionOp && tokens[2].value.value_or("0")[3] == ')') {
                    std::cout<<"Found nested expression"<<std::endl;
                    auto node = std::make_shared<CompositionNode>();
                    node->lhs = std::make_shared<SetElementNode>(tokens[0]);
                    consume(2);
                    node->rhs = std::make_shared<SetElementNode>(tokens[0]);
                    return node;
                }

                if (peek().type == TokenType::CompositionOp) {
                    if(peek(2).type == TokenType::SetElement || peek(2).type == TokenType::MapCall){
                        auto node = std::make_shared<CompositionNode>();
                        node->lhs = std::make_shared<SetElementNode>(tokens[0]);
                        consume(2);
                        node->rhs = parse_program(tokens);
                        return node;
                    } else if (peek(2).type == TokenType::GroupIdentity) {
                        //we have encountered a nested expression;
                        auto node = std::make_shared<CompositionNode>();
                        node->lhs = std::make_shared<SetElementNode>(tokens[0]);
                        consume(2);
                        node->rhs = parse_program(tokens);
                        return node;
                    }
                }
                return std::make_shared<SetElementNode>(SetElementNode{tokens[0]});

            case TokenType::Eof:
                return nullptr;

            case TokenType::NewLine:
                error.new_line_counter++;
                break;


            case TokenType::CompositionOp:
                if(peek().type == TokenType::Eof){
                    std::cerr << "cyclic ~ Compilation error: Expected non-null expression after CompositionOp" << std::endl;
                    exit(1);
                }

            case TokenType::MapCall:
                return std::make_shared<MapCallNode>(MapCallNode{tokens[0]});

            default:
                std::cerr << "cyclic ~ Compilation error: No valid token found in input." << std::endl;
                return nullptr;
        }
    }
    return nullptr;
}