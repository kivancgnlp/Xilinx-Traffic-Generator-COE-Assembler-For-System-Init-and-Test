cmake_minimum_required (VERSION 3.8)


project ("Coe_Assembler")


add_executable (Coe_Assembler  "COE Assembler main.cpp" "COE_Asm_File_Utils.cpp")
add_executable (Coe_Disassembler "COE Parser main.cpp" "COE_Parse_File_Utils.cpp")

set_property(TARGET Coe_Assembler PROPERTY CXX_STANDARD 20)
set_property(TARGET Coe_Disassembler PROPERTY CXX_STANDARD 20)


# TODO: Add tests and install targets if needed.
