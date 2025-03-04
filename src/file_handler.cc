#include <fstream>
#include "../include/file_handler.hh"
#include "../include/asm_gen.hh"


[[nodiscard]] std::string FileHandler::read_file(const std::string& file_name) {
    //open file
    std::ifstream input_file(file_name, std::ios::binary);
    if (!input_file.is_open()) {
        throw std::runtime_error("Could not open file --ensure that file is in directory of executable.: " + file_name);
    }

    //set position to file end to determine file size
    input_file.seekg(0, std::ios::end);
    std::streampos size = input_file.tellg();
    if (size <= 0) {
        throw std::runtime_error("File is empty or error in getting file size: " + file_name);
    }

    //allocate buffer and fill with ' ' chars
    std::string buffer(static_cast<size_t>(size), ' ');

    //set position to file start
    input_file.seekg(0);

    //read file into buffer
    input_file.read(&buffer[0], size);
    if (!input_file) {
        throw std::runtime_error("Error reading file: " + file_name);
    }

    return buffer;
}


void FileHandler::write_file(const Generator& generator){

    std::ofstream output_file;
    output_file.open("a.asm");
    output_file << generator.get_asm();

}
