#include "core/virt/vm/thread.h"

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include "core/misc/utils.h"

#define writebyte(argbyte) page->data[ctr] = argbyte; ctr++;

using namespace Core::Virt::VM;
int main(int argc, char** argv) {


    Core::Virt::Mem::MemorySpace memspace;
     
    addr_t addr = memspace.AllocPages(1, 1)[0];
    MemPage* page = memspace.FetchPage(addr);
    Core::Virt::VM::Thread thread(&memspace, addr, 1);


    //init ip
    thread.context.ip = thread.context.base_address;

    //mov r7, num_to_test
    uint64 num_to_test = 1336;
    byte u64_mask = 1 << 4 | 1 << 5;
    //qword
    uint64 ctr = 0;
    writebyte(opcodes::mov | OpcodeFlags::FL_TWO_OPERANDS);
    //arg0: r7
    writebyte((Operand(true, false, false)._operand_byte | 7 | u64_mask));
    //arg1: literal(64 bits wide=3)
    writebyte(Operand(false, false, false)._operand_byte | 3);
    //write in literal
    memcpy(page->data + ctr, &num_to_test, sizeof(num_to_test));
    ctr += sizeof(num_to_test);
    
    
    

    //instruction 2: square r7
    writebyte(opcodes::mul | OpcodeFlags::FL_TWO_OPERANDS)
    //arg0 = arg1 = r7
    writebyte(Operand(true, false, false)._operand_byte | 7 | u64_mask);
    writebyte(Operand(true, false, false)._operand_byte | 7 | u64_mask);

    //instruction 3: sub r7 by 1
    writebyte(opcodes::sub | OpcodeFlags::FL_TWO_OPERANDS)
    writebyte(Operand(true, false, false)._operand_byte | 7 | u64_mask);
    //1 byte literal
    writebyte(Operand(false, false, false)._operand_byte | 0);
    //write literal
    writebyte(1);

    //instruction 4: mod r7 by 24
    writebyte(opcodes::mod | OpcodeFlags::FL_TWO_OPERANDS)
    writebyte(Operand(true, false, false)._operand_byte | 7 | u64_mask);
    //1 byte literal
    writebyte(Operand(false, false, false)._operand_byte | 0);
    //write literal
    writebyte(24);
    //instruction 5: jump to 0 if it is prime
    writebyte(opcodes::jz | OpcodeFlags::FL_TWO_OPERANDS);
    writebyte(Operand(false, false, false)._operand_byte | 0);
    writebyte(Operand(true, false, false)._operand_byte | 7 | u64_mask);
    writebyte(0);

    std::cerr << dump_page(page);
    std::cout << ctr;

    std::cout << thread.to_string() << "\n";
    thread.StepAndHandleException();
    std::cout << thread.to_string() << "\n";
    thread.StepAndHandleException();
    std::cout << thread.to_string() << "\n";
    thread.StepAndHandleException();
    std::cout << thread.to_string() << "\n";
    thread.StepAndHandleException();
    std::cout << thread.to_string() << "\n";
    thread.StepAndHandleException();
    std::cout << thread.to_string();
    thread.StepAndHandleException();
    
}
