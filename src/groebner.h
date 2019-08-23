#pragma once

#include "polynom.h"

#include <algorithm>
#include <vector>


struct Pair
{
    uint16_t i;
    uint16_t j;
    uint64_t lcm;
    uint16_t degree;

    static Allocator allocator;

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


class GroebnerBasis
{
public:
    GroebnerBasis() = default;
    GroebnerBasis(const std::vector<Polynom*>& set);

    Polynom* operator[](size_t num);
    Polynom* sPoly(int i, int j);

    void selectPair(int& i, int& j);
    size_t length();
    void reduceSet(int i);

    friend std::ostream& operator<<(std::ostream& out, GroebnerBasis& gBasis);

private:
    bool criterion1(int i, int j, unsigned long& lcm, int& degree);
    bool criterion2(int i, int j);
    void pushPoly(Polynom* p);
    void calculateGB();

private:
    std::vector<Polynom*> basis_;
    size_t dim_ = 0;
    std::vector<std::vector<bool>> allPairs_;
    std::vector<Pair*> refToPairs_;
};

inline Polynom* GroebnerBasis::operator[](size_t num)
{
    auto it(basis_.begin());
    it += length() - 1 - num;
    return *it;
}

inline void GroebnerBasis::selectPair(int& i, int& j)
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
