#include "core/virt/vm/thread.h"
#include <cassert>
#include <iostream>
#include <bitset>
#include <cstring>

using namespace Core::Virt::VM;
 


void Thread::ParseOperand(Operand& operand) {
    //std::cerr << "\n" <<std::bitset<8>(operand._operand_byte) << "\n";
    if(operand.is_literal()) {
        //Fetch literal
        size_t n_bytes = operand.get_literal_n_bytes(); 
        uint64 val = 0;
        byte* fetched = &FetchInstructionBytes(n_bytes);
        memcpy(&val, fetched, n_bytes);
        operand.value_lit = val;
    }
    else {
        //if is register
        operand.is_ref = true;
        operand.value_ref = (&context.r0) + operand.get_reg_idx();
    }
    if(operand.needs_deref())
    {
        //deref
        //if literal, it is still a literal at this point so we can use get
        addr_t addr = operand.get();
        //if using relative addressing
        if(operand.use_rel())
            addr += context.base_address;
        //The type isnt necessarily uint64, im just lazy
        operand.value_ref = reinterpret_cast<uint64*>(&FetchBytes(addr, operand.op_length()));
        operand.is_ref = true;
        //subsequent gets should be the ref
    }
}


bool Thread::Step() {
    
    //fetch next instruction
    byte ins_opcode = FetchInstruction<byte>();
    Operand op1(FetchInstruction<byte>()), op2(0);
    if(is_two_operands(ins_opcode))
        op2 = Operand(FetchInstruction<byte>());
        
    ParseOperand(op1);
    if(is_two_operands(ins_opcode))
        ParseOperand(op2);

    OpcodeDispatch(ins_opcode, op1, op2);
    return true;
}

bool Thread::StepAndHandleException() {
    try {
        return Step();
    } catch(std::runtime_error e) {
        std::cerr << "\n[ERROR]: Caught VM thread exception: " << e.what();
        std::cerr << "\nThread state at error: " + this->to_string();
        return false;
    }
    return false;
    
}