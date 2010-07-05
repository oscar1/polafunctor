#ifndef _PROXY_HPP
#define _PROXY_HPP
#include "functor.hpp"
namespace polafunctor {

  template <typename R, typename ... Args>
  class filtered_rval: public functor<void,Args...> {
        functor<R,Args...> &mRaw;
        functor<R,R> &mFilter;
     public:
        filtered_rval(functor<R,Args...> &raw, functor<R,R> &rvalfilter): mRaw(raw), mFilter(rvalfilter) {}
        void operator()(Args...args) {
           return mFilter(mRaw(args...));
        }
  };
  
  //A filter_first_argument places a filter on the first argument of a functor.
  template <typename R, typename F, typename ...Args>
  class filter_first_argument: public functor<R,F,Args...> {
     functor<R,F,Args...> &mRaw;
     functor<F,F> &mFilter;
   public:
     filter_first_argument(functor<R,F,Args...> &raw,functor<F,F> &filter):mRaw(raw),mFilter(filter){}
     R operator()(F first,Args... args) {
        return mRaw(mFilter(first),args...);
     }
  };

  template <typename F, typename ...Args>
  class filter_first_argument<void,F,Args...>: public functor<void,F,Args...> {
     functor<void,F,Args...> &mRaw;
     functor<F,F> &mFilter;
   public:
     filter_first_argument(functor<void,F,Args...> &raw,functor<F,F> &filter):mRaw(raw),mFilter(filter){}
     void operator()(F first,Args... args) {
        mRaw(mFilter(first),args...);
     }
  };

  //A redirect_rval redirects the return vallue to a redirection sink without disclosing the result to the caller.
  template <typename R, typename ... Args>
  class redirect_rval: public functor<R,Args...> {
      functor<R,Args...> &mRaw;
      functor<void,R> &mRedirect;
     public:
      redirect_rval(functor<R,Args...> &raw, functor<void,R> &redirect): mRaw(raw),mRedirect(redirect) {}
      R operator()(Args...args) {
         return mRedirect(mRaw(args...));
      }
  };

  //A redirected_first_argument adds an extra argument to a functor. This argument gets redirected to a sink.
  template <typename R, typename F, typename ... Args>
  class redirected_first_argument: public functor<R,F,Args...> {
      functor<R,Args...> &mRaw;
      functor<void,F> &mSink;
     public:
      redirected_first_argument(functor<R,Args...> &raw,functor<void,F> &snk):mRaw(raw),mSink(snk){}
      R operator()(F first,Args...args) {
          mSink(first);
          return mRaw(args...);
      }
  };

  template <typename F, typename ... Args>
  class redirected_first_argument<void,F,Args...>: public functor<void,F,Args...> {
      functor<void,Args...> &mRaw;
      functor<void,F> &mSink;
     public:
      redirected_first_argument(functor<void,Args...> &raw,functor<void,F> &snk):mRaw(raw),mSink(snk){}
      void operator()(F first,Args...args) {
          mSink(first);
          return mRaw(args...);
      }
  };

  //An auto_first_argument is a proxy that exposes all but the first functor argument. The first argument on proxying is set by its source.
  template <typename R, typename F, typename ... Args>
  class auto_first_argument: public functor<R,Args...> {
      functor<R,F,Args...> &mRaw;
      functor<F> &mSource;
    public:
      auto_first_argument(functor<R,F,Args...> &raw,functor<F> &src): mRaw(raw),mSource(src){}
      R operator()(Args...args) {
         return mRaw(mSource(),args...);
      }
  };

  template <typename F, typename ... Args>
  class auto_first_argument<void,F,Args...>: public functor<void,Args...> {
      functor<void,F,Args...> &mRaw;
      functor<F> &mSource;
    public:
      auto_first_argument(functor<void,F,Args...> &raw,functor<F> &src): mRaw(raw),mSource(src){}
      void operator()(Args...args) {
         return mRaw(mSource(),args...);
      }
  };

  template <typename R,typename ... Args>
  class tee_rval: public functor<R,Args...> {
       functor<R,Args...> &mRaw;
       functor<void,R> &mTarget;
     public:
       tee_rval(functor<R,Args...> &raw,functor<void,R> &target):mRaw(raw),mTarget(target){}
        R operator()(Args...args) {
           R rval=mRaw(args...);
           mTarget(rval);
           return rval;
        }
  };

  template <typename R,typename ... Args>
  class tee_arguments: public functor<R,Args...> {
       functor<R,Args...> &mRaw;
       functor<void,Args...> &mTarget;
     public:
       tee_arguments(functor<R,Args...> &raw,functor<void,Args...> &target): mRaw(raw),mTarget(target){}
       R operator()(Args...args) {
          mTarget(args...);
          return mRaw(args...);
       }
  };

  template <typename ... Args>
  class tee_arguments<void,Args...>: public functor<void,Args...> {
       functor<void,Args...> &mRaw;
       functor<void,Args...> &mTarget;
     public:
       tee_arguments(functor<void,Args...> &raw,functor<void,Args...> &target): mRaw(raw),mTarget(target){}
       void operator()(Args...args) {
          mTarget(args...);
          mRaw(args...);
       }
  };

  template <typename R,typename ... Args>
  class conditional: public functor<R,Args...> {
        functor<R,Args...> &mOnTrue;
        functor<R,Args...> &mOnFalse;
        functor<bool> &mCondition;    
     public:
        conditional(functor<R,Args...> &ontrue,functor<R,Args...> &onfalse,functor<bool> &thecondition):mOnTrue(ontrue),mOnFalse(onfalse),mCondition(thecondition){}
        R operator()(Args...args) {
           if (mCondition) {
              return mOnTrue(args...);
           } else {
              return mOnFalse(args...);
           }
        }
  };

  template <typename ... Args>
  class conditional<void,Args...>: public functor<void,Args...> {
        functor<void,Args...> &mOnTrue;
        functor<void,Args...> &mOnFalse;
        functor<bool> &mCondition;
     public:
        conditional(functor<void,Args...> &ontrue,functor<void,Args...> &onfalse,functor<bool> &thecondition):mOnTrue(ontrue),mOnFalse(onfalse),mCondition(thecondition){}
        void operator()(Args...args) {
           if (mCondition) {
              mOnTrue(args...);
              return;
           } else {
              mOnFalse(args...);
              return;
           }
        }
  };

}
#endif
