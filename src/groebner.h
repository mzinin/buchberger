#pragma once

#include "polynom.h"

#include <algorithm>
#include <vector>


struct Pair
{
    size_t i;
    size_t j;
    uint64_t lcm;
    uint16_t degree;

    static Allocator allocator;

    Pair(size_t iNew, size_t jNew, uint64_t lcmNew, uint16_t degreeNew)
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


class GroebnerBasis
{
public:
    GroebnerBasis() = default;
    GroebnerBasis(const std::vector<Polynom*>& set);

    Polynom* operator[](size_t num);
    Polynom* sPoly(size_t i, size_t j);

    void selectPair(size_t& i, size_t& j);
    size_t length();
    void reduceSet(int i);

    friend std::ostream& operator<<(std::ostream& out, GroebnerBasis& gBasis);

private:
    bool criterion1(size_t i, size_t j, uint64_t& lcm, uint16_t& degree);
    bool criterion2(size_t i, size_t j);
    void pushPoly(Polynom* p);
    void calculateGB();

private:
    std::vector<Polynom*> basis_;
    uint16_t dim_ = 0;
    std::vector<std::vector<bool>> allPairs_;
    std::vector<Pair*> refToPairs_;
};

inline Polynom* GroebnerBasis::operator[](size_t num)
{
    auto it = basis_.begin();
    std::advance(it, length() - 1 - num);
    return *it;
}

inline void GroebnerBasis::selectPair(size_t& i, size_t& j)
{
    i = refToPairs_.back()->i;
    j = refToPairs_.back()->j;
    refToPairs_.pop_back();
    return;
}

inline size_t GroebnerBasis::length()
{
    return basis_.size();
}
