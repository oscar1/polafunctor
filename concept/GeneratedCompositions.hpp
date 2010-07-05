//This is untested likely incomplete code and is meant purely as an example of what
//polacastify should produce.
#include "AbstractFoo.hpp"
#include "../inc/polafunctor.hpp"

template<>
class composition<AbstactFoo>: public AbstactFoo {
    functor<void,int,std::string,char *> &mFunctor0;
    functor<int,std::string> &mFunctor1;
    functor<std::string,std::string> &mFunctor2;
  public:
    composition<AbstactFoo>(functor<void,int,std::string,char *> &f1,functor<int,std::string> &f2,functor<std::string,std::string> &f3):mFunctor0(f1),&mFunctor1(f2),mFunctor2(f3){}

    void aap(int a1, std::string a2, char * a3) {
       return mFunctor0(a1,a2,a3);
    } 

    int mies(std::string a1) {
       return mFunctor1(a1);
    }

    std::string noot(std::string a1) {
       return mFunctor2(a1);
    }
};

