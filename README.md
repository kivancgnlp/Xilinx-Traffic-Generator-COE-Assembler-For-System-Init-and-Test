# Xilinx Traffic Generator COE Assembler (For System Init and Test)

For writing simple applications to Xilinx TG IP using my made up assembly language. Four ".coe" files are generated as a result of the compilation.

# Instruction formats

All values are hexadecimal and all operands should be seperated using white spaces. Lines starting with '#' are interpreted as comment line and ';' usage at the end of assembly instructions are also interpreted as comment.

## Write instruction 

**Syntax** : "W A D [MASK] [NEXT ADR]"

Begins with letter 'W' and adr, data operands follows. Usually adr and data is enough for this instruction. Mask sometimes required for narrow writes and if you omit next adr then next adr is set to the next instruction.

**Examples** : 
- "W 108 68" Write 68 to the adr 108. Continue execution with the next instruction.
- "W 108 68 ff 0" Write 68 to the adr 108 but use first byte enable to indicate a byte write on AXI bus. Goto start (adr 0) for next instruction.

## Read instruction

**Syntax** : "R A [COMPARE DATA] [COMPARE MASK] [NEXT ADR]"

Begins with letter 'R' and adr of the data to be read follows. While loops can be constructed using compare operands.

Examples :
- "R 100" just read from adr 100 and continue execution.
- "R 104 80 80" read from adr 104 and mask it with 80, if the result is 80 continue execution otherwise repeat this read instruction.

## See the "Example_assembly_apps" folder for sample applications


