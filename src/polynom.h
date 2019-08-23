#pragma once

#include "monom.h"

#include <iostream>


class Polynom
{
public:
    class Iterator;

    class ConstIterator
    {
    public:
        ConstIterator(const Monom* constIt)
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

        const Monom& monom() const
        {
            return *constIt_;
        }

        const Monom& operator*() const
        {
            return *constIt_;
        }

        const Monom* operator->() const
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
        friend class Polynom;
        friend class Iterator;

        const Monom* constIt_ = nullptr;
    };

    class Iterator
    {
    public:
        Iterator(Monom*& it)
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

        const Monom& monom() const
        {
            return **it_;
        }

        Monom& operator*() const
        {
            return **it_;
        }

        Monom* operator->() const
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

        void insert(const Monom &m)
        {
            Monom* tmp = new Monom(m);
            tmp->next = *it_;
            *it_ = tmp;
        }

        void insertNoCopy(Iterator another)
        {
            Monom* tmp = *another.it_;
            *another.it_ = (*another.it_)->next;
            tmp->next = *it_;
            *it_ = tmp;
        }

        void insert(ConstIterator another)
        {
            Monom* tmp = const_cast<Monom*>(another.constIt_);
            tmp->next = *it_;
            *it_ = tmp;
        }

        ConstIterator get()
        {
            ConstIterator result(*it_);
            *it_ = (*it_)->next;
            Monom* tmp = const_cast<Monom*>(result.constIt_);
            tmp->next = nullptr;
            return result;
        }

        void del()
        {
            Monom* tmp = *it_;
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
        friend class Polynom;
        friend class ConstIterator;

        Monom** it_ = nullptr;
    };


public:
    Polynom() = default;
    Polynom(const Polynom& a);
    Polynom(const Polynom& a, int var);
    Polynom(const Polynom& a, const Monom& m);

    ~Polynom()
    {
        begin().clear();
    }

    void setOne();
    void setZero();
    void set(const Polynom& a);
    void operator=(const Polynom& a)
    {
        set(a);
    }

    void swap(Polynom& a);
    bool isZero() const;
    size_t length() const;

    int degree() const;
    int degreeOfMonom(int i) const;
    int deg(int var);

    const Monom& lm() const;
    const Monom& monom(int i) const;
    void ridOfLm();

    void add(const Monom &m);
    void add(const Polynom& a);
    void addNoCopy(Polynom& a);
    void sub(const Polynom& a)
    {
        add(a);
    }

    void mult(int var);
    void mult(int var, unsigned deg);
    void mult(const Monom& m);
    void mult(const Polynom& a);

    void pow(unsigned deg);
    void reduction(const Polynom& a);
    void headReduction(const Polynom& a);

    ConstIterator begin() const;
    Iterator begin();

    friend std::ostream& operator<<(std::ostream& out, const Polynom& a);
    friend std::istream& operator>>(std::istream& in, Polynom& a);
    friend bool operator==(const Polynom& a, const Polynom& b);
    friend bool operator!=(const Polynom& a, const Polynom& b)
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
    Monom* head_ = nullptr;
    size_t len_ = 0;
};

inline const Monom& Polynom::lm() const
{
    return *head_;
}

inline const Monom& Polynom::monom(int i) const
{
    const Monom* r = head_;
    for (int j = 0; j < i; ++j)
    {
        r = r->next;
    }
    return *r;
}

inline size_t Polynom::length() const
{
    return len_;
}

inline bool Polynom::isZero() const
{
    return !head_;
}
