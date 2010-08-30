//Experimental untested stuff. Ignore for now, not even sure if this will compile.
#ifndef _EVENT_HPP
#define _EVENT_HPP
namespace polafunctor {
  template <typename R>
  class delayed_invocation: public functor<void> {
       functor<void,R> &mRaw;
       R mVal;
     public:
       delayed_invocation(functor<void,R> &raw,R val):mRaw(raw),mVal(val){}
       void operator()() {
          mRaw(mVal);
       }
  };

  template <typename R, typename ... Args>
  class delayed_invocation: public functor<void> {
       const_source<R> mSource;
       auto_first_argument<R,Args...> mAutoFirst;
       delayed_invocation<Args...> mEvent;
     public:
       delayed_invocation(functor<void,R,Args...> &raw,R first,Args...args):mSource(first),mAutoFirst(raw,mSource),mEvent(mAutoFirst,args...){}
       void operator()() {
         mEvent();
       }
  };
}
#endif
