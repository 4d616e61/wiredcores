#include "core/misc/typedefs.h"
#include "core/virt/mem/memory_manager.h"
#include "core/misc/utils.h"

#include <unordered_map>
#include <vector>
#include <stdlib.h>
using namespace Core::Virt::Mem;
page_list_t MemoryManager::AllocPages(size_t n_pages) {
    pagealigned_addr_t addr = 0;
    while(true) {
        //get a random address
        addr = get_aligned(urandu64());
        bool breakflag = true;
        //if other pages are within the range(probably wont happen)
        for(size_t i = 0; i < n_pages; i++) {
            if(PageExists(addr + i * PAGE_SIZE)){
                breakflag = false;
                break;
            }
        }
        if(breakflag)
            break;

    }
    //Now that we have the base address, start allocating pages
    page_list_t pagelist;
    MemPage* mem = new MemPage[n_pages];
    allocs.emplace(addr, mem);
    for(size_t i = 0; i < n_pages; i++) {
        //TODO: verify that this works
        addr_t res_addr = addr + i * PAGE_SIZE;
        pages.emplace(res_addr, mem);
        pagelist.push_back(res_addr);

        //this should increment mem by PAGE_SIZE
        mem++;
    }
    return pagelist;
    
}
void MemoryManager::FreePage(addr_t addr) {
    pages.erase(get_aligned(addr));
    // Note that subsequent calls won't affect alloc, only the 
    // call to the actual base address will free the memory in alloc
    // This fact should be opaque to everything but the mem manager
    allocs.erase(get_aligned(addr));
}
MemPage* MemoryManager::LookupPage(addr_t addr) {
    if(!PageExists(addr))
        return nullptr;
    return pages.find(get_aligned(addr))->second;
    
}


bool MemoryManager::PageExists(addr_t addr) {
    if(pages.find(get_aligned(addr)) == pages.end())
        return false;
    return true;
}