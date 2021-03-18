// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_CURRENTTHREAD_H
#define MUDUO_BASE_CURRENTTHREAD_H

//#include "muduo/base/Types.h"
#include "Types.h"


namespace muduo
{
namespace CurrentThread
{
  // internal
  // __thread修饰的变量是线程局部存储的
  // __thread只能修饰POD类型
  extern __thread int t_cachedTid;          // 线程真实pid(tid)的缓存 减少系统调用的次数
  extern __thread char t_tidString[32];     // tid字符串形式
  extern __thread int t_tidStringLength;    // 长度
  extern __thread const char* t_threadName; // 线程名称
  void cacheTid();

  inline int tid()
  {
    if (__builtin_expect(t_cachedTid == 0, 0))
    {
      cacheTid();
    }
    return t_cachedTid;
  }

  inline const char* tidString() // for logging
  {
    return t_tidString;
  }

  inline int tidStringLength() // for logging
  {
    return t_tidStringLength;
  }

  inline const char* name()
  {
    return t_threadName;
  }

  bool isMainThread();

  void sleepUsec(int64_t usec);  // for testing

  string stackTrace(bool demangle);
}  // namespace CurrentThread
}  // namespace muduo

#endif  // MUDUO_BASE_CURRENTTHREAD_H
