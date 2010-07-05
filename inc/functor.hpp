#ifndef _FUNCTOR_HPP
#define _FUNCTOR_HPP
namespace polafunctor {
  //The base abstract functor class template.
  template <typename R, typename ... Args>
  class functor {
     public:
        virtual R operator()(Args...)=0;
        virtual ~functor(){}
  };  
}
#endif
