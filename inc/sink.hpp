#ifndef _SINK_HPP
#define _SINK_HPP
#include "functor.hpp"
namespace polafunctor {

  template <class T>
  class null_sink: public sink<T> {
     public:
        void operator()(T) {
           return;
        };
  };

}
#endif
