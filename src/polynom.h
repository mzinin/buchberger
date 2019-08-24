#pragma once

#include "monom.h"

#include <cstdint>
#include <iostream>


class Polynom
{
private:
    class Iterator;

    class ConstIterator
    {
    public:
        ConstIterator(const Monom* constIt)
            : constIt_(constIt)
        {
        }

        operator bool() const
        {
            return constIt_;
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

    private:
        friend class Polynom;

        const Monom* constIt_ = nullptr;
    };

    class Iterator
    {
    public:
        Iterator(Monom*& it)
            : it_(&it)
        {
        }

        operator bool() const
        {
            return *it_;
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

        Monom** it_ = nullptr;
    };


public:
    Polynom() = default;
    Polynom(const Polynom& a);
    Polynom(const Polynom& a, const Monom& m);

    ~Polynom()
    {
        begin().clear();
    }

    void setOne();
    void set(const Polynom& a);
    void operator=(const Polynom& a)
    {
        set(a);
    }

    void swap(Polynom& a);
    bool isZero() const;

    const Monom& lm() const;
    void ridOfLm();

    void add(const Monom& m);
    void add(const Polynom& a);
    void addNoCopy(Polynom& a);

    void mult(uint16_t var);
    void mult(const Monom& m);
    void mult(const Polynom& a);

    void pow(uint16_t deg);
    void headReduction(const Polynom& a);

    friend std::ostream& operator<<(std::ostream& out, const Polynom& a);
    friend std::istream& operator>>(std::istream& in, Polynom& a);
    friend bool operator==(const Polynom& a, const Polynom& b);
    friend bool operator!=(const Polynom& a, const Polynom& b)
    {
        return !(a == b);
    }

private:
    ConstIterator begin() const;
    Iterator begin();

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

inline bool Polynom::isZero() const
{
    return !head_;
}
