#include "polynom.h"

#include <cstdarg>
#include <cstring>


Polynom::Polynom(const Polynom& a)
    : len_(a.len_)
{
    ConstIterator ia(a.head_);
    Iterator i(head_);

    while (ia)
    {
        i.insert(*ia);
        ++ia;
        ++i;
    }
}

Polynom::Polynom(const Polynom& a, const Monom& m)
    : len_(a.len_)
{
    ConstIterator ia(a.head_);
    Iterator i(head_);

    while (ia)
    {
        i.insert(*ia);
        ++ia;
        ++i;
    }
    mult(m);
}

void Polynom::setOne()
{
    Iterator i(head_);
    i.clear();
    head_ = new Monom();
    len_ = 1;
}

void Polynom::set(const Polynom& a)
{
    Iterator i(head_);
    i.clear();

    ConstIterator ia(a.head_);
    while (ia)
    {
        i.insert(*ia);
        ++i;
        ++ia;
    }
    len_ = a.len_;
}

void Polynom::swap(Polynom& a)
{
    Monom* tmp = head_;
    head_ = a.head_;
    a.head_ = tmp;

    size_t itmp = len_;
    len_ = a.len_;
    a.len_ = itmp;
}

void Polynom::ridOfLm()
{
    Iterator i(head_);
    i.del();
    --len_;
}

void Polynom::add(const Monom& m)
{
    Iterator i(head_);
    bool placed = false;

    while (i && !placed)
    {
        switch (i->compare(m))
        {
        case 1:
            ++i;
            break;

        case -1:
            i.insert(m);
            placed = true;
            ++len_;
            break;

        case 0:
            i.del();
            placed = true;
            --len_;
            break;
        }
    }

    if (!placed)
    {
        i.insert(m);
        ++len_;
    }
}

void Polynom::add(const Polynom& a)
{
    Iterator i1(head_);
    ConstIterator i2(a.head_);

    while (i1 && i2)
    {
        switch (i1->compare(*i2))
        {
        case 1:
            ++i1;
            break;

        case -1:
            i1.insert(*i2);
            ++i2;
            ++i1;
            ++len_;
            break;

        case 0:
            i1.del();
            ++i2;
            --len_;
            break;
        }
    }

    while (i2)
    {
        i1.insert(*i2);
        ++i1;
        ++i2;
        ++len_;
    }
}

void Polynom::addNoCopy(Polynom& a)
{
    Iterator i1(head_);
    Iterator i2(a.head_);

    while (i1 && i2)
    {
        switch (i1->compare(*i2))
        {
        case 1:
            ++i1;
            break;

        case -1:
            i1.insertNoCopy(i2);
            ++i1;
            ++len_;
            break;

        case 0:
            i1.del();
            ++i2;
            --len_;
            break;
        }
    }

    while (i2)
    {
        i1.insertNoCopy(i2);
        ++i1;
        ++len_;
    }
}

void Polynom::mult(uint16_t var)
{
    if (isZero())
    {
        return;
    }

    Polynom* tmpNoX = new Polynom();
    Iterator it(head_);
    Iterator itNoX(tmpNoX->head_);

    while (it)
    {
        if (!it->deg(var))
        {
            it.moveTo(itNoX);
            ++itNoX;
            --len_;
        }
        else
        {
            ++it;
        }
    }

    itNoX = tmpNoX->begin();
    while (itNoX)
    {
        itNoX->prolong(var);
        ++itNoX;
    }

    addNoCopy(*tmpNoX);
    delete tmpNoX;
}

void Polynom::mult(const Monom& m)
{
    if (isZero())
    {
        return;
    }

    for (uint16_t i = 0; i < m.dimIndepend(); ++i)
    {
        if (m.deg(i))
        {
            mult(i);
        }
    }
}

void Polynom::mult(const Polynom& a)
{
    Polynom* p = new Polynom();
    ConstIterator ia(a.head_);

    while (ia)
    {
        Polynom* tmp = new Polynom(*this, *ia);
        p->addNoCopy(*tmp);
        delete tmp;
        ++ia;
    }

    swap(*p);
    delete p;
}

void Polynom::pow(uint16_t)
{
    //степеней-то нет, эта функция ничего не делает
}

void Polynom::headReduction(const Polynom& a)
{
    Monom* m2 = new Monom();
    Polynom* p = nullptr;

    ConstIterator j(head_);
    while (j)
    {
        if (j->divisibility(a.lm()))
        {
            m2->divide(*j, a.lm());
            p = new Polynom(a);
            p->mult(*m2);
            addNoCopy(*p);
            delete p;
            j.constIt_ = head_;
        }
        else
        {
            break;
        }
    }
}

Polynom::ConstIterator Polynom::begin() const
{
    return head_;
}

Polynom::Iterator Polynom::begin()
{
    return head_;
}

std::ostream& operator<<(std::ostream& out, const Polynom& a)
{
    if (a.isZero())
    {
        out << '0';
    }
    else
    {
        Polynom::ConstIterator i(a.begin());
        if ((*i).degree())
        {
            out << *i;
        }
        else
        {
            out << '1';
        }

        ++i;
        while(i)
        {
            out << " + ";
            if ((*i).degree())
            {
                out << *i;
            }
            else
            {
                out << '1';
            }
            ++i;
        }
    }
    return out;
}

void Polynom::additive(std::istream& in)
{
    multiplicative(in);

    int op = (in >> std::ws).peek();
    Polynom tmp;
    while (op == '+' || op == '-')
    {
        in.get();
        tmp.multiplicative(in);
        add(tmp);
        op = in.peek();
    }
}

void Polynom::multiplicative(std::istream& in)
{
    unary(in);

    int op = (in >> std::ws).peek();
    Polynom tmp;
    while(op == '*')
    {
        in.get();
        tmp.unary(in);
        mult(tmp);
        op = in.peek();
    }
}

void Polynom::unary(std::istream& in)
{
    int ch = (in >> std::ws).peek();
    if (ch != '+' && ch != '-')
    {
        power(in);
    }
    else
    {
        // int k = 1;
        do
        {
            ch = in.get();
            // if (ch == '-')
            // {
            //     k *= -1;
            // }
            ch = (in >> std::ws).peek();
        } while (ch == '+' || ch == '-');
        power(in);
    }
}

void Polynom::power(std::istream& in)
{
    bracket(in);

    int op = (in >> std::ws).peek();
    if (op == '^')
    {
        in.get();
        int deg = 0;
        in >> std::ws >> deg;
        if (in.fail() || deg <= 0)
        {
            IMESSAGE("expected 'degree > 0'");
        }
        pow(static_cast<uint16_t>(deg));
    }
}

void Polynom::bracket(std::istream& in)
{
    int op = (in >> std::ws).peek();
    if (op == '(')
    {
        in.get();
        additive(in);
        if (in.get() != ')')
        {
            IMESSAGE("expected ')'");
        }
    }
    else
    {
        setOne();
        Monom m;
        in >> m;
        if (!in.fail())
        {
            mult(m);
        }
        else
        {
            in.clear();
            if (in.get() != '1')
            {
                IMESSAGE("expected 'monomial'");
            };
        }
    }
}

std::istream& operator>>(std::istream& in, Polynom& a)
{
    a.additive(in);
    return in;
}

bool operator==(const Polynom& a, const Polynom& b)
{
    if (a.len_ != b.len_)
    {
        return false;
    }

    Polynom::ConstIterator ia(a.head_), ib(b.head_);
    while (ia && ib)
    {
        if (*ia != *ib)
        {
            return false;
        }
        ++ia;
        ++ib;
    }

    return !ia && !ib;
}
