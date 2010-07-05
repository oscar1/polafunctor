#ifndef _CONDITION_HPP
#define _CONDITION_HPP
#include "functor.hpp"
#include <sys/types.h>
#include <string>
namespace polafunctor {
  //A quota condition returns true on the first n invocations and false after the invocation quota has been excausted.
  class quota: public functor<bool> {
      size_t mMax;
      size_t mCount;
    public:
      quota(size_t maxinvoke):mMax(maxinvoke),mCount(0){}
      bool operator()(){
        if (mCount >= mMax) {
               return false;
	}
        mCount++;
        return true;
      }
  }; 

  //once is shorthand for a quota of 1.
  class once: public quota {
     public:
      once():quota(1){}
   };

  //A revokable will return true each invocation untill after its revoke method has been invoked.
  class revokable: public functor<bool> {
      class revokefunctor: public functor<void> {
	   revokable *mRevokable;
        public:
	   revokefunctor(revokable *rev): mRevokable(rev){}
	   void operator()(){
              return mRevokable->revoke();
	   }
      };
      bool mRevoked;
      revokefunctor mRFunctor;
    public:
      revokable():mRevoked(false),mRFunctor(this){}
      void revoke(){mRevoked=true;}
      functor<void> &getRevokeFunctor() {
        return mRFunctor;
      }
      bool operator()(){
        if (mRevoked) {
		return false;
	}
        return true;
      }
  };

  class conditionstore: public functor<bool>, public functor<void,bool> {
        bool mState;
     public:
        conditionstore(bool state):mState(state){}
        bool operator()(){
           return mState;
        }
        void operator()(bool newstate) {
           mState=newstate;
        }
  };
}
#endif
