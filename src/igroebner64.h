#pragma once

#include "poly64.h"

#include <algorithm>
#include <vector>


struct Pair
{
    uint16_t i;
    uint16_t j;
    uint64_t lcm;
    uint16_t degree;

    static IAllocator allocator;

    Pair(uint16_t iNew, uint16_t jNew, uint64_t lcmNew, uint16_t degreeNew)
        : i(iNew)
        , j(jNew)
        , lcm(lcmNew)
        , degree(degreeNew)
    {
    }

    void* operator new(size_t)
    {
        return allocator.allocate();
    }

    void operator delete(void* ptr)
    {
        allocator.deallocate(ptr);
    }
};


class IGBasis64
{
public:
    IGBasis64() = default;
    IGBasis64(const std::vector<Poly64*>& set);

    Poly64* operator[](size_t num);
    Poly64* sPoly(int i, int j);

    void selectPair(int& i, int& j);
    size_t length();
    void reduceSet(int i);

    friend std::ostream& operator<<(std::ostream& out, IGBasis64& gBasis);

private:
    bool criterion1(int i, int j, unsigned long& lcm, int& degree);
    bool criterion2(int i, int j);
    void pushPoly(Poly64* p);
    void calculateGB();

private:
    std::vector<Poly64*> basis_;
    size_t dim_ = 0;
    std::vector<std::vector<bool>> allPairs_;
    std::vector<Pair*> refToPairs_;
};

inline Poly64* IGBasis64::operator[](size_t num)
{
    auto it(basis_.begin());
    it += length() - 1 - num;
    return *it;
}

inline void IGBasis64::selectPair(int& i, int& j)
{
    i = refToPairs_.back()->i;
    j = refToPairs_.back()->j;
    refToPairs_.pop_back();
    return;
}

inline size_t IGBasis64::length()
{
    return basis_.size();
}
