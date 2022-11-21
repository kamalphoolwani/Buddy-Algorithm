#include "header.h"
#define nl NULL
using namespace std;
void *my_realloc(void *ptr, size_t size)
{
    size_t hdrsize;
    MallocHeader *hdr;
    if (ptr == nl)
    {
        size_t malsize;
        malsize = size;
        return my_malloc(malsize);
    }
    hdr = (MallocHeader *)MALLOC_BASE(ptr);
    hdrsize = hdr->size;
    void *ret;
    if (hdrsize >= size)
        return ptr;

    ret = my_malloc(size);
    memcpy((char *)ret, ptr, hdrsize);
    my_free(ptr);
    return ret;
}

void *my_reallocarray(void *ptr, size_t nmemb, size_t size)
{
    size_t totalsize = nmemb * size;
    void *ret;
    ret = my_realloc(ptr, totalsize);
    return ret;
}