#ifndef _POLAFUNCTOR_HPP
#define _POLAFUNCTOR_HPP
#include <stdexcept>
#include <string>
namespace polafunctor {
  class failed_condition: public std::runtime_error {
    public:
      failed_condition(std::string msg) : std::runtime_error(msg) {}
  };
  class functor_exception : public std::runtime_error {
    public:
      functor_exception(std::string msg) : std::runtime_error(msg) {}
  };
  class assert_filter_exception : public std::runtime_error {
    public:
      assert_filter_exception(std::string msg) : std::runtime_error(msg){}
  };
  class equal_assert_filter_exception : public assert_filter_exception {
    public:
      equal_assert_filter_exception(std::string msg) : assert_filter_exception(msg){}
  };
  class range_assert_filter_exception : public assert_filter_exception {
    public:
      range_assert_filter_exception(std::string msg) : assert_filter_exception(msg){}
  };
  //The base abstract functor class template.
  template <typename R, typename ... Args>
  class functor {
     public:
        virtual R operator()(Args...)=0;
        virtual ~functor(){}
  };  
  //Sinks are shorthand for functors that return void.
  template <typename ... Args>
  class sink: public functor<void,Args...> {};
  //Sources are shorthand for zero argument functors that return a given type vallue.
  template <typename R>
  class source: public functor<R> {};
  //Filters are shorthand for single argument functors that have a return type equal to the argument type.
  template <typename R>
  class filter: public functor<R,R> {};
  //Events are shorthand for zero argument functors that returen void.
  class event: public functor<void> {};
  //Conditions are shorthand for zero argument functors that returen booleans.
  class condition: public functor<bool> {};

 
  //A discard_rval discards the return value without disclosing it to the caller.
  template <typename R, typename ... Args>
  class discard_rval: public functor<void,Args...> {
      functor<R,Args...> &mRaw;
    public:
      discard_rval(functor<R,Args...> &raw):mRaw(raw){}
      void operator()(Args...args) {
         mRaw(args...);
	 return;
      }
  };
  
  //A redirect_rval redirects the return vallue to a redirection sink without disclosing the result to the caller.
  template <typename R, typename ... Args>
  class redirect_rval: public functor<void,Args...> {
      functor<R,Args...> &mRaw;
      sink<R> &mRedirect;
     public:
      redirect_rval(functor<R,Args...> &raw, sink<R> &redirect): mRaw(raw),mRedirect(redirect) {}
      void operator()(Args...args) {
         return mRedirect(mRaw(args...));
      }
  };
  //A profile invokes an event before the actual raw functor gets invoked and an other event after invocation.
  template <typename R, typename ... Args>
  class profile: public functor<R,Args...> {
      functor<R,Args...> &mRaw;
      event &mOnStart;
      event &mOnEnd;
    public:
      profile(functor<R,Args...> &raw,event &startevent,event &endevent): mRaw(raw),mOnStart(startevent),mOnEnd(endevent){}
      R operator()(Args...args) {
         mOnStart();
         R rval=mRaw(args...);
         mOnEnd();
         return rval;
      }
  };
  //Specialized profile for void return type.
  template <typename ... Args>
  class profile<void,Args...> : public functor<void,Args...> {
      functor<void,Args...> &mRaw;
      event &mOnStart;
      event &mOnEnd;
    public:
      profile(functor<void,Args...> &raw,event &startevent,event &endevent): mRaw(raw),mOnStart(startevent),mOnEnd(endevent){}
      void operator()(Args...args) {
         mOnStart();
         mRaw(args...);
         mOnEnd();
         return;
      }
  };
  //A conditional_discard will proxy when its condition returns true and will without proxying return a default value if the condition returns false. 
  template <typename R, typename ... Args>
  class conditional_discard: public functor<R,Args...> {
     functor<R,Args...> &mRaw;
     condition &mCondition;
     R mDefault;
   public:
     conditional_discard(functor<R,Args...> &raw,condition &cond,R dfl):mRaw(raw),mCondition(cond),mDefault(dfl){}
     R operator()(Args...args) {
         if (mCondition()) {
            return mRaw(args...);
	 } else {
            return mDefault;
	 }
     }
  };
  //The conditional_discard void special case has no default.
  template <typename ... Args>
  class conditional_discard<void,Args...>: public functor<void,Args...> {
     functor<void,Args...> &mRaw;
     condition &mCondition;
   public:
     conditional_discard(functor<void,Args...> &raw,condition &cond):mRaw(raw),mCondition(cond){}
     void operator()(Args...args) {
         if (mCondition()) {
            return mRaw(args...);
         } else {
            return ;
         }
     }
  };
  //A conditional_throw will proxy when its condition returns true and will throw a failed_condition if the condition returns false.
  template <typename R, typename ... Args>
  class conditional_throw: public functor<R,Args...> {
     functor<R,Args...> &mRaw;
     condition &mCondition;
     std::string mMsg;
   public:
     conditional_throw(functor<R,Args...> &raw,condition &cond,std::string failmsg):mRaw(raw),mCondition(cond),mMsg(failmsg){}
     R operator()(Args...args) {
         if (mCondition()) {
            return mRaw(args...);
         } else {
            throw failed_condition(mMsg);
         }
     }
  };

  //A const_first_argument is a proxy that exposes all but the first functor argument. The first argument on proxying is set to a constant value.
  template <typename R, typename F, typename ... Args>
  class const_first_argument: public functor<R,Args...> {
      functor<R,F,Args...> &mRaw;
      F mVal;
    public: 
      const_first_argument(functor<R,F,Args...> &raw,F &val): mRaw(raw),mVal(val){}
      R operator()(Args...args) {
         return mRaw(mVal,args...);
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

  //An redirected_extra_first_argument adds an extra discarded argument to a functor.
  template <typename R, typename F, typename ... Args>
  class discarded_first_argument: public functor<R,F,Args...> {
      functor<R,Args...> &mRaw;
     public:
      discarded_first_argument(functor<R,Args...> &raw):mRaw(raw){}
      R operator()(F &,Args...args) {
          return mRaw(args...);
      }
  };
  
  //A redirected_extra_first_argument adds an extra argument to a functor. This argument gets redirected to a sink.
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

  //An argument store is a (non thread safe) helper class that can be given to both an auto_first_argument and a corresponding redirected_first_argument. 
  template <typename R>
  class argumentstore: public source<R>,public sink<R> {
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

  //A const functor simply discards all its arguments and returns the same vallue each invocation.
  template <class R, typename ... Args>
  class const_functor: public functor<R,Args...> {
        R mRval;
      public:
        const_functor(R val):mRval(val){}
        R operator()(Args...) {
          return mRval;
        }
  };

  //A constfilter is the filter variant of the constfunctor, it ignores the input and produces the same output every invocation.
  template <class T>
  class const_filter: public filter<T> {
        T       mVal;
     public:
        const_filter(T val):mVal(val){}
        T operator()(T){
           return mVal;
        }
  };

  template <class T>
  class null_sink: public sink<T> {
     public:
        void operator()(T) {
           return;
        };
  };

  //The maskfilter applies an '&' operation on the input and a mask.
  template <class T>
  class mask_filter: public filter<T> {
        T       mMask;
     public:
        mask_filter(T val):mMask(val){}
        T operator()(T arg){
           return (arg & mMask);
        }
  };

  //The nullfilter simply returns its input without any filtering.
  template <class T>
  class null_filter: public filter<T> {
     public:
        T operator()(T arg){return arg;}
  };

  //The rangefilter makes sure that the value fals within a given range and changes the vallue to min or max if it falls outside of the range.
  template <class T>
  class range_filter: public filter<T> {
        T       mMin;
        T       mMax;
     public:
        range_filter(T min,T max):mMin(min),mMax(max){}
        T operator()(T arg){
           if (arg < mMin) return mMin;
           if (mMax < arg) return mMax;
           return arg;
        }
  };

  //The equalassertfilter throws an exception if the input is unequal to its preset vallue.
  template <class T>
  class equal_assert_filter: public filter<T> {
        T       mVal;
        std::string mMsg;
     public:
        equal_assert_filter(T val,std::string msg):mVal(val),mMsg(msg){}
        T operator()(T arg){
           if (!(arg== mVal)) throw equal_assert_filter_exception(mMsg);
           return arg;
        }
  };
  //The rangeassertfilter throws an exception if the input falls outdide a preset range.
  template <class T>
  class range_assert_filter: public filter<T> {
        T       mMin;
        T       mMax;
        std::string mMsg;
     public:
        range_assert_filter(T min,T max,std::string msg):mMin(min),mMax(max),mMsg(msg){}
        T operator()(T arg){
           if ((arg < mMin) ||(mMax < arg))
              throw range_assert_filter_exception(mMsg);
           return arg;
        }
  };

  //A const_source will return the same preset vallue each invocation.
  template <class T>
  class const_source: public source<T> {
       T mConst;
     public:
       const_source(T val):mConst(val){}
       T operator()() { return mConst;}
  }; 

  //A quota condition returns true on the first n invocations and false after the invocation quota has been excausted.
  class quota: public condition {
      size_t mMax;
      size_t mCount;
      std::string mMsg;
    public:
      quota(size_t maxinvoke,std::string msg):mMax(maxinvoke),mCount(0),mMsg(msg){}
      bool operator()(){
        if (mCount >= mMax) {
               return false;
	}
        mCount++;
        return true;
      }
  }; 

  class once: public condition {
      bool mRevoked;
     public:
      once():mRevoked(false){}
      bool operator()(){
        if (mRevoked) {
          return false;
        }
        mRevoked=true;
        return true;
      }
   };

  //A revokable will return true each invocation untill after its revoke method has been invoked.
  class revokable: public condition {
      bool mRevoked;
    public:
      revokable():mRevoked(false){}
      void revoke(){mRevoked=true;}
      bool operator()(){
        if (mRevoked) {
		return false;
	}
        return true;
      }
  };

}
#endif
