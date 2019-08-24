#include "allocator.h"


Allocator::Allocator(const size_t n)
    : tSize_((n + sizeof(void*) - 1) / sizeof(void*))
    , pageSize_(MEMORY_PAGE_SIZE / (sizeof(void*) * (n + sizeof(void*) - 1) / sizeof(void*)))
    , free_(nullptr)
{
}

void Allocator::getMem()
{
    ++maxMemory_;
    void** beg = reinterpret_cast<void**>(malloc(MEMORY_PAGE_SIZE));
    void** end = beg + tSize_ * (pageSize_ - 1);
    void** tmp;

    IASSERT(beg);

    *(reinterpret_cast<void***>(end)) = free_;
    free_ = beg;

    do
    {
        tmp = beg + tSize_;
        *(reinterpret_cast<void***>(beg)) = tmp;
        beg = tmp;
    } while (beg < end);
}

int Allocator::maxMemory_ = 0;
