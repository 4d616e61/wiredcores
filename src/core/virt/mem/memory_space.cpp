#include "core/misc/typedefs.h"
#include "core/virt/mem/memory_space.h"


using namespace Core::Virt::Mem;


MemPage* MemorySpace::FetchPage(addr_t addr, process_id_t pid) {
    //TODO: implement caching by pid
    return manager.LookupPage(addr);
}
page_list_t MemorySpace::AllocPages(size_t pages, process_id_t pid) {
    return manager.AllocPages(pages);
}
void MemorySpace::FreeByOwner(process_id_t pid) {
    
}


