#include <vector>
#include <algorithm>
#include "imypoly64.h"

struct Pair {
  int i;
  int j;
  unsigned long lcm;
  int degree;

  Pair() {}
  Pair(int i_new, int j_new, unsigned lcm_new, int deg_new){
    i = i_new; j = j_new; lcm = lcm_new; degree = deg_new;
  }
  ~Pair() {}
};

IMyPoly64* findR(IMyPoly64& p, vector<IMyPoly64*> &Q);
IMyPoly64* Reduce(IMyPoly64& p, vector<IMyPoly64*> &Q);

class IGBasis64{
protected:
  vector<IMyPoly64*> basis;
  IVariables vars;
  IMyMonomInterface64* mInterface_local;
  IMyPolyInterface64* pInterface_local;
  int Dim;
  vector< vector<bool> > all_pairs;
  vector<Pair*> ref_to_pairs;

  bool criterion1(int i, int j, unsigned long &lcm, int &degree);
  bool criterion2(int i, int j);
  void push_poly(IMyPoly64* p,int flag);
  void CalculateGB();

public:
  IGBasis64();
  IGBasis64(vector<IMyPoly64*> set);
  ~IGBasis64() {}

  IMyPoly64* operator[](int num);
  IMyPoly64* S(int i,int j);
  int length();
  void ReduceSet(int i);

  IMyMonomInterface64* monomInterface() const {return mInterface_local;}
  IMyPolyInterface64*  polyInterface()  const {return pInterface_local;}

  friend std::ostream& operator<<(std::ostream& out, IGBasis64& GBasis);
};
