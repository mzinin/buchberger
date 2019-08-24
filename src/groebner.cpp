#include "groebner.h"
#include "timer.h"

#include <iostream>
#include <memory>


namespace
{
    int compareRefToPair(const Pair* a, const Pair* b)
    {
        if (a->degree != b->degree)
        {
            return (a->degree < b->degree) ? 0 : 1;
        }
        else if(a->lcm != b->lcm)
        {
            return (a->lcm > b->lcm) ? 0 : 1;
        }

        return (a->i < b->i) ? 0 : 1;
    }

    Polynom* findR(const Polynom& p, const std::vector<Polynom*>& qSet)
    {
        if (p.isZero())
        {
            return nullptr;
        }

        auto iq(qSet.begin());
        auto qEnd(qSet.end());
        const auto* plm = &p.lm();

        while (iq != qEnd)
        {
            if (plm->divisibility((**iq).lm()))
            {
                return *iq;
            }
            ++iq;
        }
        return nullptr;
    }

    Polynom* reduce(const Polynom& p, const std::vector<Polynom*>& qSet)
    {
        Polynom* r = new Polynom(p);
        if (qSet.empty())
        {
            return r;
        }

        Polynom* q = new Polynom();
        while (!r->isZero())
        {
            Polynom* red = findR(*r, qSet);
            while (red)
            {
                r->headReduction(*red);
                red = findR(*r, qSet);
            }
            if (!r->isZero())
            {
                q->add(r->lm());
                r->ridOfLm();
            }
        }

        delete r;
        return q;
    }
}

Allocator Pair::allocator(sizeof(Pair));

GroebnerBasis::GroebnerBasis(const std::vector<Polynom*>& set)
{
    auto i1 = set.begin();
    auto i2 = basis_.begin();

    dim_ = (**i1).lm().dimIndepend();

    while (i1 != set.end())
    {
        i2 = basis_.insert(i2, new Polynom(**i1));
        for (uint16_t i = 0; i < dim_; ++i)
        {
            i2 = basis_.insert(i2, new Polynom(**i1));
            (**i2).mult(i);
        }
        ++i1;
    }

    reduceSet(0);
    calculateGB();
    reduceSet(0);
}

Polynom* GroebnerBasis::sPoly(size_t i, size_t j)
{
    if (i < dim_)
    {
        Polynom* r1 = new Polynom(*(*this)[j - dim_]);
        r1->mult(static_cast<uint16_t>(i));
        return r1;
    }

    Monom* w = new Monom();
    Polynom* f = (*this)[i - dim_];
    Polynom* g = (*this)[j - dim_];
    w->gcd(f->lm(), g->lm());

    Monom* q1 = new Monom();
    Monom* q2 = new Monom();
    q1->divide(f->lm(), *w);
    q2->divide(g->lm(), *w);
    delete w;

    Polynom* r1 = new Polynom(*f),
    *r2 = new Polynom(*g);
    r1->mult(*q2); delete q2;
    r2->mult(*q1); delete q1;
    r1->add(*r2); delete r2;

    return r1;
}

void GroebnerBasis::reduceSet(int i)
{
    std::vector<Polynom*> R, P, Q;
    std::vector<Polynom*>::iterator ir(R.begin()), ip(P.begin()), iq(Q.begin());
    std::vector<Polynom*>::const_iterator j(basis_.begin()), qEnd, rEnd, pEnd;
    Polynom *h, *h1;

    if (i)
    {
        while (j != basis_.end())
        {
            ir = R.insert(ir, *j);
            ++j;
        }
    }
    else
    {
        R = basis_;
    }

    while (!R.empty())
    {
        ir = R.begin();
        h = *ir;
        ir = R.erase(ir);
        h = reduce(*h,P);

        if (!h->isZero())
        {
            auto* hlm = &h->lm();
            ip = P.begin();
            pEnd = P.end();

            while (ip != pEnd)
            {
                if ((**ip).lm().divisibility(*hlm))
                {
                    iq = Q.insert(iq, *ip);
                }
                ++ip;
            }

            // R = R U Q
            iq = Q.begin();
            qEnd = Q.end();
            while (iq != qEnd)
            {
                ir = R.begin();
                rEnd = R.end();
                while (ir != rEnd && (**ir) != (**iq))
                {
                    ++ir;
                }
                if (ir == rEnd)
                {
                    ir = R.insert(ir, *iq);
                }
                ++iq;
            }

            // P = (P-Q) U {h}
            ip = P.begin();
            while (ip != P.end())
            {
                iq = Q.begin();
                qEnd = Q.end();
                while (iq != qEnd && (**iq)!=(**ip))
                {
                    ++iq;
                }
                if (iq != qEnd)
                {
                    ip = P.erase(ip);
                }
                else
                {
                    ++ip;
                }
            }
            ip = P.insert(ip, h);
        }
    }

    R.clear();
    Q.clear();
    ir = R.begin();
    iq = Q.begin();
    ip = P.begin();
    pEnd = P.end();
    while (ip != pEnd)
    {
        iq = Q.insert(iq, *ip);
        ++ip;
    }

    ip = P.begin();
    while (ip != pEnd)
    {
        h = *ip;
        iq = Q.begin();
        while ((*h) != (**iq))
        {
            ++iq;
        }

        iq = Q.erase(iq);
        h1 = reduce(*h, Q);
        iq = Q.insert(iq, h);
        if (!h1->isZero())
        {
            ir = R.insert(ir, h1);
        }
        ++ip;
    }

    basis_ = R;
}

bool GroebnerBasis::criterion1(size_t i, size_t j, uint64_t& lcm, uint16_t& degree)
{
    Polynom *f,*g;
    if (i < dim_)
    {
        if (j < dim_)
        {
            return false;
        }
        else
        {
            g = (*this)[j-dim_];
            if (!g->lm().deg(static_cast<uint16_t>(i)))
            {
                return false;
            }
            lcm = uint64_t(1) << i;
            lcm |= g->lm().rank();
            degree = static_cast<uint16_t>(g->lm().degree() + 1);
            return true;
        }
    }
    else
    {
        f = (*this)[i-dim_];
        g = (*this)[j-dim_];

        if (!f->lm().gcd(g->lm()))
        {
            return false;
        }
        else
        {
            Monom* lcmMonom = new Monom();
            lcmMonom->lcm(f->lm(), g->lm());
            lcm = lcmMonom->rank();
            degree = lcmMonom->degree();
            delete lcmMonom;
            return true;
        }
    }
}

bool GroebnerBasis::criterion2(size_t i, size_t j)
{
    auto* ilist = &allPairs_[i];
    auto* jlist = &allPairs_[j];

    auto iit((*ilist).end());
    auto jit((*jlist).end());

    size_t len = length();
    Polynom* tmp;
    auto lcmMonom = std::make_unique<Monom>((*this)[j-dim_]->lm());

    if (i >= dim_)
    {
        lcmMonom->mult((*this)[i-dim_]->lm());
    }

    for (auto k = len + dim_ - 1; k >= dim_; --k)
    {
        --iit;
        --jit;
        if (!(*iit) && !(*jit) && k != i && k != j)
        {
            tmp = (*this)[k - dim_];
            if (lcmMonom->divisibility(tmp->lm()))
            {
                return false;
            }
        }
    }

    return true;
}

void GroebnerBasis::pushPoly(Polynom* p)
{
    size_t k = length() + dim_;
    uint16_t degree = 0;
    uint64_t lcm = 0;
    std::vector<Polynom*>::iterator basisIt = basis_.begin();

    basisIt = basis_.insert(basisIt, p);

    std::vector<bool> addToAllPairs;
    std::vector<Pair*> addToPairs;
    allPairs_.push_back(addToAllPairs);

    for (size_t inum = 0; inum < k; ++inum)
    {
        if (criterion1(inum, k, lcm, degree))
        {
            Pair* tmpPair = new Pair(inum, k, lcm, degree);
            addToPairs.push_back(tmpPair);
            allPairs_[inum].push_back(true);
            allPairs_[k].push_back(true);
        }
        else
        {
            allPairs_[inum].push_back(false);
            allPairs_[k].push_back(false);
        }
    }

    allPairs_[k].push_back(false);

    std::vector<Pair*>::iterator mid, addEnd, addBegin;
    if (!addToPairs.empty())
    {
        mid = refToPairs_.end();
        addEnd = addToPairs.end();
        addBegin = addToPairs.begin();

        std::sort(addBegin, addEnd, compareRefToPair);

        --addEnd;
        do
        {
            mid = refToPairs_.insert(mid, *addEnd);
            --addEnd;
        } while (addEnd != addBegin);

        std::inplace_merge(refToPairs_.begin(), mid, refToPairs_.end(), compareRefToPair);
    }
}

void GroebnerBasis::calculateGB()
{
    size_t k = length() + dim_, inum = 0, jnum = 0;
    uint16_t degree = 0;
    uint64_t lcm = 0;
    Polynom *h, *spoly;

    for (inum = 0; inum < k; ++inum)
    {
        std::vector<bool> k1;
        allPairs_.push_back(k1);
        for (jnum = 0; jnum < k; ++jnum)
        {
            allPairs_[inum].push_back(false);
        }
    }

    for (inum = 0; inum < k; ++inum)
    {
        for (jnum = inum + 1; jnum < k; ++jnum)
        {
            if (criterion1(inum, jnum, lcm, degree))
            {
                Pair* tmpPair = new Pair(inum, jnum, lcm, degree);
                refToPairs_.push_back(tmpPair);
                allPairs_[inum][jnum] = true;
                allPairs_[jnum][inum] = true;
            }
        }
    }

    std::sort(refToPairs_.begin(), refToPairs_.end(), compareRefToPair);

    while (!refToPairs_.empty())
    {
        selectPair(inum, jnum);
        allPairs_[inum][jnum] = false;
        allPairs_[jnum][inum] = false;

        if (criterion2(inum, jnum))
        {
            spoly = sPoly(inum, jnum);
            h = reduce(*spoly, basis_);
            delete spoly;

            if (!h->isZero())
            {
                pushPoly(h);
            }
            else
            {
                delete h;
            }
        }
    }
}

std::ostream& operator<<(std::ostream& out, GroebnerBasis& gBasis)
{
    for (size_t i = 0; i < gBasis.length(); ++i)
    {
        out << '[' << i<< "] = " << *gBasis[i] << '\n';
    }
    return out;
}
