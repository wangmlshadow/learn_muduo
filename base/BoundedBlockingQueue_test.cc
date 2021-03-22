//#include "muduo/base/BlockingQueue.h"
#include "muduo/base/BoundedBlockingQueue.h"
#include "muduo/base/Thread.h"
#include "muduo/base/CountDownLatch.h"

//#include <functional>
#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <string>
#include <stdio.h>
#include <iostream>
class Test {
public:
  Test(int numThreads) :queue_(20), latch_(numThreads), threads_(numThreads) {
    for (int i = 0; i < numThreads; ++i) {
      char name[32];
      snprintf(name, sizeof name, "work thread %d", i);
      threads_.push_back(new muduo::Thread(std::bind(&Test::threadFunc, this), std::string(name)));
    }
    for_each (threads_.begin(), threads_.end(), boost::bind(&muduo::Thread::start, _1));
  }

  void run(int times) {
    std::cout << "waiting for count down latch" << std::endl;
    latch_.wait();
    std::cout << "all threads started" << std::endl;

    for (int i = 0; i < times; ++i) {
      char buf[32];
      snprintf(buf, sizeof buf, "hello %d", i);
      queue_.put(buf);
      std::cout << "tid = " << muduo::CurrentThread::tid() << ", put data = " << buf << ", size = " << queue_.size() << std::endl;
    }
  }
  
  void joinAll() {
    for (size_t i = 0; i < threads_.size(); ++i) {
      queue_.put("stop");
    }

    for_each(threads_.begin(), threads_.end(), boost::bind(&muduo::Thread::join, _1));
  }

private:
  void threadFunc() {
    std::cout << "tid = " << muduo::CurrentThread::tid() << ", " << muduo::CurrentThread::name() << " started" << std::endl;

    latch_.countDown();
    bool running = true;

    while (running) {
      std::string d(queue_.take());
      std::cout << "tid = " << muduo::CurrentThread::tid() << ", get data = " << d << ", size = " << queue_.size() << std::endl;
      running = (d != "stop");
    }

    std::cout << "tid = " << muduo::CurrentThread::tid() << ", " << muduo::CurrentThread::name() << " stoped" << std::endl;
  }

  muduo::BoundedBlockingQueue<std::string> queue_;
  muduo::CountDownLatch latch_;
  boost::ptr_vector<muduo::Thread> threads_;
};

int main()
{
  std::cout << "pid = " << ::getpid() << ", tid = " << muduo::CurrentThread::tid() << std::endl;

  Test t(5);
  t.run(100);
  t.joinAll();

  std::cout << "number of createed thread: " << muduo::Thread::numCreated() << std::endl;

  return 0;
}
