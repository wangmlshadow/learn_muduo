// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_COUNTDOWNLATCH_H
#define MUDUO_BASE_COUNTDOWNLATCH_H

// 对条件变量的封装 一个倒计时的门栓类

// 可用于所有子线程等待主线程发起起跑  主线程通知子线程
// 也可用于主线程等待子线程初始化完成才开始工作  

#include "muduo/base/Condition.h"
#include "muduo/base/Mutex.h"

namespace muduo
{

class CountDownLatch : noncopyable
{
 public:

  explicit CountDownLatch(int count);

  void wait();

  void countDown();

  int getCount() const;

 private:
  mutable MutexLock mutex_;// mutable 可以改变其修饰的数据成员的状态
  Condition condition_ GUARDED_BY(mutex_);
  int count_ GUARDED_BY(mutex_);// 计数器
};

}  // namespace muduo
#endif  // MUDUO_BASE_COUNTDOWNLATCH_H
