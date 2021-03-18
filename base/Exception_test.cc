#include "Exception.h"
#include <iostream>

class Bar {
public:
  void test() {
    throw::muduo::Exception("oops"); // 抛出异常
  }  
};

void foo() {
  Bar b;
  b.test();
}

int main()
{
  try {
    foo(); // 运行此函数 会抛出 oops 异常
  }
  catch (const muduo::Exception& ex) {  // 捕捉异常 并打印异常信息
    std::cout << "reason: " << ex.what() << std::endl;
    std::cout << "stack trace: " << ex.stackTrace() << std::endl;
  }

  return 0;
}
