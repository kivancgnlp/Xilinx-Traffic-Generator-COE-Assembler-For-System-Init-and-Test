

#include <iostream>
#include "COE_Asm_File_Utils.hpp"

using namespace std;


int main(int argc, const char *argv[]) try{

    if (argc != 2) {
        std::cerr << "Asm file path should be given as a argument\n";
        return -1;
    }
 
    COE_Asm_File_Utils asm_utils;

    bool ok = asm_utils.open_src_assembly_file(argv[1]);

    if (not ok) {
        std::cerr << "Asm file open error\n";
        return -2;
    }
    
    std::string output_files_base_path = COE_Asm_File_Utils::generate_output_files_base_path(argv[1]);
    asm_utils.open_output_coe_files(output_files_base_path);

    Instruction ins;
    while (asm_utils.find_next_non_comment_line()) {
        ins = asm_utils.parse_current_line();
        asm_utils.write_instruction_to_files(ins);
        std::cout << ins << "\n";
    }

    asm_utils.write_instruction_to_files(ins, true);
    std::cout << "End of app\n";
    return 0;


} catch(const std::exception &ex){
    std::cerr << "Exception catched : " << ex.what();
    return -3;
}

