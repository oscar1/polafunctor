//Experimental untested stuff. Ignore for now, not even sure if this will compile.
#ifndef _EVENT_HPP
#define _EVENT_HPP
#include "functor.hpp"
#include "source.hpp"
#include "proxy.hpp"
namespace polafunctor {
  template <typename R>
  class asynchonous_invocation: public functor<void> {
       functor<void,R> &mRaw;
       R mVal;
     public:
       asynchonous_invocation(functor<void,R> &raw,R val):mRaw(raw),mVal(val){}
       void operator()() {
          mRaw(mVal);
       }
  };

  template <typename R, typename ... Args>
  class asynchonous_invocation: public functor<void> {
       const_source<R> mSource;
       auto_first_argument<R,Args...> mAutoFirst;
       asynchonous_invocation<Args...> mEvent;
     public:
       asynchonous_invocation(functor<void,R,Args...> &raw,R first,Args...args):mSource(first),mAutoFirst(raw,mSource),mEvent(mAutoFirst,args...){}
       void operator()() {
         mEvent();
       }
  };
}
#endif
