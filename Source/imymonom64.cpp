#include <cstdarg>
#include <cmath>
#include "iutil.h"
#include "imymonom64.h"

IMyMonomInterface64::IMyMonomInterface64(IVariables* independ){
  mDimIndepend = independ->dim();
  mIndepend = independ;
}

IMyMonomInterface64* IMyMonomInterface64::create(IVariables* independ) {
  IMyMonomInterface64* r = new IMyMonomInterface64(independ);
  return r;
}

IMyMonom64* IMyMonomInterface64::create() {
  return new IMyMonom64(this);
}

IMyMonom64* IMyMonomInterface64::copy(const IMyMonom64 &a) {
  IMyMonom64* r;
  if (this == a.monomInterface())
    r = new IMyMonom64(a);

  return r;
}

IMyMonom64::IMyMonom64(IMyMonomInterface64* r):
  mRealization(r),
  mDimIndepend(r->dimIndepend()),mWord((mDimIndepend-1)/32),exp(),Next(NULL){
      IASSERT(mRealization);
      total_degree = 0;
  }

IMyMonom64::IMyMonom64(const IMyMonom64& a):
  mRealization(a.mRealization),
  mDimIndepend(a.mDimIndepend),mWord((mDimIndepend-1)/32),exp(),Next(NULL){
    total_degree = a.total_degree;
    exp = a.exp;
  }

IMyMonom64* IMyMonom64::copy(){
  IMyMonom64* r = new IMyMonom64(mRealization);
  r->total_degree = total_degree;
  r->exp = exp;
  return r;
}

void IMyMonom64::set(const IMyMonom64& a) {
  if (monomInterface() == a.monomInterface()) {
    total_degree = a.total_degree;
    exp = a.exp;
  }
}

void IMyMonom64::swap(IMyMonom64& a) {
  unsigned t;
  t = total_degree;
  total_degree = a.total_degree;
  a.total_degree = t;

  bitset<64> d = exp;
  exp = a.exp;
  a.exp = d;
}

void IMyMonom64::prolong(int var, unsigned deg) {
  if (deg>0) prolong(var);
}

void IMyMonom64::div(int var) {
  if ( exp.test(var) ){
    exp.reset(var);
    total_degree--;
  }
  else
   IERROR("Monom can't be divided by variable");
}

bool IMyMonom64::equality(const IMyMonom64& a, int var, unsigned degree) const {
  bitset<64> d=1;
  d=d<<var;
  d|=a.exp;
  return exp==d;
}

int IMyMonom64::compare(const IMyMonom64& a, const IMyMonom64& b) const {
  IMyMonom64 tmp(a);
  tmp.mult(b);
  return compare(tmp);
}

std::istream& operator>>(std::istream& in, IMyMonom64& a) {
  std::streampos posbeg = in.tellg();
  int var = a.independ()->read(in);
  if (var < 0) {
    in.clear();
    in.setstate(std::ios::failbit);
  }
  else {
    a.setZero();
    int deg;
    do {
      deg = 1;
      std::streampos posbeg = in.tellg();
      if ((in >> std::ws).peek() == '^') {
        in.get();
        in >> std::ws >> deg;
        if (in.fail() || deg < 0) {
          in.setstate(std::ios::failbit);
          IMESSAGE("expected 'degree >= 0'");
        }
      }
      a.prolong(var,deg);

      posbeg = in.tellg();
      if (in.peek() != '*')
        var = -1;
      else {
        in.get();
        var = a.independ()->read(in);
        if (var < 0) {
          in.clear();
          in.seekg(posbeg);
        }
      }
    } while(var >= 0);
    if (in.eof() &&
        deg >= 0)
      in.clear();
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const IMyMonom64& a) {
  if (a.degree() == 0)
    out << '1';
  else {
    int i = 0;
    IVariables::ConstIterator j(a.independ()->begin());
    while(a.deg(i) == 0) {
      ++i;
      ++j;
    }
    out << *j;
    if (a.deg(i) > 1)
      out << '^' << a[i];
    ++i;
    ++j;
    while(j!=a.independ()->end()){
      if (a.deg(i)) {
        out << '*' << *j;
        if (a.deg(i) > 1)
          out << '^' << a[i];
      }
      ++i;
      ++j;
    }
  }
  return out;
}
