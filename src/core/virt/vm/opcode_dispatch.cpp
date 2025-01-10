#include "core/virt/vm/thread.h"
#include <iostream>
using namespace Core::Virt::VM;

void Thread::OpcodeDispatch_1op(byte opcode, Operand &o1)
{
    switch (opcode)
    {
    case opcodes_1op::inc:
        o1 = o1.get() + 1;
        break;
    case opcodes_1op::dec:
        o1 = o1.get() - 1;
        break;
    case opcodes_1op::_not:
        o1 = ~o1.get();
        break;
    case opcodes_1op::jmp:
        context.ip = o1.get();
    case opcodes_1op::jf:
        if(context.flag)
            context.ip = o1.get();
        break;
    case opcodes_1op::jnf:
        if(!context.flag)
            context.ip = o1.get();
        break;
    default:
        std::__throw_runtime_error("[VM ERROR]: Unknown opcode");
        break;
    }
}
void Thread::OpcodeDispatch_2op(byte opcode, Operand &o1, Operand &o2)
{
    switch (opcode)
    {
    case opcodes_2op::add:
        o1 = o1.get() + o2.get();
        break;
    case opcodes_2op::sub:
        o1 = o1.get() - o2.get();
        break;
    case opcodes_2op::shl:
        o1 = o1.get() << o2.get();
        break;
    case opcodes_2op::shr:
        o1 = o1.get() >> o2.get();
        break;
    case opcodes_2op::_or:
        o1 = o1.get() | o2.get();
        break;
    case opcodes_2op::_and:
        o1 = o1.get() & o2.get();
        break;
    case opcodes_2op::_xor:
        o1 = o1.get() ^ o2.get();
        break;
    // you dont actually need this, im just adding them because im such a nice person
    case opcodes_2op::mul:
        o1 = o1.get() * o2.get();
        break;
    case opcodes_2op::div:
        o1 = o1.get() / o2.get();
        break;
    case opcodes_2op::mod:
        o1 = o1.get() % o2.get();
        break;
    case opcodes_2op::mov:
        o1 = o2.get();
        break;
    // these are actually optional too
    case opcodes_2op::cmpg:
        if( o1.get() > o2.get() ){
            context.flag = 1;
        } else {
            context.flag = 0;
        }
        break;
    case opcodes_2op::cmpge:
        if( o1.get() >= o2.get() ){
            context.flag = 1;
        } else {
            context.flag = 0;
        }
        break;
    case opcodes_2op::cmpeq:
        if( o1.get() == o2.get() ){
            context.flag = 1;
        } else {
            context.flag = 0;
        }
        break;
    case opcodes_2op::cmple:
        if( o1.get() <= o2.get() ){
            context.flag = 1;
        } else {
            context.flag = 0;
        }
        break;
    case opcodes_2op::cmpl:
        if( o1.get() < o2.get() ){
            context.flag = 1;
        } else {
            context.flag = 0;
        }
        break;


    default:
        std::__throw_runtime_error("[VM ERROR]: Unknown opcode");
        break;
    }
}
void Thread::OpcodeDispatch(byte opcode, Operand &o1, Operand &o2)
{
    bool is_2op = opcode >> OpcodeFlags::TWO_OPERANDS & 1;
    opcode &= (~OpcodeFlags::FL_TWO_OPERANDS);
    if (is_2op)
    {
        OpcodeDispatch_2op(opcode, o1, o2);
        return;
    }
    OpcodeDispatch_1op(opcode, o1);

    // TODO: add opcode asserts
}