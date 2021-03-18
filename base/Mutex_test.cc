#include "Mutex.h"
#include "Thread.h"
#include "Timestamp.h"

#include <functional>
#include <vector>
#include <iostream>
#include <boost/ptr_container/ptr_vector.hpp>
using namespace muduo;
using namespace std;

MutexLock g_mutex;
vector<int> g_vec;
const int kCount = 10*1000*1000;

void threadFunc() {
  for (int i = 0; i < kCount; i++) {
    MutexLockGuard lock(g_mutex);
    g_vec.push_back(i);
  }
}

int main()
{
  const int kMaxThreads = 8;
  g_vec.reserve(kMaxThreads * kCount);

  Timestamp start(Timestamp::now());

  // 仅仅push_back
  for (int i = 0; i < kCount; i++) {
    g_vec.push_back(i);
  }

  // push_back kCount个数所用的时间
  cout << "single thread without lock " << timeDifference(Timestamp::now(), start) << endl;

  start = Timestamp::now();

  // push_back + MutexLockGuard的构造析构
  threadFunc(); // 
  cout << "single thread with lock " << timeDifference(Timestamp::now(), start) << endl;

  for (int nthreads = 1; nthreads < kMaxThreads; ++nthreads) {
    boost::ptr_vector<Thread> threads;
    g_vec.clear();
    start = Timestamp::now();

    for (int i = 0; i < nthreads; ++i) {
      threads.push_back(new Thread(&threadFunc));
      threads.back().start();
    }

    for (int i = 0; i < nthreads; ++i) {
      threads[i].join();
    }

    cout << nthreads << " threads with lock " << timeDifference(Timestamp::now(), start) << endl;
  }

  return 0;
}

// g++ Timestamp.cc CountDownLatch.cc CurrentThread.cc Thread.cc Mutex_test.cc -o Mutex_test -lpthread
