#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <gtest/gtest.h>



#include "../include/file_handler.hh"
#include "../include/lexer.hh"
#include "../include/parser.hh"
#include "../include/AST.hh"
#include "../include/error.hh"
#include "../include/asm_gen.hh"

int main(int argc, char** argv){

    //if argument is passed, (file_name) read the file into memory.
    std::cout<<argv[1]<<std::endl;


    if(argv[1] && argc > 0){

        testing::InitGoogleTest(&argc, argv);
        //return RUN_ALL_TESTS();
        std::string file_name = argv[1];

        if(!file_name.empty()){
            std::string file_contents = FileHandler::read_file(file_name);
            //file_contents = file_name;

            //lexing/parsing phase
            Lexer lexer;
            Parser parser;
            lexer.lex_input(file_contents);
            lexer.set_lines_lexed(0);
            lexer.append_eof();
#ifdef debug
            lexer.print();
#endif


            AST ast;
            std::vector<std::shared_ptr<ExpressionNode>> expression_roots;
            for(const auto& token_vector : lexer.get_tokens()){
                auto root_ptr = parser.parse_program(token_vector);
                expression_roots.push_back(root_ptr);
            }

            for(const auto& root_ptrs : expression_roots){
                if(root_ptrs){
                    ast.visit(root_ptrs);
                    std::cout<< " " << std::endl;
                } else {
                    std::cerr<<"Failed to parse program.\n";
                }
            }

            /*
            Generator asm_gen;
            AST ast;

            if(auto root_ptr = parser.parse_program()) {

                ast.visit(root_ptr);
                asm_gen.generate(root_ptr);
                //asm_gen.print();
            } else {
                std::cout << "Failed to parse program.\n";
            }
             */
        }
    }

    //flush stream and exit;
    if(argc <= 1){

        std::cerr<<"No file provided!\nUsage: ./cyclic [program.cy]"<<std::endl;
        return 1;
    }
}
