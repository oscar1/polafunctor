#ifndef _FILTER_HPP
#define _FILTER_HPP
#include "functor.hpp"
namespace polafunctor {
  //A constfilter is the filter variant of the constfunctor, it ignores the input and produces the same output every invocation.
  template <class T>
  class source_filter: public filter<T> {
        source<T> &mSource;
     public:
        source_filter(source<T> &fsource):mSource(fsource){}
        T operator()(T){
           return mSource();
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

  template <class T>
  class tee_filter: public filter<T> {
       sink<T> &mSink;
     public:
       tee_filter(sink<T> &teesink):mSink(teesink){}
       T operator()(T arg){
           mSink(arg);
           return arg;
       }
  };

}
#endif
