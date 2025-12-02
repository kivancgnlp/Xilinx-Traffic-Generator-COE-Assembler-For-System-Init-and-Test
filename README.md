# Xilinx Traffic Generator COE Assembler (For System Init and Test)

For writing simple applications to Xilinx Traffic Generator IP using my made up assembly language. Four ".coe" files are generated as a result of the compilation.

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

## Build and run instructions 


### Run 
```
./Coe_Assembler .\I2C_App_1.txt
Starting to read asm file : .\I2C_App_1.txt
[0] Write : 20 <= d0
[1] Write : 108 <= 68
[2] Write : 108 <= 21
[3] Write : 120 <= 1
[4] Write : 100 <= d
[5] Read from : 20, compare mask : 4, compare data : 4
[6] Write : 20 <= 4
[7] Write : 100 <= 25
[8] Write : 108 <= 69
[9] Read from : 100
[10] Read from : 20, compare mask : 4, compare data : 4
[11] Write : 20 <= 4
[12] Read from : 20, compare mask : 8, compare data : 8
[13] Write : 20 <= 8
[14] Write : 100 <= 1
[15] Read from : 10c
[16] Read from : 10c
```
