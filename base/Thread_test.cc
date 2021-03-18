#include <iostream>
#include <string>
#include <functional>
#include <stdio.h>
#include <unistd.h>
#include "Thread.h"
#include "CurrentThread.h"

void threadFunc() {
  std::cout << "tid = " << muduo::CurrentThread::tid() << std::endl;
}

void threadFunc2(int x) {
  std::cout << "tid = " << muduo::CurrentThread::tid() << " x = " << x << std::endl;
}

class Foo {
public:
  explicit Foo(double x) : x_(x) {}
  
  void memberFunc2(const std::string& text) {
    std::cout << "tid = " << muduo::CurrentThread::tid() << " Foo::x_ = " << x_ << " text = " << text << std::endl;
  }

private:
  double x_;
};

int main()
{
  std::cout << "pid = " << ::getpid() << " tid = " << muduo::CurrentThread::tid() << std::endl;

  muduo::Thread t1(threadFunc);
  t1.start();
  t1.join();

  muduo::Thread t2(std::bind(threadFunc2, 42), "thread for free function with argument");
  t2.start();
  t2.join();

  Foo foo(87.35);
  //muduo::Thread t3(std::bind(&Foo::memberFunc2, &foo);
  //t3.start();
  //t3.join();

  muduo::Thread t4(std::bind(&Foo::memberFunc2, &foo, "thread for member function with argument"));
  t4.start();
  t4.join();

  std::cout << "number of created threads " << muduo::Thread::numCreated() << std::endl;

  return 0;
}
