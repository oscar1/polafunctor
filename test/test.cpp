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

typedef polacanthus::functor<void,int,std::string> foofunctor;

class Foo {
  public:
    void doit( foofunctor &fob){
        fob(1699,"Test string");
    }
};

int main(int argc,char **argv) {
  MyFunctor bla;
  double pi=3.1415926;
  polacanthus::const_setfirst_proxy<int,double,int,std::string> bla1(bla,pi);
  polacanthus::rangefilter<int> myrange(1,16); 
  polacanthus::filterfirst_proxy<int,int,std::string> bla2(bla1,myrange);
  polacanthus::void_proxy<int,int,std::string> bla3(bla2);
  Foo myfoo;
  myfoo.doit(bla3);
}
