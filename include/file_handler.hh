#pragma once

#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
#include <fstream>
#include "asm_gen.hh"

namespace FileHandler {

    [[nodiscard]] std::string read_file(const std::string& file_name);
    void write_file(const Generator& generator);

};
#endif //FILE_HANDLER_H