#pragma once 

#include <cstddef>
#include <cstdint>
#include <vector>


using uint64 = unsigned long long;
using byte = unsigned char;
using word = unsigned short;
using dword = unsigned int;
using qword = uint64;

using addr_t = uint64;
using pagealigned_addr_t = addr_t;
using process_id_t = uint64;



constexpr size_t PAGE_SIZE = 256;
constexpr uint64 ADDR_MASK = 0xff;

struct MemPage {
    byte data[PAGE_SIZE];
};


using page_list_t = std::vector<addr_t>;






