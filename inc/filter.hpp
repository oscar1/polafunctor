#ifndef _FILTER_HPP
#define _FILTER_HPP
#include "functor.hpp"
namespace polafunctor {
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
  //template <class T>
  //class equal_assert_filter: public filter<T> {
  //      T       mVal;
  //      std::string mMsg;
  //   public:
  //      equal_assert_filter(T val,std::string msg):mVal(val),mMsg(msg){}
  //      T operator()(T arg){
  //         if (!(arg== mVal)) throw equal_assert_filter_exception(mMsg);
  //         return arg;
  //      }
  //};
  //The rangeassertfilter throws an exception if the input falls outdide a preset range.
  //template <class T>
  //class range_assert_filter: public filter<T> {
  //      T       mMin;
  //      T       mMax;
  //      std::string mMsg;
  //   public:
  //      range_assert_filter(T min,T max,std::string msg):mMin(min),mMax(max),mMsg(msg){}
  //      T operator()(T arg){
  //         if ((arg < mMin) ||(mMax < arg))
  //            throw range_assert_filter_exception(mMsg);
  //         return arg;
  //      }
  //};

}
#endif
