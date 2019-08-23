#pragma once

#include "iallocator.h"
#include "ivariables.h"

#include <cstdint>
#include <cstring>
#include <iostream>


class Monom64
{
public:
    Monom64* next;
    static IVariables* independ;

public:
    static void init();

    Monom64() = default;
    Monom64(const Monom64& a)
        : totalDegree_(a.totalDegree_)
        , exponent_(a.exponent_)
    {
    }

    void* operator new(size_t)
    {
        return allocator_.allocate();
    }

    void operator delete(void* ptr)
    {
        allocator_.deallocate(ptr);
    }

    size_t dimIndepend() const
    {
        return dimIndepend_;
    }

    unsigned deg(int var) const;
    unsigned operator[](int var) const
    {
        return deg(var);
    }

    unsigned degree() const;
    uint64_t rank() const;

    void set(const Monom64& a);
    void operator=(const Monom64& a)
    {
        set(a);
    }

    void setZero();
    void prolong(int var);
    void prolong(int var, unsigned deg);
    void div(int var);
    void div(const Monom64& a);

    void mult(const Monom64& a);
    void mult1(const Monom64& a);
    void mult(const Monom64& a, const Monom64& b);

    bool divisibility(const Monom64& a) const;
    bool divisibilityTrue(const Monom64& a) const;
    void divide(const Monom64& a, const Monom64& b);
    void divide1(const Monom64& a, const Monom64& b);

    uint64_t gcd(const Monom64& a) const;
    uint64_t lcm(const Monom64& a) const;
    void gcd(const Monom64& a, const Monom64& b);
    void lcm(const Monom64& a, const Monom64& b);

    bool equality(const Monom64& a, int var, unsigned degree = 1) const;
    int compare(const Monom64& a) const;
    int compare(const Monom64& a, const Monom64& b) const;

    friend std::istream& operator>>(std::istream& in, Monom64& a);
    friend std::ostream& operator<<(std::ostream& out, const Monom64& a);
    friend bool operator==(const Monom64 &a, const Monom64 &b);
    friend bool operator!=(const Monom64 &a, const Monom64 &b)
    {
        return !(a == b);
    };

private:
    uint64_t totalDegree_;
    uint64_t exponent_;

    static size_t dimIndepend_;
    static IAllocator allocator_;
    static uint64_t zero_[64];
    static uint64_t one_[64];
    static uint64_t degrees_[256];
};

inline void Monom64::set(const Monom64& a)
{
    memcpy(this, &a, sizeof(Monom64));
}

inline unsigned Monom64::deg(int var) const
{
    return (exponent_ & one_[var]) ? 1 : 0;
}

inline unsigned Monom64::degree() const
{
    return totalDegree_;
}

inline uint64_t Monom64::rank() const {
    return exponent_;
}

inline void Monom64::setZero()
{
    totalDegree_ = 0;
    exponent_ = 0;
}

inline void Monom64::prolong(int var)
{
    if (!(exponent_ & one_[var]))
    {
        exponent_ |= one_[var];
        ++totalDegree_;
    };
}

inline void Monom64::prolong(int var, unsigned deg)
{
    if (deg > 0)
    {
        prolong(var);
    }
}

inline void Monom64::div(int var)
{
    if (exponent_ & one_[var])
    {
        exponent_ &= zero_[var];
        --totalDegree_;
    }
    else
    {
        IERROR("Monom can't be divided by variable");
    }
}

inline void Monom64::div(const Monom64& a)
{
    exponent_ ^= a.exponent_;
    uint8_t* c = reinterpret_cast<uint8_t*>(&exponent_);
    totalDegree_ = degrees_[*c];
    for (int i = 0; i < 7; ++i)
    {
        ++c;
        totalDegree_ += degrees_[*c];
    }
}

inline void Monom64::mult(const Monom64& a)
{
    exponent_ |= a.exponent_;
    uint8_t* c = reinterpret_cast<uint8_t*>(&exponent_);
    totalDegree_ = degrees_[*c];
    for (int i = 0; i < 7; ++i)
    {
        ++c;
        totalDegree_ += degrees_[*c];
    }
}

inline void Monom64::mult1(const Monom64& a)
{
    exponent_ |= a.exponent_;
}

inline void Monom64::mult(const Monom64& a, const Monom64& b)
{
    exponent_ = a.exponent_ | b.exponent_;
    uint8_t* c = reinterpret_cast<uint8_t*>(&exponent_);
    totalDegree_ = degrees_[*c];
    for (int i = 0; i < 7; ++i)
    {
        ++c;
        totalDegree_ += degrees_[*c];
    }
}

inline bool Monom64::divisibility(const Monom64& a) const
{
    uint64_t d = exponent_ ^ a.exponent_;
    d &= a.exponent_;
    return d == 0;
}

inline bool Monom64::divisibilityTrue(const Monom64& a) const
{
    uint64_t d = exponent_ ^ a.exponent_;
    if (d == 0)
    {
        return false;
    }

    d &= a.exponent_;
    return d == 0;
}

inline void Monom64::divide(const Monom64& a, const Monom64& b)
{
    totalDegree_ = a.totalDegree_ - b.totalDegree_;
    exponent_ = a.exponent_ ^ b.exponent_;
}

inline void Monom64::divide1(const Monom64& a, const Monom64& b)
{
    exponent_ = a.exponent_ ^ b.exponent_;
}

inline uint64_t Monom64::gcd(const Monom64& a) const
{
    uint64_t d = exponent_ & a.exponent_;
    uint8_t* c = reinterpret_cast<uint8_t*>(&d);
    uint64_t r = degrees_[*c];
    for (int i = 0; i < 7; ++i)
    {
        ++c;
        r += degrees_[*c];
    }
    return r;
}

inline uint64_t Monom64::lcm(const Monom64& a) const
{
    uint64_t d = exponent_ | a.exponent_;
    uint8_t* c = reinterpret_cast<uint8_t*>(&d);
    uint64_t r = degrees_[*c];
    for (int i = 0; i < 7; ++i)
    {
        ++c;
        r += degrees_[*c];
    }
    return r;
}

inline void Monom64::gcd(const Monom64& a, const Monom64& b)
{
    exponent_ = a.exponent_ & b.exponent_;
    uint8_t* c = reinterpret_cast<uint8_t*>(&exponent_);
    totalDegree_ = degrees_[*c];
    for (int i = 0; i < 7; ++i)
    {
        ++c;
        totalDegree_ += degrees_[*c];
    }
}

inline void Monom64::lcm(const Monom64& a, const Monom64& b)
{
    exponent_ = a.exponent_ | b.exponent_;
    uint8_t* c = reinterpret_cast<uint8_t*>(&exponent_);
    totalDegree_ = degrees_[*c];
    for (int i = 0; i < 7; ++i)
    {
        ++c;
        totalDegree_ += degrees_[*c];
    }
}

inline bool Monom64::equality(const Monom64& a, int var, unsigned) const
{
    uint64_t d = 1;
    d = d << var;
    d |= a.exponent_;
    return exponent_ == d;
}

inline int Monom64::compare(const Monom64& a) const
{
    if (totalDegree_ < a.totalDegree_)
    {
        return -1;
    }
    else if (totalDegree_ > a.totalDegree_)
    {
        return 1;
    }

    if (exponent_ < a.exponent_)
    {
        return 1;
    }
    else if (exponent_ > a.exponent_)
    {
        return -1;
    }

    return 0;
}

inline bool operator==(const Monom64& a, const Monom64& b)
{
    return a.exponent_ == b.exponent_;
}
