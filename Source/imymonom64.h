#include <iostream>
#include <bitset>
#include "ivariables.h"

using namespace std;

class IMyMonom64;
class IMyMonomInterface64 {
  friend class IMyMonom64;

protected:
  int           mDimIndepend;
  IVariables*   mIndepend;

public:
  IMyMonomInterface64(IVariables* independ);
  static IMyMonomInterface64* create(IVariables* independ);
  ~IMyMonomInterface64() {}

  int dimIndepend() const { return mDimIndepend; }
  const IVariables* independ() const { return mIndepend; }

  IMyMonom64* create();
  IMyMonom64* copy(const IMyMonom64& a);
};

class IMyMonom64 {
  friend class IMyMonomInterface64;

protected:
  IMyMonom64(IMyMonomInterface64* r);
  IMyMonomInterface64* const mRealization;
  unsigned total_degree;
  int mDimIndepend,mWord;
  bitset<64> exp;

public:
  IMyMonom64 *Next;
  IMyMonom64* copy();

  IMyMonom64(const IMyMonom64& a);
  ~IMyMonom64() {Next =NULL;};

  IMyMonomInterface64* monomInterface() const { return mRealization; }
  int dimIndepend() const { return mDimIndepend; }
  const IVariables* independ() const { return mRealization->independ(); }

  unsigned deg(int var) const;
  unsigned operator[](int var) const { return deg(var); }
  unsigned degree() const;
  unsigned long rank() const;

  void set(const IMyMonom64& a);
  void operator=(const IMyMonom64& a) { set(a); }

  void setZero();
  void swap(IMyMonom64& a);
  void prolong(int var);
  void prolong(int var, unsigned deg);
  void div(int var);

  void mult(const IMyMonom64& a);
  void mult1(const IMyMonom64& a);
  void mult(const IMyMonom64& a, const IMyMonom64& b);

  bool divisibility(const IMyMonom64& a) const;
  bool divisibilityTrue(const IMyMonom64& a) const;
  void divide(const IMyMonom64& a, const IMyMonom64& b);
  void divide1(const IMyMonom64& a, const IMyMonom64& b);

  unsigned gcd(const IMyMonom64& a) const;
  unsigned lcm(const IMyMonom64& a) const;
  void gcd(const IMyMonom64& a, const IMyMonom64& b);
  void lcm(const IMyMonom64& a, const IMyMonom64& b);

  bool equality(const IMyMonom64& a, int var, unsigned degree=1) const;
  int compare(const IMyMonom64& a) const;
  int compare(const IMyMonom64& a, const IMyMonom64& b) const;

  friend std::istream& operator>>(std::istream& in, IMyMonom64& a);
  friend std::ostream& operator<<(std::ostream& out, const IMyMonom64& a);
  friend bool operator==(const IMyMonom64 &a, const IMyMonom64 &b);
  friend bool operator!=(const IMyMonom64 &a, const IMyMonom64 &b) { return !(a==b); };

  //void assertValid(const char* fileName, int fileLine) const;
};

inline unsigned IMyMonom64::deg(int var) const {
  return exp.test(var);
}

inline unsigned IMyMonom64::degree() const {
  return total_degree;
}

inline void IMyMonom64::setZero() {
  total_degree=0;
  //*exp_int=0;
  exp = 0;
}

inline void IMyMonom64::prolong(int var) {
  if ( !exp.test(var) ) {
    exp.set(var);
    total_degree++;
  };
}

inline void IMyMonom64::mult(const IMyMonom64& a) {
  exp|=a.exp;
  total_degree=exp.count();
}

inline void IMyMonom64::mult1(const IMyMonom64& a) {
  exp|=a.exp;
}

inline void IMyMonom64::mult(const IMyMonom64& a, const IMyMonom64& b) {
  exp = a.exp | b.exp;
  total_degree = exp.count();
}

inline bool IMyMonom64::divisibility(const IMyMonom64& a) const{
  bitset<64> d;
  d = exp ^ a.exp;
  d&=a.exp;
  if(d!=0)
    return false;
  else
    return true;
}

inline bool IMyMonom64::divisibilityTrue(const IMyMonom64& a) const {
  bitset<64> d;
  d = exp ^ a.exp;
  if (d==0)
    return false;
  else{
    d&=a.exp;
    if(d!=0)
      return false;
    else
      return true;
  }
}

inline void IMyMonom64::divide(const IMyMonom64& a, const IMyMonom64& b) {
  total_degree = a.total_degree - b.total_degree;
  exp = a.exp ^ b.exp;
}

inline void IMyMonom64::divide1(const IMyMonom64& a, const IMyMonom64& b) {
  exp = a.exp ^ b.exp;
}

inline unsigned IMyMonom64::gcd(const IMyMonom64& a) const {
  bitset<64> d;
  d = exp & a.exp;
  return d.count();
}

inline unsigned IMyMonom64::lcm(const IMyMonom64& a) const {
  bitset<64> d;
  d = exp | a.exp;
  return d.count();
}

inline void IMyMonom64::gcd(const IMyMonom64& a, const IMyMonom64& b) {
  exp = a.exp & b.exp;
  total_degree = exp.count();
}

inline void IMyMonom64::lcm(const IMyMonom64& a, const IMyMonom64& b) {
  exp = a.exp | b.exp;
  total_degree = exp.count();
}


inline int IMyMonom64::compare(const IMyMonom64& a) const {
  if (total_degree<a.total_degree)
    return -1;
  else if (total_degree>a.total_degree)
    return 1;
  else {
    unsigned long *exp_int = (unsigned long*)&exp;
    unsigned long *exp_int_a = (unsigned long*)&a.exp;
    if (*exp_int<*exp_int_a)
      return 1;
    else if (*exp_int>*exp_int_a)
      return -1;
    else
      return 0;
  }
}

inline bool operator==(const IMyMonom64 &a, const IMyMonom64 &b){
  return a.exp==b.exp;
}

inline unsigned long IMyMonom64::rank() const {
    //unsigned r = total_degree;
    //unsigned long *exp_int = (unsigned long*)&exp;
    //r = r<<mDimIndepend;
    //r-=*exp_int;
    //return r;
    unsigned long r = *(unsigned long*)&exp;
    return r;
}
