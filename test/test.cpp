#include "../inc/polafunctor.hpp"
#include <string>
#include <iostream>

class MyFunctor : public polacanthus::functor< int , double,int,std::string > {
   public:
      int operator()(double a,int b, std::string c) {
	 std::cout << "a=" << a <<", b=" << b << " c='" << c << "'" << std::endl;
         return 42;
      }
}; 

class MySequence : public polacanthus::generator<int> {
      int mSeq;
   public:
      MySequence():mSeq(-1){}
      int operator()(){ 
	      mSeq++;
	      return mSeq;
      }
};

typedef polacanthus::functor<void,int,std::string> foofunctor;

class Foo {
  public:
    void doit( foofunctor &fob){
        fob(1699,"Test string");
	fob(2,"One more test string");
	fob(3,"Can we exceed our quota?");
    }
};

int main(int argc,char **argv) {
  MyFunctor bla;
  double pi=3.1415926;
  polacanthus::const_setfirst_proxy<int,double,int,std::string> bla1(bla,pi);
  polacanthus::rangefilter<int> myrange(1,16); 
  polacanthus::filterfirst_proxy<int,int,std::string> bla2(bla1,myrange);
  polacanthus::quota onetime(2,"Foo may only invoke the functor once");
  polacanthus::statefull_proxy<int,int,std::string> bla3(bla2,onetime,0);
  polacanthus::void_proxy<int,int,std::string> bla4(bla3);
  Foo myfoo;
  std::cerr << "Invoking doit with a quota" << std::endl;
  myfoo.doit(bla4);
  MySequence sequence;
  polacanthus::statefull_setfirst_proxy<int,int,std::string> bla5(bla1,sequence);
  
}
