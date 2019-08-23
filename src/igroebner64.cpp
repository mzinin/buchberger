#include "igroebner64.h"
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

    Poly64* findR(const Poly64& p, const std::vector<Poly64*>& qSet)
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

    Poly64* reduce(const Poly64& p, const std::vector<Poly64*>& qSet)
    {
        Poly64* r = new Poly64(p);
        if (qSet.empty())
        {
            return r;
        }

        Poly64* q = new Poly64();
        while (!r->isZero())
        {
            Poly64* red = findR(*r, qSet);
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

IAllocator Pair::allocator(sizeof(Pair));

IGBasis64::IGBasis64(const std::vector<Poly64*>& set)
{
    auto i1 = set.begin();
    auto i2 = basis_.begin();

    dim_ = (**i1).lm().dimIndepend();
    int i, j;

    while (i1 != set.end())
    {
        i2 = basis_.insert(i2, new Poly64(**i1));
        for (i = 0; i < dim_; ++i)
        {
            i2 = basis_.insert(i2, new Poly64(**i1));
            (**i2).mult(i);
        }
        ++i1;
    }

    reduceSet(0);
    calculateGB();
    reduceSet(0);
}

Poly64* IGBasis64::sPoly(int i, int j)
{
    if (i < dim_)
    {
        Poly64* r1 = new Poly64(*(*this)[j - dim_]);
        r1->mult(i);
        return r1;
    }

    Monom64* w = new Monom64();
    Poly64* f = (*this)[i - dim_];
    Poly64* g = (*this)[j - dim_];
    w->gcd(f->lm(), g->lm());

    Monom64* q1 = new Monom64();
    Monom64* q2 = new Monom64();
    q1->divide(f->lm(), *w);
    q2->divide(g->lm(), *w);
    delete w;

    Poly64* r1 = new Poly64(*f),
    *r2 = new Poly64(*g);
    r1->mult(*q2); delete q2;
    r2->mult(*q1); delete q1;
    r1->add(*r2); delete r2;

    return r1;
}

void IGBasis64::reduceSet(int i)
{
    std::vector<Poly64*> R, P, Q;
    std::vector<Poly64*>::iterator ir(R.begin()), ip(P.begin()), iq(Q.begin());
    std::vector<Poly64*>::const_iterator j(basis_.begin()), qEnd, rEnd, pEnd;
    Poly64 *h, *h1;

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

    int num;

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

bool IGBasis64::criterion1(int i, int j, unsigned long& lcm, int& degree)
{
    Poly64 *f,*g;
    if (i < dim_)
    {
        if (j < dim_)
        {
            return false;
        }
        else
        {
            g = (*this)[j-dim_];
            if (!g->lm().deg(i))
            {
                return false;
            }
            lcm = 1 << i;
            lcm |= g->lm().rank();
            degree = g->lm().degree() + 1;
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
            Monom64* lcmMonom = new Monom64();
            lcmMonom->lcm(f->lm(), g->lm());
            lcm = lcmMonom->rank();
            degree = lcmMonom->degree();
            delete lcmMonom;
            return true;
        }
    }
}

bool IGBasis64::criterion2(int i, int j)
{
    auto* ilist = &allPairs_[i];
    auto* jlist = &allPairs_[j];

    auto iit((*ilist).end());
    auto jit((*jlist).end());

    size_t len = length();
    Poly64* tmp;
    auto lcmMonom = std::make_unique<Monom64>((*this)[j-dim_]->lm());

    if (i>=dim_)
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

void IGBasis64::pushPoly(Poly64* p)
{
    int k = length() + dim_, degree;
    unsigned long lcm;
    std::vector<Poly64*>::iterator basisIt = basis_.begin();

    basisIt = basis_.insert(basisIt, p);

    std::vector<bool> addToAllPairs;
    std::vector<Pair*> addToPairs;
    allPairs_.push_back(addToAllPairs);

    for (auto inum = 0; inum < k; ++inum)
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

void IGBasis64::calculateGB()
{
    int k = length() + dim_, inum, jnum, degree;
    unsigned long lcm;
    Poly64 *h, *spoly;

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

std::ostream& operator<<(std::ostream& out, IGBasis64& gBasis)
{
    for (size_t i = 0; i < gBasis.length(); ++i)
    {
        out << '[' << i<< "] = " << *gBasis[i] << '\n';
    }
    return out;
}
