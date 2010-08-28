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
}
#endif
