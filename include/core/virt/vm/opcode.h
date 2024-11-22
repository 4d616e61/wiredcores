#pragma once

#include "core/misc/typedefs.h"


namespace Core::Virt::VM {

    namespace OpcodeFlags {
        enum Flags {
            TWO_OPERANDS = 7
        };
        constexpr byte FL_TWO_OPERANDS = 1 << TWO_OPERANDS;
    };

    namespace opcodes_1op { 
        enum ops {
            inc,
            dec,
            _not,
            jmp,


        };
    };
    namespace opcodes_2op { 
        enum ops{
            add,
            sub, 
            shl,
            shr,
            _or,
            _and,
            _xor,
            _not,
            mul,
            div,
            mod,
            mov,
            jz,
            jnz,


        };
    };
    inline bool is_two_operands(byte _opcode_byte) {
        if(_opcode_byte >> OpcodeFlags::TWO_OPERANDS & 1) 
            return true;
        return false;
    }

}