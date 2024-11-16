#pragma once


#include "core/misc/typedefs.h"


#include <unordered_map>
#include <vector>
#include <memory>

namespace Core::Virt::Mem {

    //Consider using a range tree for page lookup later

    class MemoryManager {
        std::unordered_map<pagealigned_addr_t, std::unique_ptr<MemPage>> allocs;
        std::unordered_map<pagealigned_addr_t, MemPage*> pages;
        

        public:
            //ALL addresses returned in page_list_t must be freed with FreePage
            page_list_t AllocPages(size_t pages);
            
            void FreePage(addr_t addr);
            MemPage* LookupPage(addr_t addr);
            bool PageExists(addr_t addr);

    };
}