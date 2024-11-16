#pragma once


#include "core/misc/typedefs.h"
#include "core/virt/mem/memory_space.h"
#include <memory>
#include <string>
#include <iomanip>

#include <iostream>

namespace Core::Virt::VM {

    namespace OperandFlags {
        enum Flags {
            //whether to deref the input or not
            NEEDS_DEREF = 7,
            IS_REGISTER = 6,
            USE_REL = 3,
        };
        //bit 4 and 5 specify the operand size 

        //LITERALS:
        //bit 0 and 1 specifies the literal size
        //bit 2 idk

        //REGISTERS:
        //bit 0, 1, 2 specifies which register is referred to
        

        //Note that everything read will be zero-extended to 64bits, 
        //and writes will not affect any other bits
    }
    struct Operand {
        
        byte _operand_byte = 0;
        union {
            uint64* value_ref;
            uint64 value_lit = 0;
        };
        bool literal_fetched = false;
        bool is_ref = false;
        bool been_derefed = false;
        //uint64 op_length = 0;

        inline uint64 get_literal_n_bytes() {
            
            //if not literal, no need for additional fetching
            if(!is_literal())
                std::__throw_runtime_error("[VM ERROR]: Malformed instruction - getting literal size for register");
            //1 to 8
            return 1 << (_operand_byte & 0x3);
        }
        inline uint64 get_reg_idx() {
            //if not literal, no need for additional fetching
            if(is_literal())
                std::__throw_runtime_error("[VM ERROR]: Malformed instruction - getting register index of literal");
            //1 to 8
            return (_operand_byte & 0x7);
        }

        //Only applies to refs
        inline uint64 op_length() const {
            return 1 << ((_operand_byte >> 4) & 0x3);
        }

        inline uint64 get() const {
            if(!is_ref)
                return value_lit;
            switch (op_length())
            {
            case sizeof(qword):
                return *(qword*)value_ref;
            case sizeof(dword):
                return *(dword*)value_ref;
            case sizeof(word):
                return *(word*)value_ref;
            case sizeof(byte):
                return *(byte*)value_ref;
            default:
                std::__throw_runtime_error("[VM Error]: Unknown operation word size");
            }
        
        }

        inline void set(uint64 val) {
            
            if(!is_ref)
                std::__throw_runtime_error("[VM Error]: Error dereferencing and setting non-reference variable");
            switch (op_length())
            {
            case sizeof(qword):
                *(qword*)value_ref = val;
                break;
            case sizeof(dword):
                *(dword*)value_ref = val;
                break;
            case sizeof(word):
                *(word*)value_ref = val;
                break;
            case sizeof(byte):
                *(byte*)value_ref = val;
                break;
            default:
                std::__throw_runtime_error("[VM Error]: Unknown operation word size");
            }
        }

        inline void operator=(uint64 val) {
            set(val);
        }
        

        inline bool flag_get(OperandFlags::Flags flag) const {
            return (_operand_byte >> flag) & 1;
        }
        inline void flag_set(OperandFlags::Flags flag) {
            _operand_byte |= 1 << flag;
        }

        
        inline bool is_literal() const {
            return !flag_get(OperandFlags::IS_REGISTER);
        }
        inline bool is_register() const {
            return flag_get(OperandFlags::IS_REGISTER);
        }

        inline bool needs_deref() const {
            return flag_get(OperandFlags::NEEDS_DEREF);
        }
        // Use relative addressing(only applicable when dereferencing)
        inline bool use_rel() const {
            return flag_get(OperandFlags::USE_REL);
        }
        
        


        inline void set_literal(uint64 val) {
            value_lit = val;
        }
        
        Operand(byte op_byte) {
            _operand_byte = op_byte;
        }
        Operand(bool isreg, bool needsderef, bool userel) {
            _operand_byte = 0;
            if(isreg)
                flag_set(OperandFlags::IS_REGISTER);
            if(needsderef)
                flag_set(OperandFlags::NEEDS_DEREF);
            if(userel)
                flag_set(OperandFlags::USE_REL);
            
            
        }
    };  

}