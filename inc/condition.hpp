#ifndef _CONDITION_HPP
#define _CONDITION_HPP
#include "functor.hpp"
#include <sys/types.h>
#include <string>
namespace polafunctor {
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
