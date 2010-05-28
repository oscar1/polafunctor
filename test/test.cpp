#include "../inc/polafunctor.hpp"

class MyFunctor : public polacanthus::functor< int , float > {
   public:
      int operator()(float b) {
         return 42;
      }
}; 

int main(int argc,char **argv) {
  MyFunctor bla;
  int x=bla(3.1415926);
  polacanthus::nullfunctor<int,float> NullFunctor(9);
  int y=NullFunctor(2.1994); 
  polacanthus::throwfunctor<int,float> ThrowFunctor("hoi daar");
  int z=ThrowFunctor(9.777);
}
