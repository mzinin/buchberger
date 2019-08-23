#include "poly64.h"

#include <cstdarg>
#include <cstring>


Poly64::Poly64(const Poly64& a)
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

Poly64::Poly64(const Poly64& a, int var)
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
    mult(var);
}

Poly64::Poly64(const Poly64& a, const Monom64& m)
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

void Poly64::setOne()
{
    Iterator i(head_);
    i.clear();
    head_ = new Monom64();
    len_ = 1;
}

void Poly64::setZero()
{
    Iterator i(head_);
    i.clear();
    head_ = NULL;
    len_ = 0;
}

void Poly64::set(const Poly64& a)
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

void Poly64::swap(Poly64& a)
{
    Monom64 *tmp = head_;
    head_ = a.head_;
    a.head_ = tmp;

    size_t itmp = len_;
    len_ = a.len_;
    a.len_ = itmp;
}

int Poly64::degree() const
{
    return head_ ? head_->degree() : -100;
}

int Poly64::degreeOfMonom(int i) const
{
    ConstIterator cit(head_);
    for (int j = 0; j < i; ++j)
    {
        ++cit;
    }
    return cit->degree();
}

int Poly64::deg(int var)
{
    ConstIterator i(head_);
    int output = 0;

    while (i && !output)
    {
        if (output < i->deg(var))
        {
            output = i->deg(var);
        }
        ++i;
    }
    return output;
}

void Poly64::ridOfLm()
{
    Iterator i(head_);
    i.del();
    --len_;
}

void Poly64::add(const Monom64& m)
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

void Poly64::add(const Poly64& a)
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

void Poly64::addNoCopy(Poly64& a)
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

void Poly64::mult(int var)
{
    if (isZero())
    {
        return;
    }

    Poly64* tmpNoX = new Poly64();
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

void Poly64::mult(int var, unsigned deg)
{
    if (var > 0 && deg > 1)
    {
        mult(var);
    }
}

void Poly64::mult(const Monom64& m)
{
    if (isZero())
    {
        return;
    }

    for (int i = 0; i < m.dimIndepend(); ++i)
    {
        if (m.deg(i))
        {
            mult(i);
        }
    }
}

void Poly64::mult(const Poly64 &a)
{
    Poly64* p = new Poly64();
    ConstIterator ia(a.head_);

    while (ia)
    {
        Poly64* tmp = new Poly64(*this, *ia);
        p->addNoCopy(*tmp);
        delete tmp;
        ++ia;
    }

    swap(*p);
    delete p;
}

void Poly64::pow(unsigned deg)
{
    //степеней-то нет, эта функция ничего не делает
}

void Poly64::reduction(const Poly64 &a)
{
    Monom64* m2 = new Monom64();
    Poly64* p = nullptr;

    ConstIterator j(head_);
    while (j)
    {
        if (j->divisibility(a.lm()))
        {
            m2->divide(*j, a.lm());
            p = new Poly64(a);
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

    if (isZero())
    {
        return;
    }

    ConstIterator i(j);
    ++i;

    while (i)
    {
        if (i->divisibility(a.lm()))
        {
            m2->divide1(*i, a.lm());
            p = new Poly64(a);
            p->mult(*m2);
            addNoCopy(*p);
            delete p;
            i = j;
            ++i;
        }
        else
        {
            ++i;
            ++j;
        }
    }
}

void Poly64::headReduction(const Poly64 &a)
{
    Monom64* m2 = new Monom64();
    Poly64* p = nullptr;

    ConstIterator j(head_);
    while (j)
    {
        if (j->divisibility(a.lm()))
        {
            m2->divide(*j, a.lm());
            p = new Poly64(a);
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

Poly64::ConstIterator Poly64::begin() const
{
    return head_;
}

Poly64::Iterator Poly64::begin()
{
    return head_;
}

std::ostream& operator<<(std::ostream& out, const Poly64& a)
{
    if (a.isZero())
    {
        out << '0';
    }
    else
    {
        Poly64::ConstIterator i(a.begin());
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

void Poly64::additive(std::istream& in)
{
    multiplicative(in);

    int op = (in >> std::ws).peek();
    Poly64 tmp;
    while (op == '+' || op == '-')
    {
        in.get();
        tmp.multiplicative(in);
        add(tmp);
        op = in.peek();
    }
}

void Poly64::multiplicative(std::istream& in)
{
    unary(in);

    int op = (in >> std::ws).peek();
    Poly64 tmp;
    while(op == '*')
    {
        in.get();
        tmp.unary(in);
        mult(tmp);
        op = in.peek();
    }
}

void Poly64::unary(std::istream& in)
{
    int ch = (in >> std::ws).peek();
    if (ch != '+' && ch != '-')
    {
        power(in);
    }
    else
    {
        int k = 1;
        do
        {
            ch = in.get();
            if (ch == '-')
            {
                k *= -1;
            }
            ch = (in >> std::ws).peek();
        } while (ch == '+' || ch == '-');
        power(in);
    }
}

void Poly64::power(std::istream& in)
{
    bracket(in);

    int op = (in >> std::ws).peek();
    if (op == '^')
    {
        in.get();
        int deg = 0;
        in >> std::ws >> deg;
        if (in.fail() || deg <= 0)
        IMESSAGE("expected 'degree > 0'");
        pow(deg);
    }
}

void Poly64::bracket(std::istream& in)
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
        Monom64 m;
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

std::istream& operator>>(std::istream& in, Poly64& a)
{
    a.additive(in);
    return in;
}

bool operator==(const Poly64& a, const Poly64& b)
{
    if (a.len_ != b.len_)
    {
        return false;
    }

    Poly64::ConstIterator ia(a.head_), ib(b.head_);
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

void Poly64::assertValid(const char* fileName, int fileLine) const
{
    if (head_)
    {
        ConstIterator i(head_);
        IASSERT2(fileName, fileLine, i);
        ConstIterator prev(i);
        ++i;

        while (i)
        {
            IASSERT2(fileName, fileLine, i);
            IASSERT2(fileName, fileLine, (*prev).compare(*i) > 0);
            prev = i;
            ++i;
        }
    }
}
