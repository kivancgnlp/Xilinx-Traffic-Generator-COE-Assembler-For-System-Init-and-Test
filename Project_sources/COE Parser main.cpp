// Xilinx_Trafic_Generator_System_Test_Util.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>

#include "COE_Parse_File_Utils.hpp"



int main(){

    const std::string file_path_base{ R"(C:\xilinx_ws\vivado_ws\KC\I2C_testbench_ex\axi_iic_0_ex\imports\atg_16)" };
    //const std::string file_path_base{ "/Users/kivanc/Documents/GitHub/Kiv_Cpp_Calismalar/ROM (COE) file reader for Xilinx Traffic Generator ( System Init and Test )/sample_data/atg" };
   
    constexpr bool input_coe_files_in_hex = true;
    constexpr bool also_convert_input_files_to_hexadecimal = false;
    constexpr bool write_parsed_data_to_file = true;

    COE_Parse_File_Utils cfu{file_path_base,also_convert_input_files_to_hexadecimal,input_coe_files_in_hex};

    if (not cfu.all_files_ok()){
        std::cerr << "File open error\n";
        return -1;
    }

    cfu.load_known_adr_map(R"(D:\BilgiSistemKivDec\2025-01-22\Kiv_Cpp_Calismalar\ROM (COE) file reader for Xilinx Traffic Generator ( System Init and Test )\aux_data\known_adrs.txt)");
    //cfu.load_known_adr_map("/Users/kivanc/Documents/GitHub/Kiv_Cpp_Calismalar/ROM (COE) file reader for Xilinx Traffic Generator ( System Init and Test )/aux_data/known_adrs.txt");
    
  
    cfu.skip_header();
    Instruction ins = cfu.find_next_non_comment_line();

    std::ofstream of;
    if (write_parsed_data_to_file) {
        of.open("COE Parsed Output.txt");
    }
    

    
    while (ins.adres != 0xffffffff) {
        std::cout << ins << " (" << cfu.lookup_adr(ins.adres) << ")\n";

        if (write_parsed_data_to_file) {
            of << ins << " (" << cfu.lookup_adr(ins.adres) << ")\n";
        }

        ins = cfu.find_next_non_comment_line();
    }
        
    std::cout << "End of app\n";
    return 0;
}

