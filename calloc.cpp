#include "header.h"

using namespace std;

void *my_calloc(size_t nmemb, size_t size)
{
    void *ret;
    int zerovalue;
    ret = my_malloc(nmemb * size);
    if (ret)
    {
        zerovalue = 0;
        memset(ret, zerovalue, nmemb * size);
    }
    return ret;
}