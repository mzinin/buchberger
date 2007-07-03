#include <iostream>
#include <fstream>
#include "itimer.h"
#include "igroebner64.h"

using namespace std;

void init64(char* filename){
  ifstream fin(filename);
  ITimer timer;

  IVariables vars;
  //-----чтение переменных
  int i=0;
  char s[524288],c='0';
  while ( c!=';' ){
  	fin>>c;
	if (c==',' || c==';') {
		vars.add(s);
		s[0]='\0';
		i=0;
	}
	else {
		s[i]=c;
		i++;
		s[i]='\0';
	}
  }
  //-----конец чтения переменных

  IMyMonomInterface64 mInterface = IMyMonomInterface64(&vars);
  IMyPolyInterface64 *pInterface = IMyPolyInterface64::create(&mInterface);

  IMyPoly64 *poly = pInterface->create();
  vector<IMyPoly64*> pl, answer;
  vector<IMyPoly64*>::iterator it(pl.begin()), an_it(answer.begin());

  i=0;
  s[0]='\0';
  c='0';

  ifstream tmp_in,tmp_in2;
  ofstream tmp_out;
  //-----чтение исходного базиса
  while (c!=';'){
    fin>>c;
    if (c==',' || c==';') {s[i]='\n';i++;s[i]='\0';}
    else {s[i]=c;i++;s[i]='\0';}
  }
  tmp_out.open("tmp1");
  tmp_out<<s;
  tmp_out.close();

  tmp_in.open("tmp1");
  while (!tmp_in.eof()){
    tmp_in>>*poly;
    it=pl.insert(it, pInterface->copy(*poly));
  }
  tmp_in.close();
  //-----конец чтения исходного базиса

  i=0;
  s[0]='\0';
  c='0';

  //-----чтение ответа
  while (c!=';'){
    fin>>c;
    if (c==',' || c==';') {s[i]='\n';i++;s[i]='\0';}
    else {s[i]=c;i++;s[i]='\0';}
  }
  tmp_out.open("tmp2");
  tmp_out<<s;
  tmp_out.close();

  tmp_in2.open("tmp2");
  while (!tmp_in2.eof()){
    tmp_in2>>*poly;
    an_it=answer.insert(an_it, pInterface->copy(*poly));
  }
  tmp_in2.close();
  //-----конец чтения ответа
  fin.close();

  timer.start();
  IGBasis64 bg(pl);
  timer.stop();
  //cout<<bg<<endl;
  cout<<timer<<endl;

  bool Is_Correct=true,Found;
  //-----проверка
  if (bg.length()<answer.size())
    Is_Correct=false;
  else{
    an_it=answer.begin();
    while (an_it!=answer.end()) {
      Found=false;
      for (i=0;i<bg.length();i++){
        if (*bg[i]==**an_it){
          Found=true;
	  break;
        }
      }
      if (!Found) {
        Is_Correct=false;
	cout<<"The mistake is here "<<**an_it<<endl;
        break;
      }
      ++an_it;
    }
  }
  //-----конец проверки
  if (Is_Correct)
    cout<<"The answer is CORRECT"<<endl;
  else
    cout<<"The answer is WRONG"<<endl;

  return;
}
