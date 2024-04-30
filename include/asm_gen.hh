#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <sstream>
#include <random>
#include "../include/AST.hh"
#include "../include/token.hh"
#include "../include/lexer.hh"

class Generator : public Visitor{

private:

    uint64_t reg_count = 0;

    std::unordered_map<std::string, bool> register_usage = {
        {"r8", false},
        {"r9", false},
        {"r10", false},
        {"r11", false},
        {"r12", false},
        {"r13", false},
        {"r14", false},
        {"r15", false},
        {"rax", false},
        {"rbx", false},
        {"rcx", false},
        {"rdx", false},
        {"rsi", false},
        {"rdi", false},
        {"rsp", false},
        {"rbp", false}
    };

    std::string register_name;
    std::string asm_setup = "default rel\n"
                            "extern printf, exit\n"
                            "\n"
                            "section .data\n"
                            "    format_int db \"%i\", 10, 0\n";
                            //data section where further variables will be declared.
    std::string asm_data =
                            "\n"
                            "section .text\n"               //text section
                            "    global main\n"             //define global entry point
                            "\n"
                            "main:\n"                       //main entry point
                            "\n"
                            "    sub rsp, 8\n";             //align stack pointer

    std::string asm_end =   "    lea rdi, [rel format_int]\n"
                            "\n"
                            "    xor eax, eax\n"
                            "    call printf\n"
                            "\n"
                            "    xor eax, eax\n"
                            "    add rsp, 8\n"
                            "    ret\n";
    std::stringstream asm_stream;

public:

    Generator(){
        register_usage["rsi"] = true;
    }

    void print(){
        std::cout<< this->asm_setup + asm_stream.str() + this->asm_end <<std::endl;
    }

    [[nodiscard]] std::string get_asm() const {
        return this->asm_stream.str();
    };

    [[nodiscard]] std::string unused_register();

    void print_inuse_reg();

    std::string generate_var_name(){
        //TODO: check if register is in use
        this->reg_count++;

        return "var" + std::to_string(reg_count);
    }

    void visit(GroupIdentityNode& node) override {
        asm_stream << '\t' << generate_var_name() << " dd" << 0;
        //TODO: implement code generation for group identity nodes
    }

    void visit(MappingNode& node) override {
        this->register_name = "rsi";
        this->asm_stream << "mov " << register_name << ", ";
        node.rhs->accept(*this);
        this->asm_stream << '\n';
    }

    void visit(SetElementNode& node) override {

        std::string value;

        for(const auto& character : node.token.value.value_or("0")){
            if(std::isdigit(character)) value = character;
        }

        this->asm_stream << value;
    }

    void visit(DefineNode& node) override {
        //TODO: implement code generation for define nodes
    }

    void visit(MapCallNode& node) override {
        //TODO: implement code generation for map call nodes
    }

    void visit(CompositionNode& node) override {
        if (node.lhs->getNodeType() == NodeType::SetElement && node.rhs->getNodeType() == NodeType::SetElement) {
            //rhs and lhs are setelementnodes (integer literals) so the arithmetic can be computed at compile time.
            auto lhs = static_cast<SetElementNode*>(node.lhs.get());
            auto rhs = static_cast<SetElementNode*>(node.rhs.get());
            int sum = std::stoi(lhs->token.value.value_or("0")) + std::stoi(rhs->token.value.value_or("0"));
            asm_stream << sum;
        } else {
            //generate code without compile-time computation
            node.lhs->accept(*this);
            asm_stream << ", ";
            node.rhs->accept(*this);
        }
    }

    void generate(std::shared_ptr<ExpressionNode> node) {
        node->accept(*this);
    }

    //TODO: implement code generation for declaration of reserved memory locations, variables, in assembly
    void declare_variable(const std::string &name, int value);
    void load_variable(const std::string &name, const std::string &register_name);
};
