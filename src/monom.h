#pragma once

#include "allocator.h"
#include "variables.h"

#include <cstdint>
#include <cstring>
#include <iostream>


class Monom
{
public:
    Monom* next;
    static Variables* independ;

public:
    static void init();

    Monom() = default;
    Monom(const Monom& a)
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

    void set(const Monom& a);
    void operator=(const Monom& a)
    {
        set(a);
    }

    void setZero();
    void prolong(int var);
    void prolong(int var, unsigned deg);
    void div(int var);
    void div(const Monom& a);

    void mult(const Monom& a);
    void mult1(const Monom& a);
    void mult(const Monom& a, const Monom& b);

    bool divisibility(const Monom& a) const;
    bool divisibilityTrue(const Monom& a) const;
    void divide(const Monom& a, const Monom& b);
    void divide1(const Monom& a, const Monom& b);

    uint64_t gcd(const Monom& a) const;
    uint64_t lcm(const Monom& a) const;
    void gcd(const Monom& a, const Monom& b);
    void lcm(const Monom& a, const Monom& b);

    bool equality(const Monom& a, int var, unsigned degree = 1) const;
    int compare(const Monom& a) const;
    int compare(const Monom& a, const Monom& b) const;

    friend std::istream& operator>>(std::istream& in, Monom& a);
    friend std::ostream& operator<<(std::ostream& out, const Monom& a);
    friend bool operator==(const Monom &a, const Monom &b);
    friend bool operator!=(const Monom &a, const Monom &b)
    {
        return !(a == b);
    };

private:
    uint64_t totalDegree_;
    uint64_t exponent_;

    static size_t dimIndepend_;
    static Allocator allocator_;
    static uint64_t zero_[64];
    static uint64_t one_[64];
    static uint64_t degrees_[256];
};

inline void Monom::set(const Monom& a)
{
    memcpy(this, &a, sizeof(Monom));
}

inline unsigned Monom::deg(int var) const
{
    return (exponent_ & one_[var]) ? 1 : 0;
}

inline unsigned Monom::degree() const
{
    return totalDegree_;
}

inline uint64_t Monom::rank() const {
    return exponent_;
}

inline void Monom::setZero()
{
    totalDegree_ = 0;
    exponent_ = 0;
}

inline void Monom::prolong(int var)
{
    if (!(exponent_ & one_[var]))
    {
        exponent_ |= one_[var];
        ++totalDegree_;
    };
}

inline void Monom::prolong(int var, unsigned deg)
{
    if (deg > 0)
    {
        prolong(var);
    }
}

inline void Monom::div(int var)
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

inline void Monom::div(const Monom& a)
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

inline void Monom::mult(const Monom& a)
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

inline void Monom::mult1(const Monom& a)
{
    exponent_ |= a.exponent_;
}

inline void Monom::mult(const Monom& a, const Monom& b)
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

inline bool Monom::divisibility(const Monom& a) const
{
    uint64_t d = exponent_ ^ a.exponent_;
    d &= a.exponent_;
    return d == 0;
}

inline bool Monom::divisibilityTrue(const Monom& a) const
{
    uint64_t d = exponent_ ^ a.exponent_;
    if (d == 0)
    {
        return false;
    }

    d &= a.exponent_;
    return d == 0;
}

inline void Monom::divide(const Monom& a, const Monom& b)
{
    totalDegree_ = a.totalDegree_ - b.totalDegree_;
    exponent_ = a.exponent_ ^ b.exponent_;
}

inline void Monom::divide1(const Monom& a, const Monom& b)
{
    exponent_ = a.exponent_ ^ b.exponent_;
}

inline uint64_t Monom::gcd(const Monom& a) const
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

inline uint64_t Monom::lcm(const Monom& a) const
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

inline void Monom::gcd(const Monom& a, const Monom& b)
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

inline void Monom::lcm(const Monom& a, const Monom& b)
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

inline bool Monom::equality(const Monom& a, int var, unsigned) const
{
    uint64_t d = 1;
    d = d << var;
    d |= a.exponent_;
    return exponent_ == d;
}

inline int Monom::compare(const Monom& a) const
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

inline bool operator==(const Monom& a, const Monom& b)
{
    return a.exponent_ == b.exponent_;
}