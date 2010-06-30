#ifndef _SINK_HPP
#define _SINK_HPP
#include "functor.hpp"
#include <stdexcept>
#include <string>
namespace polafunctor {

  template <typename ... Args>
  class null_sink: public sink<Args...> {
     public:
        void operator()(Args...args) {
           return;
        };
  };
  
  template <typename ... Args>
  class tee_sink: public sink<Args...> {
       sink<Args...> &mFirst;
       sink<Args...> &mSecond;
    public:
       tee_sink(sink<Args...> &first,sink<Args...> &second):mFirst(first),mSecond(second){}
       void operator()(Args...args) {
          mFirst(args...);
          mSecond(args...);
       }
  };

/*  template <typename ... Args>
  class validating_sink: public sink<Args...> {
        sink<Args...> &mOnValid;
        sink<Args...> &mOnInValid;
        validator<Args...> &mValidator;
     public:
        validating_sink(sink<Args...> &onvalid,sink<Args...> &oninvalid,validator<Args...> &svalidator):mOnValid(onvalid),mOnInValid(oninvalid),mValidator(svalidator){}
        void operator()(Args...args) {
            if (mValidator(args...)) {
               mOnValid(args...);
            } else {
               mOnInValid(args...);
            }
        }

  };
*/
  template <typename ... Args>
  class throwing_sink: public sink<Args...> {
        std::string mFailMsg;
     public: 
        throwing_sink(std::string failmsg): mFailMsg(failmsg){}
        void operator()(Args...args) {
           throw std::runtime_error(mFailMsg);
        }
  };

}
#endif
