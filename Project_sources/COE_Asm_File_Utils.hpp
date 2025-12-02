#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "COE Common.hpp"

class COE_Asm_File_Utils{
    std::ofstream control_coe_file;
    std::ofstream adr_coe_file;
    std::ofstream data_coe_file;
    std::ofstream mask_coe_file;

    std::ifstream src_assembly_file;

    std::string m_line_str;
    unsigned m_current_row_index{};

public:
     

    Instruction parse_current_line();

    bool find_next_non_comment_line();

    bool open_src_assembly_file(const std::string& in_asm_file_path);

    bool open_output_coe_files(const std::string& base_path);

    void prepare_header();

    void write_instruction_to_files(const Instruction& ins, bool last_instruction = false);

    static std::string generate_output_files_base_path(const char* path);



};

