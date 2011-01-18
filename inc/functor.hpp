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
  template <typename R, typename ... Args>
  class const_functor {
     public:
	virtual R operator()(Args...) const=0;
	virtual ~const_functor(){}
  }; 
}
#endif
