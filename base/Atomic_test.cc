#include "Atomic.h"
#include <assert.h>
#include <iostream>

int main()
{
  muduo::AtomicInt64 a0; // 初始值为0
  assert(a0.get() == 0); // 获取a0的值 并判断其是否为0
  assert(a0.getAndAdd(1) == 0); // a0加1并返回之前的值
  assert(a0.get() == 1);
  assert(a0.addAndGet(2) == 3); // a0加2并返回加完后的值
  assert(a0.get() == 3);
  assert(a0.incrementAndGet() == 4); // ++a0
  assert(a0.get() == 4);
  a0.increment();
  assert(a0.get() == 5);
  assert(a0.decrementAndGet() == 4);
  assert(a0.get() == 4);
  a0.decrement();
  assert(a0.get() == 3);

  std::cout << "Successful ..." << std::endl;

  return 0;
}
