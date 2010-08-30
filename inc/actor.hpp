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
     class EventProxy: public functor<void> {
	   functor<void> &mRaw;
        public:
            EventProxy(functor<void> &raw):mRaw(raw){}
	    void operator()() {
               mRaw();
	    }
     };
     void eventLoop() {
        EventProxy eproxy;
	mQueue.dequeue(&eproxy);
	eproxy();
     }
     boost::lockfree::queue mQueue;
    public:
       paralel_event_loop() {
         boost::thread eventthread(eventloop);
       }
       void operator()(functor<void> &funct) {
         EventProxy eprox(funct);
	 mQueue.enqueue(eprox);
       }
  };

  template <typename ... Args>
  class asynchonous_proxy: public functor<void,Args...> {
       functor<void,Args...> &mRaw;
       parallel_event_loop &mEloop;
     public:
       asynchonous_proxy(functor<void,Args...> &raw,parallel_event_loop eloop): mRaw(raw),mEloop(eloop){}
       void operator()(Args...args) {
          asynchonous_invocation<Args...> invokelater(mRaw,args...);
          eloop(invokelater); //This will not work for now, probably should use pointers or value objects, need to fix this.
       }   
  }
}
#endif
