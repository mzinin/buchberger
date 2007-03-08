#include <vector>
#include <algorithm>
#include "imypoly64.h"

struct Pair {
  int i;
  int j;
  unsigned long lcm_degree;

  Pair() {}
  Pair(int i_new, int j_new, unsigned deg_new){
    i = i_new; j = j_new; lcm_degree = deg_new;
  }
  ~Pair() {}
};

//IMyPoly* S(IMyPoly& f,IMyPoly& g);
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

public:
  IGBasis64();
  IGBasis64(vector<IMyPoly64*> set);
  ~IGBasis64() {}

  IMyPoly64* operator[](int num);
  int length();
  void ReduceSet(int i);
  void CalculateGB();
  void Check();
  void push_poly(IMyPoly64* p,int flag);
  IMyMonomInterface64* monomInterface() const {return mInterface_local;}
  IMyPolyInterface64*  polyInterface()  const {return pInterface_local;}

  bool criterion1(IMyPoly64 &pi, IMyPoly64 &pj, unsigned long &lcm_degree, int i);
  bool criterion2(int i, int j, IMyPoly64& ipoly, IMyPoly64& jpoly);

  friend std::ostream& operator<<(std::ostream& out, IGBasis64& GBasis);
};