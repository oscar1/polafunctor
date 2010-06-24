#ifndef _PROXY_HPP
#define _PROXY_HPP
#include "functor.hpp"
namespace polafunctor {

  template <typename R, typename ... Args>
  class filtered_rval: public functor<void,Args...> {
        functor<R,Args...> &mRaw;
        filter<R> &mFilter;
     public:
        filtered_rval(functor<R,Args...> &raw, filter<R> &rvalfilter): mRaw(raw), mFilter(rvalfilter) {}
        void operator()(Args...args) {
           return mFilter(mRaw(args...));
        }
  };
  
  //A filter_first_argument places a filter on the first argument of a functor.
  template <typename R, typename F, typename ...Args>
  class filter_first_argument: public functor<R,F,Args...> {
     functor<R,F,Args...> &mRaw;
     filter<F> &mFilter;
   public:
     filter_first_argument(functor<R,F,Args...> &raw,filter<F> &filter):mRaw(raw),mFilter(filter){}
     R operator()(F first,Args... args) {
        return mRaw(mFilter(first),args...);
     }
  };

  template <typename F, typename ...Args>
  class filter_first_argument<void,F,Args...>: public sink<F,Args...> {
     sink<F,Args...> &mRaw;
     filter<F> &mFilter;
   public:
     filter_first_argument(sink<F,Args...> &raw,filter<F> &filter):mRaw(raw),mFilter(filter){}
     void operator()(F first,Args... args) {
        mRaw(mFilter(first),args...);
     }
  };

  //A redirect_rval redirects the return vallue to a redirection sink without disclosing the result to the caller.
  template <typename R, typename ... Args>
  class redirect_rval: public functor<R,Args...> {
      functor<R,Args...> &mRaw;
      sink<R> &mRedirect;
     public:
      redirect_rval(functor<R,Args...> &raw, sink<R> &redirect): mRaw(raw),mRedirect(redirect) {}
      R operator()(Args...args) {
         return mRedirect(mRaw(args...));
      }
  };

  //A redirected_first_argument adds an extra argument to a functor. This argument gets redirected to a sink.
  template <typename R, typename F, typename ... Args>
  class redirected_first_argument: public functor<R,F,Args...> {
      functor<R,Args...> &mRaw;
      sink<F> &mSink;
     public:
      redirected_first_argument(functor<R,Args...> &raw,sink<F> &snk):mRaw(raw),mSink(snk){}
      R operator()(F first,Args...args) {
          mSink(first);
          return mRaw(args...);
      }
  };

  template <typename F, typename ... Args>
  class redirected_first_argument<void,F,Args...>: public sink<F,Args...> {
      sink<Args...> &mRaw;
      sink<F> &mSink;
     public:
      redirected_first_argument(sink<Args...> &raw,sink<F> &snk):mRaw(raw),mSink(snk){}
      void operator()(F first,Args...args) {
          mSink(first);
          return mRaw(args...);
      }
  };

  //An auto_first_argument is a proxy that exposes all but the first functor argument. The first argument on proxying is set by its source.
  template <typename R, typename F, typename ... Args>
  class auto_first_argument: public functor<R,Args...> {
      functor<R,F,Args...> &mRaw;
      source<F> &mSource;
    public:
      auto_first_argument(functor<R,F,Args...> &raw,source<F> &src): mRaw(raw),mSource(src){}
      R operator()(Args...args) {
         return mRaw(mSource(),args...);
      }
  };

  template <typename F, typename ... Args>
  class auto_first_argument<void,F,Args...>: public sink<Args...> {
      sink<F,Args...> &mRaw;
      source<F> &mSource;
    public:
      auto_first_argument(sink<F,Args...> &raw,source<F> &src): mRaw(raw),mSource(src){}
      void operator()(Args...args) {
         return mRaw(mSource(),args...);
      }
  };

  template <typename R,typename ... Args>
  class tee_rval: public functor<R,Args...> {
       functor<R,Args...> &mRaw;
       sink<R> &mTarget;
     public:
       tee_rval(functor<R,Args...> &raw,sink<R> &target):mRaw(raw),mTarget(target){}
        R operator()(Args...args) {
           R rval=mRaw(args...);
           mTarget(rval);
           return rval;
        }
  };

  template <typename R,typename ... Args>
  class tee_arguments: public functor<R,Args...> {
       functor<R,Args...> &mRaw;
       sink<Args...> &mTarget;
     public:
       tee_arguments(functor<R,Args...> &raw,sink<Args...> &target): mRaw(raw),mTarget(target){}
       R operator()(Args...args) {
          mTarget(args...);
          return mRaw(args...);
       }
  };

  template <typename ... Args>
  class tee_arguments<void,Args...>: public sink<Args...> {
       sink<Args...> &mRaw;
       sink<Args...> &mTarget;
     public:
       tee_arguments(sink<Args...> &raw,sink<Args...> &target): mRaw(raw),mTarget(target){}
       void operator()(Args...args) {
          mTarget(args...);
          mRaw(args...);
       }
  };

  template <typename R,typename ... Args>
  class conditional: public functor<R,Args...> {
        functor<R,Args...> &mOnTrue;
        functor<R,Args...> &mOnFalse;
        condition &mCondition;    
     public:
        conditional(functor<R,Args...> &ontrue,functor<R,Args...> &onfalse,condition &thecondition):mOnTrue(ontrue),mOnFalse(onfalse),mCondition(thecondition){}
        R operator()(Args...args) {
           if (mCondition) {
              return mOnTrue(args...);
           } else {
              return mOnFalse(args...);
           }
        }
  };

  template <typename ... Args>
  class conditional<void,Args...>: public sink<Args...> {
        sink<Args...> &mOnTrue;
        sink<Args...> &mOnFalse;
        condition &mCondition;
     public:
        conditional(sink<Args...> &ontrue,sink<Args...> &onfalse,condition &thecondition):mOnTrue(ontrue),mOnFalse(onfalse),mCondition(thecondition){}
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
