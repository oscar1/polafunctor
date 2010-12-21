//This file is still very very experimental, ignore it for now.
#ifndef _ACTOR_HPP
#define _ACTOR_HPP
#include <boost/lockfree/queue.hpp>
#include <boost/thread.hpp>
#include "functor.hpp"
#include <sys/types.h>
#include <string>
namespace polafunctor {
  
  class parallel_event_loop: public functor<void, functor<void> & > {
     void eventLoop() {
        functor<void> *event=0;;
	mQueue.dequeue(&event);
	(*event)();
	delete event; //FIXME, we should be able to do this without stack usage.
     }
     boost::lockfree::queue<functor<void> *> mQueue;
    public:
       paralel_event_loop() {
         boost::thread eventthread(eventloop);
       }
       void operator()(functor<void> *funct) {
	 mQueue.enqueue(funct);
       }
  };

  template <typename ... Args>
  class asynchonous_proxy: public functor<void,Args...> {
       functor<void,Args...> &mRaw;
       parallel_event_loop &mEloop;
     public:
       asynchonous_proxy(functor<void,Args...> &raw,parallel_event_loop eloop): mRaw(raw),mEloop(eloop){}
       void operator()(Args...args) {
          asynchonous_invocation<Args...> *invokelater=new asynchonous_invocation<Args...>(mRaw,args...); //FIXME, we should be able to do this without stack usage.
          eloop(invokelater);
       }   
  }
}
#endif
