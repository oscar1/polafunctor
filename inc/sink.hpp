#ifndef _SINK_HPP
#define _SINK_HPP
#include "functor.hpp"
#include <stdexcept>
#include <string>
namespace polafunctor {

  template <typename ... Args>
  class null_sink: public functor<void,Args...> {
     public:
        void operator()(Args...args) {
           return;
        };
  };
  
  template <typename ... Args>
  class tee_sink: public functor<void,Args...> {
       functor<void,Args...> &mFirst;
       functor<void,Args...> &mSecond;
    public:
       tee_sink(functor<void,Args...> &first,functor<void,Args...> &second):mFirst(first),mSecond(second){}
       void operator()(Args...args) {
          mFirst(args...);
          mSecond(args...);
       }
  };

  template <typename ... Args>
  class validating_sink: public functor<void,Args...> {
        functor<void,Args...> &mOnValid;
        functor<void,Args...> &mOnInValid;
        functor<bool,Args...> &mValidator;
     public:
        validating_sink(functor<void,Args...> &onvalid,functor<void,Args...> &oninvalid,functor<bool,Args...> &svalidator):mOnValid(onvalid),mOnInValid(oninvalid),mValidator(svalidator){}
        void operator()(Args...args) {
            if (mValidator(args...)) {
               mOnValid(args...);
            } else {
               mOnInValid(args...);
            }
        }

  };

  template <typename ... Args>
  class throwing_sink: public functor<void,Args...> {
        std::string mFailMsg;
     public: 
        throwing_sink(std::string failmsg): mFailMsg(failmsg){}
        void operator()(Args...args) {
           throw std::runtime_error(mFailMsg);
        }
  };

}
#endif
