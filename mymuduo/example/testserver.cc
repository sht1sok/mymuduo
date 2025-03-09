#include <mymuduo/TcpServer.h>
#include <string>
#include <functional>
class EchoServer
{
public:
    EchoServer(EventLoop *loop,
                const InetAddress &addr,
                const std::string &name)
            :server_(loop,addr,name),
            loop_(loop)
    {
        //设置回调函数
        server_.setConnectionCallback(
            std::bind(&EchoServer::onConnection,this,std::placeholders::_1)
        );
        server_.setMessageCallback(
            std::bind(&EchoServer::onMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)
        );
        //设置合适的loop线程数量
        server_.setThreadNum(3);
    }  
    void start()
    {
        server_.start();
    }
private:
    //建立连接或者断开回调
    void onConnection(const TcpConnectionPtr &conn)
    {
        if(conn ->connected())
        {
            printf("conn Up");
        }
        else
        {
            printf("conn Down");
        }
    }

    //可读写事件回调
    void onMessage(const TcpConnectionPtr &conn,
                    Buffer *buf,
                    Timestamp time)
    {
        std::string msg = buf -> retrieveAllAsString();
        conn->send(msg);
        conn->shutdown(); //关闭写端， EPOLLhup  => CLOSEcallback
    }

    EventLoop *loop_;
    TcpServer server_;
};

int main()
{   
    EventLoop loop;
    InetAddress addr(8000);
    EchoServer server (&loop,addr,"EchoServer-01"); //acceptor non-blocking listenfd create bind
    server.start();  //listen loopthread listen => acceptChannel => mainloop

    loop.loop();  //启动mianloop的底层Poller
    
    return 0;
}