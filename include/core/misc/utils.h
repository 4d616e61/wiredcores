#pragma once 
#include <random>
#include "core/misc/typedefs.h"
#include <iomanip>
#include <sstream>
#include <string>



inline pagealigned_addr_t get_aligned(addr_t addr) {
    return addr & (~ADDR_MASK);
}



inline uint64 urandu64() {
    std::random_device rd;
    return (uint64)rd() << 32 | (uint64)rd();
}


inline std::string dump_page(const MemPage* page) {
        std::stringstream ss;
        ss << std::hex;
        for(size_t i = 0; i < 16; i++) {
            for(size_t j = 0; j < 16; j++) {
                ss << std::setw(2) << std::setfill('0') << (int)page->data[i * 16 + j] << " ";
            }
            ss << "\n";
        }
        return ss.str();
    }