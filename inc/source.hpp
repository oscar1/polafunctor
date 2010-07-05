#ifndef _SOURCE_HPP
#define _SOURCE_HPP
namespace polafunctor {
  //An argument store is a (non thread safe) helper class that can be given to both an auto_first_argument and a corresponding redirected_first_argument. 
  template <typename R>
  class argumentstore: public functor<R>,public functor<void,R> {
       R mVal;
     public:
       argumentstore(R initial): mVal(initial){}
       R operator()() {
             return mVal;
       }
       void operator()(R arg){
             mVal=arg;
       }
  };

  //A const_source will return the same preset vallue each invocation.
  template <typename T>
  class const_source: public functor<T> {
       T mConst;
     public:
       const_source(T val):mConst(val){}
       T operator()() { return mConst;}
  }; 

}
#endif
