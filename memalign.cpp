#include "header.h"
#define nl NULL
#define malloch_header_size MALLOC_HEADER_SIZE

void *my_memalign(size_t alignment, size_t size)
{
    void *ptr;
    size_t newsize = size + alignment + malloch_header_size;
    ptr = my_malloc(newsize);
    if (ptr)
    {
        ((MallocHeader *)MALLOC_BASE((void *)(next_addr(alignment, ptr))))->header_addr = ptr;
        return (void *)(next_addr(alignment, ptr));
    }

    return nl;
}

int my_posix_memalign(void **memptr, size_t alignment, size_t size)
{
    void *ret = my_memalign(alignment, size);
    if (ret)
    {
        *memptr = ret;
        return 0;
    }    
    return errno;
}