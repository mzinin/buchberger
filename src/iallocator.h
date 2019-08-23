#pragma once

#include "iutil.h"


class IAllocator
{
private:
    enum
    {
        MEMORY_PAGE_SIZE = 4096
    };

public:
    static int maxMemory()
    {
        return maxMemory_ * MEMORY_PAGE_SIZE;
    }

    IAllocator(const size_t n);

    inline void* allocate()
    {
        if (!free_)
        {
            getMem();
        }
        IASSERT(free_);

        void* ptr = reinterpret_cast<void*>(free_);
        free_ = reinterpret_cast<void**>(*free_);
        return ptr;
    }

    inline void deallocate(void* ptr)
    {
        *(reinterpret_cast<void***>(ptr)) = free_;
        free_ = reinterpret_cast<void**>(ptr);
    }

private:
    void getMem();

private:
    const size_t tSize_;
    const size_t pageSize_;
    void**       free_;
    static int   maxMemory_;
};
