// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "muduo/net/Acceptor.h"

#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/SocketsOps.h"

#include <errno.h>
#include <fcntl.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport)
  : loop_(loop),
    acceptSocket_(sockets::createNonblockingOrDie(listenAddr.family())),  // 创建监听套接字
    acceptChannel_(loop, acceptSocket_.fd()),
    listening_(false),
    idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC))  // 打开一个空闲的文件描述符
{
  assert(idleFd_ >= 0);  // 该文件描述符有效
  acceptSocket_.setReuseAddr(true);
  acceptSocket_.setReusePort(reuseport);
  acceptSocket_.bindAddress(listenAddr);
  acceptChannel_.setReadCallback(
      std::bind(&Acceptor::handleRead, this));  // 设置一个读的回调函数
}

Acceptor::~Acceptor()
{
  acceptChannel_.disableAll();
  acceptChannel_.remove();
  ::close(idleFd_);
}

void Acceptor::listen()
{
  loop_->assertInLoopThread();
  listening_ = true;
  acceptSocket_.listen();  // 调用该套接字的监听事件
  acceptChannel_.enableReading();  // 设为刻度
}

void Acceptor::handleRead()
{
  loop_->assertInLoopThread();
  InetAddress peerAddr;  // 准备一个对等方的地址
  //FIXME loop until no more
  int connfd = acceptSocket_.accept(&peerAddr);
  if (connfd >= 0)
  {
    // string hostport = peerAddr.toIpPort();
    // LOG_TRACE << "Accepts of " << hostport;
    if (newConnectionCallback_)
    {
      newConnectionCallback_(connfd, peerAddr);
    }
    else
    {
      sockets::close(connfd);
    }
  }
  else
  {
    LOG_SYSERR << "in Acceptor::handleRead";
    // Read the section named "The special problem of
    // accept()ing when you can't" in libev's doc.
    // By Marc Lehmann, author of libev.
    if (errno == EMFILE)  // connecting失败 可用的文件描述符不足 这里可以看到上述的idleFd_的作用
    {
      ::close(idleFd_);  // 关闭这个文件描述符 使得出现一个可用的文件描述符
      idleFd_ = ::accept(acceptSocket_.fd(), NULL, NULL);  // 当有了一个新的可用文件描述符后 就可以accept
      ::close(idleFd_);   // 连接建立之后立即断开 
      idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);  // 重新打开idleFd_
      // 上述过程就是一个优雅的处理在文件描述符不足时出现新连接的情况
    }
  }
}

