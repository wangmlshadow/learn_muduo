// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_THREAD_H
#define MUDUO_BASE_THREAD_H

//#include "muduo/base/Atomic.h"
#include "Atomic.h"
//#include "muduo/base/CountDownLatch.h"
#include "CountDownLatch.h"
//#include "muduo/base/Types.h"
#include "Types.h"

#include <functional>
#include <memory>
#include <pthread.h>


namespace muduo
{

class Thread : noncopyable
{
 public:
  typedef std::function<void ()> ThreadFunc;

  explicit Thread(ThreadFunc, const string& name = string());
  // FIXME: make it movable in C++11
  ~Thread();

  void start();：
  int join(); // return pthread_join()

  bool started() const { return started_; }
  // pthread_t pthreadId() const { return pthreadId_; }
  pid_t tid() const { return tid_; }
  const string& name() const { return name_; }

  static int numCreated() { return numCreated_.get(); }

 private:
  void setDefaultName();

  bool       started_;   // 线程是否启动
  bool       joined_;
  pthread_t  pthreadId_; // 线程Id
  pid_t      tid_;       // 线程真实ID
  ThreadFunc func_;      // 回调函数
  string     name_;      // 线程名称
  CountDownLatch latch_; 

  static AtomicInt32 numCreated_; // 已创建的线程个数
};

}  // namespace muduo
#endif  // MUDUO_BASE_THREAD_H
