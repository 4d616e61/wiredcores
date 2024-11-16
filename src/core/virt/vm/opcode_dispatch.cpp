#include "core/virt/vm/thread.h"
#include <iostream>
using namespace Core::Virt::VM;

//You can just not pass the second operand if its a 0 literal anyway


//CRITIAL ERROR: FIX ASAP
//all words are casted to 64bit,  despite this not being a guarentee
//add bit widths to opcode getters and setters
//maybe cram it into this opcode lol
void Thread::OpcodeDispatch(byte opcode, Operand& o1, Operand& o2) {
    
    opcode &= (~OpcodeFlags::FL_TWO_OPERANDS);
    //TODO: add opcode asserts
    switch (opcode)
    {
    case opcodes::add:
        o1 = o1.get() + o2.get();
        break;
    case opcodes::sub:
        o1 = o1.get() - o2.get();
        break;
    case opcodes::shl:
        o1 = o1.get() << o2.get();
        break;
    case opcodes::shr:
        o1 = o1.get() >> o2.get();
        break;
    case opcodes::_or:
        o1 = o1.get() | o2.get();
        break;
    case opcodes::_and:
        o1 = o1.get() & o2.get();
        break;
    case opcodes::_xor:
        o1 = o1.get() ^ o2.get();
        break;
    case opcodes::_not:
        o1 = ~o1.get();
        break;
    //you dont actually need this, im just adding them because im such a nice person
    case opcodes::mul:
        o1 = o1.get() * o2.get();
        break;
    case opcodes::div:
        o1 = o1.get() / o2.get();
        break;
    case opcodes::mod:
        o1 = o1.get() % o2.get();
        break;
    case opcodes::mov:
        o1 = o2.get();
        break;
    //these are actually optional too
    case opcodes::jz:
        //jump to o1 if o2 = 0
        if(o2.get() == 0)
            context.ip = o1.get();
        std::cerr << o1.get();
        break;
    case opcodes::jnz:
        if(o2.get() != 0)
            context.ip = o1.get();
        break;

        
    default:
        std::__throw_runtime_error("[VM ERROR]: Unknown opcode");
        break;
    }

}