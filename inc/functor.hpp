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
  //Sinks are shorthand for functors that return void.
  template <typename ... Args>
  class sink: public functor<void,Args...> {};
  //Sources are shorthand for zero argument functors that return a given type vallue.
  template <typename R>
  class source: public functor<R> {};
  //Filters are shorthand for single argument functors that have a return type equal to the argument type.
  template <typename R>
  class filter: public functor<R,R> {};
  //Validators are shorthand for single argument functors that have a boolean return type.
  template <typename R>
  class validator: public functor<bool,R> {};
  //Events are shorthand for zero argument functors that returen void.
  class event: public functor<void> {};
  //Conditions are shorthand for zero argument functors that returen booleans.
  class condition: public functor<bool> {};
}
#endif
