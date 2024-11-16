#pragma once

#include "core/misc/typedefs.h"
#include "core/virt/mem/memory_manager.h"

namespace Core::Virt::Mem {
    class MemorySpace {

        MemoryManager manager;
        public:
        MemPage* FetchPage(addr_t address, process_id_t pid = 0);

        public:
        
        page_list_t AllocPages(size_t pages, process_id_t pid);
        

        
        void FreeByOwner(process_id_t pid);

        //pid is specified for caching purposes, not required
        //status is reserved for additional features

        byte& FetchBytes(addr_t address, size_t size, process_id_t pid = 0, uint64* status = 0) {
            MemPage* page = FetchPage(address);
            addr_t low_ptr = address & ADDR_MASK;
            addr_t high_ptr = low_ptr + size;

            //TODO: maaaaaybe consider not making fetches page aligned
            if(page == nullptr || high_ptr > PAGE_SIZE)
                std::__throw_runtime_error("VM Segmentation fault");
            return *(page->data + low_ptr); 
        }
        template<typename T>
        T& Fetch(addr_t address, process_id_t pid = 0, uint64* status = 0) {
            return reinterpret_cast<T&>(FetchBytes(address, sizeof(T), pid, status));
        }
        
        //auto& FetchByte = _Fetch<byte>;



        
        
    };
}