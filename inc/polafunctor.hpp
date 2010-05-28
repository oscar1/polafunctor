#ifndef _POLACAST_HPP
#define _POLACAST_HPP
#include <stdexcept>
#include <string>
namespace polacanthus {
  //Exceptions thrown by this library
  class functor_exception : public std::runtime_error {
    public:
      functor_exception(std::string msg) : std::runtime_error(msg) {}
  };
  class throwfunctor_exception : public functor_exception {
    public: 
      throwfunctor_exception(std::string msg) : functor_exception(msg){}
  };
  class assertfilter_exception : public functor_exception {
    public:
      assertfilter_exception(std::string msg) : functor_exception(msg){}
  };
  class equal_assertfilter_exception : public assertfilter_exception {
    public:
      equal_assertfilter_exception(std::string msg) : assertfilter_exception(msg){}
  };
  class range_assertfilter_exception : public assertfilter_exception {
    public:
      range_assertfilter_exception(std::string msg) : assertfilter_exception(msg){}
  };
  class assert_exception : public functor_exception {
    public:
      assert_exception(std::string msg) : functor_exception(msg){}
  };
  class quota_assert_exception : public assert_exception {
    public:
      quota_assert_exception(std::string msg) : assert_exception(msg){}
  };
  class revocation_assert_exception : public assert_exception {
    public:
      revocation_assert_exception(std::string msg) : assert_exception(msg){}
  };
  class invert_assertfilter_exception : public assert_exception {
    public:
      invert_assertfilter_exception(std::string msg) : assert_exception(msg){}
  };
  //A few base classes
  template <class R, typename ... Args>
  class functor {
     public:
        virtual R operator()(Args...)=0;
  };  

  template <class R>
  class filter: public functor<R,R> {};

  template <class R>
  class generator: public functor<R> {};  
 
  //A few proxies
/*
  template <class R, typename ... Args>
  class statefull_proxy: public functor<R,Args...> {
     functor<R,Args...> &mRaw;
     generator<bool> &mGenerator;
     R mDefault;
   public:
     statefull_proxy(functor<R,Args...> &raw,generator<bool> &gen,R dfl):mRaw(raw),mGenerator(gen),mDefault(dfl){}
     R operator()(Args...) {
         if (mGenerator()) {
            return mRaw(Args ...);
	 } else {
            return mDefault;
	 }
     }
  };

  template <class R, class F, typename ... Args>
  class setfirst_proxy: public functor<R,Args...> {
      functor<R,F,Args...> &mRaw;
      F mVal;
    private: 
      setfirst_proxy(functor<R,F,Args...> &raw,F &val): mRaw(raw),mVal(val){}
      R operator()(Args...) {
         return mRaw(mVal,Args...);
      }
  };

  template <class R, class F, typename ... Args>
  class rotleft_proxy: public functor<R,Args...,F> {
      functor<R,F,Args...> &mRaw;
    public:
      rotright_proxy(functor<R,F,Args...> &raw):mRaw(raw){}
      R operator()(F first, Args...) {
        return(Args...,first);
      }
  };

  template <class R, class F, typename ... Args>
  class filterfirst_proxy: public functor<R,F,Args...> {
     functor<R,F,Args...> &mRaw;
     functor<F,F> &mFilter;
   public:
     filterfirst_proxy(functor<R,F,Args...> &raw,functor<F,F> &filter):mRaw(raw),mFilter(filter){}
     R operator()(F first,Args...) {
        return mRaw(mFilter(first),Args...);
     }
  }; 
*/
  //Some simple functors with standard behaviour
  template <class R, typename ... Args>
  class nullfunctor: public functor<R,Args...> {
        R mRval;
      public:
        nullfunctor(R val):mRval(val){}
        R operator()(Args...) {
          return mRval;
        }
  };

  template <class R, typename ... Args>
  class throwfunctor: public functor<R,Args...> {
        std::string mMsg;
      public:
        throwfunctor(std::string msg):mMsg(msg){}
        R operator()(Args...) {
          throw throwfunctor_exception(mMsg);
        }
  };

  //Some simple filter functors with standard behaviour.
  template <class T>
  class constfilter: public filter<T> {
        T       mVal;
     public:
        constfilter(T val):mVal(val){}
        T operator()(T){
           return mVal;
        }
  };

  template <class T>
  class maskfilter: public filter<T> {
        T       mMask;
     public:
        maskfilter(T val):mMask(val){}
        T operator()(T arg){
           return (arg & mMask);
        }
  };

  template <class T>
  class nullfilter: public filter<T> {
     public:
        T operator()(T arg){return arg;}
  };

  template <class T>
  class rangefilter: public filter<T> {
        T       mMin;
        T       mMax;
     public:
        rangefilter(T min,T max):mMin(min),mMax(max){}
        T operator()(T arg){
           if (arg < mMin) return mMin;
           if (mMax < arg) return mMax;
           return arg;
        }
  };

  template <class T>
  class equalassertfilter: public filter<T> {
        T       mVal;
        std::string mMsg;
     public:
        equalassertfilter(T val,std::string msg):mVal(val),mMsg(msg){}
        T operator()(T arg){
           if (!(arg== mVal)) throw equal_assertfilter_exception(mMsg);
           return arg;
        }
  };

  template <class T>
  class rangeassertfilter: public filter<T> {
        T       mMin;
        T       mMax;
        std::string mMsg;
     public:
        rangeassertfilter(T min,T max,std::string msg):mMin(min),mMax(max),mMsg(msg){}
        T operator()(T arg){
           if ((arg < mMin) ||(mMax < arg))
              throw range_assertfilter_exception(mMsg);
           return arg;
        }
  };

  template <class T>
  class invertassertfilter: public filter<T> {
        functor<T,T>   &mTarget;
        std::string mMsg;
     public:
        invertassertfilter(filter<T> &target,std::string msg):mTarget(target),mMsg(msg){}
        T operator()(T arg){
           try {
              mTarget(arg);
           } catch (assertfilter_exception &e) {
              return arg;
           }
           throw invert_assertfilter_exception(mMsg);
        }
  };

  //Some standard behaviour generators
  class true_generator: public generator<bool> {
    public:
      bool operator()(){return true;}
  };

  class false_generator: public generator<bool> {
    public:
      bool operator()(){return false;}
  };

  class quota: public generator<bool> {
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

  class revokable: public generator<bool> {
      bool mRevoked;
    public:
      revokable(std::string msg):mRevoked(false){}
      void revoke(){mRevoked=true;}
      bool operator()(){
        if (mRevoked) {
		return false;
	}
        return true;
      }
  };

  class throw_on_false: public generator<bool> {
      generator<bool> &mRawGen;
      std::string mMsg;
    public:
      throw_on_false(generator<bool> &rawgen,std::string msgonfail): mRawGen(rawgen),mMsg(msgonfail){}
      bool operator()(){
          if (mRawGen()) {
             return true;
	  }
	  throw revocation_assert_exception(mMsg);
      }
  };

}
#endif
