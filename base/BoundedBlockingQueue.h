// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_BOUNDEDBLOCKINGQUEUE_H
#define MUDUO_BASE_BOUNDEDBLOCKINGQUEUE_H

#include "muduo/base/Condition.h"
#include "muduo/base/Mutex.h"

#include <boost/circular_buffer.hpp>
#include <assert.h>

namespace muduo
{

template<typename T>
class BoundedBlockingQueue : noncopyable  // 有界缓冲区
{
 public:
  explicit BoundedBlockingQueue(int maxSize)
    : mutex_(),
      notEmpty_(mutex_),
      notFull_(mutex_),
      queue_(maxSize)
  {
  }

  void put(const T& x)
  {
    MutexLockGuard lock(mutex_);
    while (queue_.full())
    {
      notFull_.wait();
    }
    assert(!queue_.full());
    queue_.push_back(x);
    notEmpty_.notify();
  }

  void put(T&& x)
  {
    MutexLockGuard lock(mutex_);
    while (queue_.full())
    {
      notFull_.wait();
    }
    assert(!queue_.full());
    queue_.push_back(std::move(x));
    notEmpty_.notify(); // 通知不满
  }

  T take()
  {
    MutexLockGuard lock(mutex_);
    while (queue_.empty())
    {
      notEmpty_.wait();
    }
    assert(!queue_.empty());
    T front(std::move(queue_.front()));
    queue_.pop_front();
    notFull_.notify(); // 通知不空
    return front;
  }

  bool empty() const // 空
  {
    MutexLockGuard lock(mutex_);
    return queue_.empty();
  }

  bool full() const // 满
  {
    MutexLockGuard lock(mutex_);
    return queue_.full();
  }

  size_t size() const // 队列中已有元素的个数
  {
    MutexLockGuard lock(mutex_);
    return queue_.size();
  }

  size_t capacity() const // 队列总容量
  {
    MutexLockGuard lock(mutex_);
    return queue_.capacity();
  }

 private:
  mutable MutexLock          mutex_;
  Condition                  notEmpty_ GUARDED_BY(mutex_);
  Condition                  notFull_ GUARDED_BY(mutex_);
  boost::circular_buffer<T>  queue_ GUARDED_BY(mutex_); // 环形队列 环形缓冲区 
};

}  // namespace muduo

#endif  // MUDUO_BASE_BOUNDEDBLOCKINGQUEUE_H
