#include <iostream>
#include "igroebner64.h"
#include "itimer.h"

using namespace std;

int Compare_ref_to_Pair(Pair* a,  Pair* b){
  if (a->degree!=b->degree)
    if (a->degree<b->degree)
      return 1;
    else
      return 0;
  else if(a->lcm!=b->lcm)
    if (a->lcm>b->lcm)
      return 1;
    else
      return 0;
  else if (a->i<b->i)
         return 1;
       else
         return 0;
}

IMyPoly64* IGBasis64::S(int i, int j){
  if (i<Dim){
    IMyPoly64 *r1 = pInterface_local->copy(*(*this)[j-Dim]);
    r1->mult(i);
    return r1;
  }
  else{
    IMyMonom64 *w(mInterface_local->create());
    IMyPoly64 *f,*g;
    f = (*this)[i-Dim];
    g = (*this)[j-Dim];
    w->gcd(f->lm(),g->lm());

    IMyMonom64 *q1(mInterface_local->create()), *q2(mInterface_local->create());
    q1->divide(f->lm(),*w);
    q2->divide(g->lm(),*w);
    delete w;

    IMyPoly64 *r1 = pInterface_local->copy(*f),
              *r2 = pInterface_local->copy(*g);
    r1->mult(*q2); delete q2;
    r2->mult(*q1); delete q1;
    r1->add(*r2); delete r2;

    return r1;
  }
}

IGBasis64::IGBasis64():
  basis(), vars(), mInterface_local(NULL), pInterface_local(NULL) {}

IMyPoly64* findR(IMyPoly64& p, vector<IMyPoly64*> &Q){
  if (p.isZero()) return NULL;
  vector<IMyPoly64*>::const_iterator iq(Q.begin()), q_end(Q.end());
  IMyMonom64 *plm = (IMyMonom64*)&p.lm();

  while (iq!=q_end){
    if ( plm->divisibility((**iq).lm()) )
      return *iq;
    iq++;
  }

  return NULL;
}

IMyPoly64* Reduce(IMyPoly64 &p, vector<IMyPoly64*> &Q){
  IMyPoly64 *r,*q;
  IMyPoly64 *red;
  q = p.polyInterface()->create();
  q->setZero();
  r = p.polyInterface()->copy(p);
  if (Q.empty()) return r;
  vector<IMyPoly64*> rrq;
  vector<IMyPoly64*>::iterator it;

  while (!r->isZero()){
    red = findR(*r,Q);
    while (red){
      r->reduction(*red);
      red = findR(*r,Q);
    }
    if (!r->isZero()){
      q->add(r->lm());
      r->rid_of_lm();
    }
  }

  delete r;
  return q;
}

void IGBasis64::ReduceSet(int i) {
  vector<IMyPoly64*> R,P,Q;
  vector<IMyPoly64*>::iterator ir(R.begin()), ip(P.begin()), iq(Q.begin());
  vector<IMyPoly64*>::const_iterator j(basis.begin()), q_end, r_end, p_end;
  IMyPoly64 *h,*h1;
  if (i)
    while (j!=basis.end()){
      ir=R.insert(ir,*j);
      ++j;
    }
  else
    R = basis;

  int num;

  while (!R.empty()){
    ir = R.begin();
    h = *ir;
    ir = R.erase(ir);
    h = Reduce(*h,P);
    if (!h->isZero()){
      IMyMonom64 *hlm=(IMyMonom64*)&h->lm();
      ip = P.begin();
      p_end = P.end();
      while (ip!=p_end){//P.end()){
        if ((**ip).lm().divisibility(*hlm))
          iq=Q.insert(iq,*ip);
	++ip;
      }
      //R = R U Q
      iq = Q.begin();
      q_end = Q.end();
      while (iq!=q_end){
        ir = R.begin();
        r_end = R.end();
        while (ir!=r_end && (**ir)!=(**iq))
	  ++ir;
        if (ir==r_end)
          ir=R.insert(ir,*iq);
	++iq;
      }
      //P = (P-Q) U {h}
      ip = P.begin();
      while (ip!=P.end()) {
        iq = Q.begin();
        q_end = Q.end();
        while (iq!=q_end && (**iq)!=(**ip))
	  ++iq;
        if (iq!=q_end) ip=P.erase(ip);
	else ++ip;
      }
      ip=P.insert(ip,h);
    }
  }

  R.clear();
  iq = Q.begin();
  ip = P.begin();
  p_end = P.end();
  while (ip!=p_end){
    iq=Q.insert(iq,*ip);
    ++ip;
  }
  ip = P.begin();
  while (ip!=p_end){
    h = *ip;
    iq = Q.begin();
    while ((*h)!=(**iq)) ++iq;
    iq=Q.erase(iq);
    h1 = Reduce(*h,Q);
    iq=Q.insert(iq,h);
    if (!h1->isZero())
      ir=R.insert(ir,h1);
    ++ip;
  }
  basis.clear();
  basis = R;
}

bool IGBasis64::criterion1(int i, int j, unsigned long &lcm, int &degree){
  IMyPoly64 *f,*g;
  if (i<Dim)
    if (j<Dim)
      return false;
    else{
      g = (*this)[j-Dim];
      if (!g->lm().deg(i))
        return false;
      lcm = g->lm().rank();
      degree = g->lm().degree() + 1;
      return true;
    }
  else{
    f = (*this)[i-Dim];
    g = (*this)[j-Dim];

    if ( !f->lm().gcd(g->lm()) ){
      return false;
    }
    else{
      IMyMonom64 *lcm_monom(mInterface_local->create());
      lcm_monom->lcm(f->lm(),g->lm());
      lcm = lcm_monom->rank();
      degree = lcm_monom->degree();
      delete lcm_monom;
      return true;
    }
  }
}

bool IGBasis64::criterion2(int i, int j){
  vector<bool> *ilist(&all_pairs[i]), *jlist=(&all_pairs[j]);
  vector<bool>::const_iterator iit((*ilist).end()), jit((*jlist).end());
  int k,len=length();
  IMyPoly64 *tmp;
  IMyMonom64 *lcm_monom(mInterface_local->copy((*this)[j-Dim]->lm()));
  if (i>=Dim)
    lcm_monom->mult((*this)[i-Dim]->lm());

  for (k=len+Dim-1; k>=Dim; k--){
    iit--;
    jit--;
    if (!(*iit) && !(*jit) && k!=i && k!=j){
      tmp = (*this)[k-Dim];
      if (lcm_monom->divisibility(tmp->lm()))
	return false;
    }
  }
  return true;
}

vector<Pair*>::iterator p_iterator;
vector<Pair*>::const_iterator p_end;

void ShowPairs(vector<Pair*>& plist){
  p_iterator = plist.begin();
  while (p_iterator!=plist.end()){
    cout<<'('<<(*p_iterator)->i<<','<<(*p_iterator)->j<<") ";
    p_iterator++;
  }
  cout<<endl<<endl;
}

void SelectPair(vector<Pair*>& plist, int& i, int& j){
  p_iterator = plist.begin();
  i = (*p_iterator)->i;
  j = (*p_iterator)->j;
  plist.erase(p_iterator);

  return;
}

void IGBasis64::push_poly(IMyPoly64* p,int flag){
  int inum, jnum, k = length() + Dim, degree;
  unsigned long lcm;
  vector<IMyPoly64*>::iterator basisIt(basis.begin());
  vector<Pair*>::iterator mid, add_end, add_begin;

  basisIt=basis.insert(basisIt, p);

  vector<bool> add_to_all_pairs;
  vector<Pair*> add_to_pairs;
  all_pairs.push_back(add_to_all_pairs);

  for (inum=0;inum<k;inum++)
    if (criterion1(inum,k,lcm,degree)){
      Pair *tmpPair = new Pair(inum,k,lcm,degree);
      add_to_pairs.push_back(tmpPair);
      all_pairs[inum].push_back(true);
      all_pairs[k].push_back(true);
    }
    else{
      all_pairs[inum].push_back(false);
      all_pairs[k].push_back(false);
    }

  all_pairs[k].push_back(false);

  if (!add_to_pairs.empty()){
    mid = ref_to_pairs.end(); add_end = add_to_pairs.end(); add_begin = add_to_pairs.begin();
    sort(add_begin,add_end,Compare_ref_to_Pair);
    add_end--;
    do{
      mid = ref_to_pairs.insert(mid, *add_end);
      add_end--;
    } while (add_end!=add_begin);
    inplace_merge(ref_to_pairs.begin(), mid, ref_to_pairs.end(), Compare_ref_to_Pair);
  }
}

void IGBasis64::CalculateGB(){

  int k = length() + Dim, inum, jnum, degree;
  unsigned long lcm;
  IMyPoly64 *h, *spoly;

  for (inum=0; inum<k; inum++){
    vector<bool> k1;
    all_pairs.push_back(k1);
    for (jnum=0; jnum<k; jnum++)
      all_pairs[inum].push_back(false);
  }

  for (inum=0; inum<k; inum++)
  for (jnum=inum+1; jnum<k; jnum++)
      if (criterion1(inum,jnum,lcm,degree)){
	Pair *tmpPair = new Pair(inum, jnum, lcm, degree);
	ref_to_pairs.push_back(tmpPair);
	all_pairs[inum][jnum]=true;
	all_pairs[jnum][inum]=true;
      }

  sort(ref_to_pairs.begin(),ref_to_pairs.end(),Compare_ref_to_Pair);

  while (!ref_to_pairs.empty()){
    //cout<<*this;ShowPairs(ref_to_pairs);
    SelectPair(ref_to_pairs,inum,jnum);
    all_pairs[inum][jnum]=false;
    all_pairs[jnum][inum]=false;

    if (criterion2(inum,jnum)){
      spoly = S(inum,jnum);
      h = Reduce(*spoly,basis);
      delete spoly;

      if (!h->isZero())
        push_poly(h,1);
      else
        delete h;
    }
  }
}

IGBasis64::IGBasis64(vector<IMyPoly64*> set):
  basis(), vars(), mInterface_local(NULL), pInterface_local(NULL) {

  vector<IMyPoly64*>::const_iterator i1(set.begin());
  vector<IMyPoly64*>::iterator i2(basis.begin());

  mInterface_local = (**i1).monomInterface();
  pInterface_local = (**i1).polyInterface();
  Dim = mInterface_local->dimIndepend();
  int i;
  IMyPoly64 *tmp;

  while (i1!=set.end()){
    i2=basis.insert(i2, pInterface_local->copy(**i1));

    for (i = 0; i < Dim; i++){
      i2=basis.insert(i2, pInterface_local->copy(**i1));
      (**i2).mult(i);
    }
    ++i1;
  }

  ReduceSet(0);
  CalculateGB();
  ReduceSet(0);
}

IMyPoly64* IGBasis64::operator[](int num){
  //IASSERT(0<=num && num<length());
  vector<IMyPoly64*>::const_iterator it(basis.begin());
  it+=length()-1-num;
  return *it;
}

int IGBasis64::length(){
  return basis.size();
}

std::ostream& operator<<(std::ostream& out, IGBasis64& GBasis) {
  int i=0;
  for (i=0;i<GBasis.length();i++)
    out<<'['<<i<<"] = "<<*GBasis[i]<<'\n';

  return out;
}
