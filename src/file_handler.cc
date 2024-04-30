#include <fstream>
#include "../include/file_handler.hh"
#include "../include/asm_gen.hh"

//read file into memory.
[[nodiscard]] std::string FileHandler::read_file(const std::string& file_name){

    //open file
    std::ifstream input_file(file_name);

    //set position file start to file end;
    input_file.seekg(0, std::ios::end);

    //obtain file size;
    size_t size = input_file.tellg();

    //allocate room and fill with ' ' chars.
    std::string buffer(size, ' ');

    //set position to file start;
    input_file.seekg(0);

    //read buffer of size "size" given address to start of buffer.
    input_file.read(&buffer[0], size);
    return buffer;
}


void FileHandler::write_file(const Generator& generator){

    std::ofstream output_file;
    output_file.open("a.asm");
    output_file << generator.get_asm();

}
