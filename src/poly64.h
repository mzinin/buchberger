#pragma once

#include "monom64.h"

#include <iostream>


class Poly64
{
public:
    class Iterator;

    class ConstIterator
    {
    public:
        ConstIterator(const Monom64* constIt)
            : constIt_(constIt)
        {
        }

        void operator=(const ConstIterator& a)
        {
            constIt_ = a.constIt_;
        }

        operator bool() const
        {
            return constIt_;
        }

        bool isNext() const
        {
            return constIt_->next;
        }

        const Monom64& monom() const
        {
            return *constIt_;
        }

        const Monom64& operator*() const
        {
            return *constIt_;
        }

        const Monom64* operator->() const
        {
            return constIt_;
        }

        void operator++()
        {
            constIt_ = constIt_->next;
        }

        bool operator==(const ConstIterator& a) const
        {
            return constIt_ == a.constIt_;
        }

        bool operator!=(const ConstIterator& a) const
        {
            return constIt_ != a.constIt_;
        }

    private:
        friend class Poly64;
        friend class Iterator;

        const Monom64* constIt_ = nullptr;
    };

    class Iterator
    {
    public:
        Iterator(Monom64*& it)
            : it_(&it)
        {
        }

        void operator=(const Iterator& a)
        {
            it_ = a.it_;
        }

        operator bool() const
        {
            return *it_;
        }

        bool isNext() const
        {
            return (*it_)->next;
        }

        const Monom64& monom() const
        {
            return **it_;
        }

        Monom64& operator*() const
        {
            return **it_;
        }

        Monom64* operator->() const
        {
            return *it_;
        }

        void operator++()
        {
            it_ = &(*it_)->next;
        }

        bool operator==(const Iterator& a) const
        {
            return *it_ == *a.it_;
        }

        bool operator!=(const Iterator& a) const
        {
            return *it_ != *a.it_;
        }

        void insert(const Monom64 &m)
        {
            Monom64* tmp = new Monom64(m);
            tmp->next = *it_;
            *it_ = tmp;
        }

        void insertNoCopy(Iterator another)
        {
            Monom64* tmp = *another.it_;
            *another.it_ = (*another.it_)->next;
            tmp->next = *it_;
            *it_ = tmp;
        }

        void insert(ConstIterator another)
        {
            Monom64* tmp = const_cast<Monom64*>(another.constIt_);
            tmp->next = *it_;
            *it_ = tmp;
        }

        ConstIterator get()
        {
            ConstIterator result(*it_);
            *it_ = (*it_)->next;
            Monom64* tmp = const_cast<Monom64*>(result.constIt_);
            tmp->next = nullptr;
            return result;
        }

        void del()
        {
            Monom64* tmp = *it_;
            *it_ = (*it_)->next;
            delete tmp;
        }

        void moveTo(Iterator another)
        {
            *another.it_ = *it_;
            *it_ = (*it_)->next;
            (*another.it_)->next = nullptr;
        }

        void clear()
        {
            while (*it_)
            {
                del();
            }
        }

    private:
        friend class Poly64;
        friend class ConstIterator;

        Monom64** it_ = nullptr;
    };


public:
    Poly64() = default;
    Poly64(const Poly64& a);
    Poly64(const Poly64& a, int var);
    Poly64(const Poly64& a, const Monom64& m);

    ~Poly64()
    {
        begin().clear();
    }

    void setOne();
    void setZero();
    void set(const Poly64& a);
    void operator=(const Poly64& a)
    {
        set(a);
    }

    void swap(Poly64& a);
    bool isZero() const;
    size_t length() const;

    int degree() const;
    int degreeOfMonom(int i) const;
    int deg(int var);

    const Monom64& lm() const;
    const Monom64& monom(int i) const;
    void ridOfLm();

    void add(const Monom64 &m);
    void add(const Poly64& a);
    void addNoCopy(Poly64& a);
    void sub(const Poly64& a)
    {
        add(a);
    }

    void mult(int var);
    void mult(int var, unsigned deg);
    void mult(const Monom64& m);
    void mult(const Poly64& a);

    void pow(unsigned deg);
    void reduction(const Poly64& a);
    void headReduction(const Poly64& a);

    ConstIterator begin() const;
    Iterator begin();

    friend std::ostream& operator<<(std::ostream& out, const Poly64& a);
    friend std::istream& operator>>(std::istream& in, Poly64& a);
    friend bool operator==(const Poly64& a, const Poly64& b);
    friend bool operator!=(const Poly64& a, const Poly64& b)
    {
        return !(a==b);
    }

    void assertValid(const char* fileName, int fileLine) const;

private:
    void additive(std::istream& in);
    void multiplicative(std::istream& in);
    void unary(std::istream& in);
    void power(std::istream& in);
    void bracket(std::istream& in);

private:
    Monom64* head_ = nullptr;
    size_t len_ = 0;
};

inline const Monom64& Poly64::lm() const
{
    return *head_;
}

inline const Monom64& Poly64::monom(int i) const
{
    const Monom64* r = head_;
    for (int j = 0; j < i; ++j)
    {
        r = r->next;
    }
    return *r;
}

inline size_t Poly64::length() const
{
    return len_;
}

inline bool Poly64::isZero() const
{
    return !head_;
}
