#pragma once

#include "core/misc/typedefs.h"
#include "core/virt/mem/memory_space.h"
#include "core/virt/vm/opcode.h"
#include "core/virt/vm/operand.h"
#include <memory>
#include <string>
#include <iomanip>

namespace Core::Virt::VM
{

    struct ThreadContext
    {
        // registers
        // r0, aka ip
        union
        {
            uint64 r0, ip = 0;
        };
        // r1, also used for address basing, you can use it for whatever you want
        // if you dont need rel addressing
        union
        {
            uint64 r1, base_address = 0;
        };

        // general purpose regisers
        uint64 r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0;
        bool flag = 0;

        // addr_t base_address = 0;
        uint64 cycle_count = 0;
        uint64 pid = 0;

        std::string to_string()
        {
            std::string res;
            std::stringstream stream;
            stream << "\nr0/ip:   " << r0 << " / 0x" << std::hex << r0 << std::dec;
            stream << "\nr1/base: " << r1 << " / 0x" << std::hex << r1 << std::dec;
            for (int i = 2; i < 8; i++)
            {
                uint64 v = *((uint64 *)this + i);
                stream << "\nr" << i << ":      " << v << " / 0x" << std::hex << v << std::dec;
            }
            stream << "\nflag:    " << flag;

            return stream.str();
        }
        // ThreadContext();
    };

    class Thread
    {

        // UNSAFE FUNCTION!
        // all the addresses in references are real addresses, make sure it is properly resolved
        void OpcodeDispatch(byte opcode, Operand &o1, Operand &o2);
        void OpcodeDispatch_1op(byte opcode, Operand &o1);
        void OpcodeDispatch_2op(byte opcode, Operand &o1, Operand &o2);

        // std::shared_ptr<Core::Virt::Mem::MemorySpace> memspace;
        Core::Virt::Mem::MemorySpace *memspace;

    public:
        ThreadContext context;

    private:
        // TODO: page caching
        template <typename T>
        inline T &Fetch(addr_t addr)
        {
            return memspace->Fetch<T>(addr, context.pid);
        }
        inline byte &FetchBytes(addr_t addr, size_t size)
        {
            return memspace->FetchBytes(addr, size, context.pid);
        }
        template <typename T>
        inline T FetchInstruction()
        {
            context.ip += sizeof(T);
            return memspace->Fetch<T>(context.ip - sizeof(T), context.pid);
        }
        inline byte &FetchInstructionBytes(size_t size)
        {
            context.ip += size;
            return memspace->FetchBytes(context.ip - size, size, context.pid);
        }
        void ParseOperand(Operand &operand);

    public:
        bool Step();
        bool StepAndHandleException();
        // bool StepCycles(size_t n_cycles);

        inline Thread(Core::Virt::Mem::MemorySpace *memspace, uint64 addrbase, uint64 pid)
        {
            this->memspace = memspace;
            this->context.pid = pid;
            this->context.base_address = addrbase;
        }
        std::string to_string()
        {
            return context.to_string();
        }
    };

}