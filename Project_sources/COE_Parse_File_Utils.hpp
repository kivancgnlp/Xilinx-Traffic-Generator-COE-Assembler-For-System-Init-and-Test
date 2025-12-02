//
//  COE_File_Utils.hpp
//  Xilinx ATG Coe Parser
//
//  Created by Kivanc Gunalp on 22.01.2025.
//

#ifndef COE_File_Utils_hpp
#define COE_File_Utils_hpp

#include <fstream>
#include <iostream>
#include <string>
#include <map>

#include "COE Common.hpp"

class COE_Parse_File_Utils {
    std::ifstream control_coe_file;
    std::ifstream adr_coe_file;
    std::ifstream data_coe_file;
    std::ifstream mask_coe_file;
    std::ifstream known_adrs_file;
    
    std::string control_file_line;
    std::string adr_file_line;
    std::string data_file_line;
    std::string mask_file_line;
    
    std::map<unsigned,std::string> m_know_adr_map;

    bool m_also_produce_hex_based_files{};
    bool m_hex_input_mode{};

    std::ofstream control_coe_out_file;
    std::ofstream adr_coe_out_file;
    std::ofstream data_coe_out_file;
    std::ofstream mask_coe_out_file;

    unsigned row_number{};
    
public:
    COE_Parse_File_Utils(const std::string& base_path, bool also_produce_hex_based_files, bool hex_input_mode);
    
    std::string lookup_adr(unsigned adr);

    void load_known_adr_map(const std::string& file_path);
    
    Instruction find_next_non_comment_line();
    
    void read_next_line();

    void skip_header();
    
    bool all_files_ok();

};

#endif /* COE_File_Utils_hpp */
