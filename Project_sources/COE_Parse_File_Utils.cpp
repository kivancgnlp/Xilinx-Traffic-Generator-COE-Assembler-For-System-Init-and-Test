//
//  COE_File_Utils.cpp
//  Xilinx ATG Coe Parser
//
//  Created by Kivanc Gunalp on 22.01.2025.
//

#include "COE_Parse_File_Utils.hpp"

bool COE_Parse_File_Utils::all_files_ok() {

    return control_coe_file && adr_coe_file && data_coe_file && mask_coe_file;

}

void COE_Parse_File_Utils::skip_header() {

    read_next_line();

    while (std::isxdigit(control_file_line.at(0)) == 0) { // search for line begining with 0
        std::getline(control_coe_file, control_file_line);
        std::getline(adr_coe_file, adr_file_line);
        std::getline(data_coe_file, data_file_line);
        std::getline(mask_coe_file, mask_file_line);
    }
}

void COE_Parse_File_Utils::read_next_line() {

    std::getline(control_coe_file, control_file_line);
    std::getline(adr_coe_file, adr_file_line);
    std::getline(data_coe_file, data_file_line);
    std::getline(mask_coe_file, mask_file_line);
}

Instruction COE_Parse_File_Utils::find_next_non_comment_line() {

    //std::cout << "Line : " << control_file_line << "\n";


    unsigned control_value = (unsigned)std::stoul(control_file_line, nullptr, m_hex_input_mode ? 16 : 2);
    unsigned adr_value = (unsigned)std::stoul(adr_file_line, nullptr, m_hex_input_mode ? 16 : 2);
    unsigned data_value = (unsigned)std::stoul(data_file_line, nullptr, m_hex_input_mode ? 16 : 2);
    unsigned mask_value = (unsigned)std::stoul(mask_file_line, nullptr, m_hex_input_mode ? 16 : 2);

    if (m_also_produce_hex_based_files) {
        if (adr_value != 0xffffffff) {
            control_coe_out_file << control_value << ",\n";
            adr_coe_out_file << adr_value << ",\n";
            data_coe_out_file << data_value << ",\n";
            mask_coe_out_file << mask_value << ",\n";
        } else {
            control_coe_out_file << control_value << ";";
            adr_coe_out_file << adr_value << ";";
            data_coe_out_file << data_value << ";";
            mask_coe_out_file << mask_value << ";";
        }

    }

    Instruction ins{};

    ins.next_fail_adr = control_value & 0xff;
    control_value >>= 8;
    ins.next_success_adr = control_value & 0xff;
    control_value >>= 8;

    ins.write_transaction = control_value & 1;
    control_value >>= 1;
    ins.count_as_error = control_value & 1;
    control_value >>= 1;

    //std::cout << "unknow field at control value : " << std::hex << control_value << std::dec << "\n";

    ins.adres = adr_value;
    ins.data = data_value;
    ins.data_mask = mask_value;
    ins.row_index = row_number;

    row_number++;
    read_next_line();
    return ins;
}

void COE_Parse_File_Utils::load_known_adr_map(const std::string& file_path) {
    std::ifstream adr_map_file{ file_path };

    if (not adr_map_file) {
        std::cerr << "Known adr file open error\n";
        return;
    }

    std::string line;
    std::string::size_type end_pos{};
    unsigned adr_val{};
    std::string desc;

    while (std::getline(adr_map_file, line)) {
        adr_val = (unsigned)std::stoul(line, &end_pos, 16);
        desc = line.substr(end_pos + 1);
        m_know_adr_map[adr_val] = desc;
    }
}

std::string COE_Parse_File_Utils::lookup_adr(unsigned adr) {

    if (m_know_adr_map.count(adr) == 1) {
        return m_know_adr_map[adr];
    } else {
        return "?";
    }

}

COE_Parse_File_Utils::COE_Parse_File_Utils(const std::string& base_path, bool also_produce_hex_based_files, bool hex_input_mode) :
    control_coe_file{ base_path + "_ctrl.coe" },
    adr_coe_file{ base_path + "_addr.coe" },
    data_coe_file{ base_path + "_data.coe" },
    mask_coe_file{ base_path + "_mask.coe" }
{

    m_also_produce_hex_based_files = also_produce_hex_based_files;
    m_hex_input_mode = hex_input_mode;

    if (also_produce_hex_based_files) {
        control_coe_out_file.open(base_path + "_ctrl_16.coe");
        adr_coe_out_file.open(base_path + "_addr_16.coe");
        data_coe_out_file.open(base_path + "_data_16.coe");
        mask_coe_out_file.open(base_path + "_mask_16.coe");

        control_coe_out_file << "memory_initialization_radix = 16;\nmemory_initialization_vector = \n";
        adr_coe_out_file << "memory_initialization_radix = 16;\nmemory_initialization_vector = \n";
        data_coe_out_file << "memory_initialization_radix = 16;\nmemory_initialization_vector = \n";
        mask_coe_out_file << "memory_initialization_radix = 16;\nmemory_initialization_vector = \n";

        control_coe_out_file << std::hex;
        adr_coe_out_file << std::hex;
        data_coe_out_file << std::hex;
        mask_coe_out_file << std::hex;
    }


    // ctor
}