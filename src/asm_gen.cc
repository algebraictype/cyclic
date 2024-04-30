// Code generation for the AST
#include "../include/asm_gen.hh"
#include "../include/error.hh"
#include <string>


//TODO: PRINT IN-USE REGISTERS
[[nodiscard]] std::string Generator::unused_register() {

    for (const auto& reg : this->register_usage) {
        if (!reg.second) {
            this->register_usage[reg.first] = (true); //register is now in use
            return reg.first;
        }
    }
    throw std::runtime_error("No unused registers available");
    //TODO: place the value somewhere else in the assembly program.
    //TODO: maybe in the data section? can something be displaced to allow for this?
    //debugln("No available registers");
    exit(1);
}

void Generator::declare_variable(const std::string& name, int value) {
    this->asm_stream << name << " db " << value << '\n';
}

void Generator::load_variable(const std::string& name, const std::string& register_name) {
    this->asm_stream << "mov " << register_name << ", [" << name << "]\n";
}

void Generator::print_inuse_reg(){
    for(const auto& reg : this->register_usage){
        if(reg.second == 1){
            std::cout<<reg.first<<std::endl;
        }
    }
}
