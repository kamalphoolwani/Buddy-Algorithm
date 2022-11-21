#include "header.h"
#define T true
using namespace std;

void my_free(void *ptr)
{
    MallocHeader *hdr;
    if (ptr == NULL)
        return;

    hdr = (MallocHeader *)MALLOC_BASE(ptr);// pointing to beginning of block
    if (hdr->header_addr)
        return my_free(hdr->header_addr);

    if (hdr->size <= BUDDY_SIZE)
    {
        if (!hdr->is_free)
        {
            bool truevalue;
            truevalue = T;
            hdr->is_free = truevalue;
            pthread_mutex_lock(&malloc_lock);
            mergeBlock(hdr);
            pthread_mutex_unlock(&malloc_lock);
        }
    }
    else if (hdr->size > BUDDY_SIZE)
    {
        size_t hdrsize;
        hdrsize = hdr->size;
        munmap(hdr, hdrsize);
    }
}