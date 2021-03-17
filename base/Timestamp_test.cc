#include <iostream>
#include "Timestamp.h"
#include <vector>

int main()
{
  muduo::Timestamp time1(muduo::Timestamp::now());
  std::cout << time1.toString() << std::endl;
  std::cout << time1.toFormattedString(true) << std::endl;
  std::cout << time1.toFormattedString(false) << std::endl;
  
  // 创建1000000个Timestamp个对象所需要的时间
  const int kNumber = 1000*1000;
  std::vector<muduo::Timestamp> stamps;
  stamps.reserve(kNumber);

  for (int i = 0; i < kNumber; i++) {
    stamps.push_back(muduo::Timestamp::now());
  }

  std::cout << stamps.front().toFormattedString(false) << std::endl;
  std::cout << stamps.back().toFormattedString(false) << std::endl;
  std::cout << timeDifference(stamps.back(), stamps.front()) << std::endl;



  return 0;
}
// 编译命令
// g++ Timestamp.cc Timestamp_test.cc -o Timestamp_test
