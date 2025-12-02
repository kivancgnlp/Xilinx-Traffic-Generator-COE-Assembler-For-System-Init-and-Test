#include "COE_Asm_File_Utils.hpp"



std::string COE_Asm_File_Utils::generate_output_files_base_path(const char* path) {

    std::string in_path_str{ path };

    std::string::size_type pos = in_path_str.find_last_of('.');

    in_path_str.erase(pos);

    return in_path_str;
}


Instruction COE_Asm_File_Utils::parse_current_line() {

    std::istringstream iss{ m_line_str };
    iss.exceptions(7);
    Instruction ins{};
    char ins_type;
    bool write_instruction;
    
    iss >> ins_type;

    ins.row_index = m_current_row_index;
    ins.next_fail_adr = m_current_row_index;
    ins.next_success_adr = m_current_row_index + 1;
    m_current_row_index++;

    if (ins_type == 'W' or ins_type == 'w') {
        write_instruction = true;
    } else if (ins_type == 'R' or ins_type == 'r') {
        write_instruction = false;
    } else {
        throw std::runtime_error{ "Unknown instruction letter" };
    }

    if (write_instruction) {
        // Write instruction
        ins.write_transaction = true;
        iss >> std::hex;

        ins.count_as_error = true;
        iss >> ins.adres;
        iss >> ins.data;
        ins.data_mask = 0xffffffff; // note : default data mask for writes is all 1
    } else {
        // Read instruction
        ins.write_transaction = false;
        iss >> std::hex;

        ins.count_as_error = false;

        iss.exceptions(0);

        iss >> ins.adres;


        iss >> ins.data;
        if (not iss) {
            ins.data = 0;
            iss.clear();
        }

        iss >> ins.data_mask;

        if (not iss) {
            ins.data_mask = 0;
            iss.clear();
        }


    }

    return ins;
}

bool COE_Asm_File_Utils::open_output_coe_files(const std::string& base_path) {

    control_coe_file.open(base_path + "_ctrl.coe");
    adr_coe_file.open(base_path + "_addr.coe");
    data_coe_file.open(base_path + "_data.coe");
    mask_coe_file.open(base_path + "_mask.coe");

    if (not control_coe_file || not adr_coe_file || not data_coe_file || not mask_coe_file) {
        std::cerr << "Write file open error\n";
        throw std::runtime_error{ "Write file open error" };
        return false;
    }

    control_coe_file.exceptions(7);
    adr_coe_file.exceptions(7);
    data_coe_file.exceptions(7);
    mask_coe_file.exceptions(7);

    prepare_header();

    return true;
}

void COE_Asm_File_Utils::prepare_header() {
    control_coe_file << "memory_initialization_radix = 16;\nmemory_initialization_vector = \n";
    adr_coe_file << "memory_initialization_radix = 16;\nmemory_initialization_vector = \n";
    data_coe_file << "memory_initialization_radix = 16;\nmemory_initialization_vector = \n";
    mask_coe_file << "memory_initialization_radix = 16;\nmemory_initialization_vector = \n";

    control_coe_file << std::hex;
    adr_coe_file << std::hex;
    data_coe_file << std::hex;
    mask_coe_file << std::hex;
}

void COE_Asm_File_Utils::write_instruction_to_files(const Instruction& ins, bool last_instruction) {

    if (last_instruction) {
        static const char* all_ff = "ffffffff";
        adr_coe_file << all_ff << ";";
        data_coe_file << all_ff << ";";
        mask_coe_file << all_ff << ";";
        control_coe_file << all_ff << ";";
        return;
    }

    adr_coe_file << ins.adres << ",\n";
    data_coe_file << ins.data << ",\n";
    mask_coe_file << ins.data_mask << ",\n";

    uint32_t control_word{};

    if (ins.count_as_error) {
        control_word |= 1;
    }
    control_word <<= 1;

    if (ins.write_transaction) {
        control_word |= 1;
    }
    control_word <<= 8;

    control_word |= ins.next_success_adr;
    control_word <<= 8;

    control_word |= ins.next_fail_adr;
    

    control_coe_file << control_word << ",\n";


}

bool COE_Asm_File_Utils::open_src_assembly_file(const std::string& in_asm_file_path) {

    std::cout << "Starting to read asm file : " << in_asm_file_path << "\n";
    src_assembly_file.open(in_asm_file_path);

    if (not src_assembly_file) {
        std::cerr << "Input file open error\n";
        return false;
    }

    return true;
}


bool COE_Asm_File_Utils::find_next_non_comment_line() {

    while (std::getline(src_assembly_file, m_line_str)) {
        if (m_line_str.size() > 0 && m_line_str.at(0) != '#') {
            return true;

        }
    }

    return false;

}