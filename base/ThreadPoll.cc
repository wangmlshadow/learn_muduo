#include "muduo/base/ThreadPool.h"
#include "muduo/base/CountDownLatch.h"
#include "muduo/base/CurrentThread.h"

#include <boost/bind.hpp>
#include <iostream>
#include <stdio.h>
void print() {
  std::cout << "tid = " << muduo::CurrentThread::tid() << std::endl;
}

void printString(const std::string& str) {
  std::cout << "tid = " << muduo::CurrentThread::tid() << ", str = " << str << std::endl;
}

int main()
{
  muduo::ThreadPool pool("MainThreadPool");
  //pool.start(5);
  pool.start(2);

  pool.run(print);
  pool.run(print);

  for (int i = 0; i < 1000; ++i) {
    char buf[32];
    snprintf(buf, sizeof buf, "take %d", i);
    pool.run(boost::bind(printString, std::string(buf)));
  }

  muduo::CountDownLatch latch(1);
  pool.run(boost::bind(&muduo::CountDownLatch::countDown, &latch));
  latch.wait();
  pool.stop();
}
