//This is untested likely incomplete code and is meant purely as an example of what
//polacastify should produce.
#include "AbstractFoo.hpp"
#include "../inc/polafunctor.hpp"

template<>
class decomposable<AbstactFoo> {
  public:
    class functortype0: public functor<void,int,std::string,char *> {
         AbstactFoo &mRaw;
       public:
         functortype0(AbstactFoo &raw):mRaw(raw){}
         void operator()(int a1, std::string a2, char * a3) {
            return mRaw.aap(a1,a2,a3);
	 }	 
    };
    class functortype1: public functor<std::string,std::string> {
         AbstactFoo &mRaw;
       public:
	 functortype1(AbstactFoo &raw):mRaw(raw){}
	 std::string operator()(std::string a1) {
            return mRaw.noot(a1);
	 }
    };
    class functortype2: public functor<int,std::string> {
         AbstactFoo &mRaw;
      public:
	 functortype2(AbstactFoo &raw):mRaw(raw){}
	 int operator()(std::string a1) {
            return mRaw.mies(a1);
	 }
    };
    functortype0 aap;
    functortype1 noot;
    functortype2 mies;
    decomposable<AbstactFoo>(AbstactFoo &raw): aap(raw),noot(raw),mies(raw){}
};

