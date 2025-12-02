#pragma once

#include <iostream>

struct Instruction {
    unsigned row_index;
    bool write_transaction;
    unsigned adres;
    unsigned data;

    unsigned data_mask;

    unsigned next_fail_adr;
    unsigned next_success_adr;
    bool count_as_error;

    friend std::ostream& operator<<(std::ostream& os, const Instruction& ins) {

        os << '[' << ins.row_index << "] ";
        if (ins.write_transaction) {
            os << std::hex << "Write : " << ins.adres << " <= " << ins.data << std::dec;
        } else {
            os << std::hex << "Read from : " << ins.adres << std::dec;

            if (ins.data_mask != 0 || ins.data != 0) {
                os << std::hex << ", compare mask : " << ins.data_mask << ", compare data : " << ins.data << std::dec;
            }
            
            
        }

        if (ins.row_index + 1 != ins.next_success_adr) {
            os << "(non standard success flow)";
        }

        if (ins.row_index != ins.next_fail_adr) {
            os << "(non standard fail flow)";
        }

        return os;
    }


};