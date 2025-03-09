#include "Buffer.h"

#include<errno.h>
#include<sys/uio.h>
#include<unistd.h>

/*
*从fd上读取数据 ，Poller工作在LT模式
*Buffer缓冲区是由大小的， 但是从fd上读数据的时候，却不知道tcp数据最终大小
*/
ssize_t  Buffer::readFd(int fd, int* saveErrno)
{
    char extrabuff[65536] = {0}; //栈上的内存空间
    struct iovec vec[2];
    const size_t writable  = writableBytes(); //设置buffer剩余可写空间大小
    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writable;

    vec[1].iov_base = extrabuff;
    vec[1].iov_len = sizeof extrabuff;

    const int iovcnt = (writable < sizeof extrabuff)  ? 2 : 1 ;
    const ssize_t n = ::readv(fd, vec ,iovcnt);
    if(n < 0)
    {
        *saveErrno = errno;
    }
    else if( n <= writable)  //Buffer的可读写缓冲区足够
    {
        writerIndex_ += n;
    }
    else
    {
        writerIndex_ = buffer_.size();
        append(extrabuff,n-writable);  //writeIndex_开始写 n-writable的数据
    }

    return n;
}

ssize_t Buffer::writeFd(int fd,int* saveErrno)
{
    ssize_t n = ::write(fd,peek(),readableBytes());
    if(n < 0)
    {
        *saveErrno = errno;
    }
    return n;
}