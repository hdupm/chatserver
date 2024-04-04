/*muduo网络库给用户提供了两个主要的类
TcpServer：用于编写服务器程序的
TcpClient：用于编写客户端程序的
epoll+线程池
好处：能够把网络IO的代码和业务代码区分开
                        用户的连接和断开 用户的可读写时间
*/
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>//绑定器都在c++头文件中
using namespace std;
using namespace muduo;
using namespace muduo::net;//muduo有很多作用域 这样下面可以省略很多细节
using namespace placeholders;//参数占位符

/*基于muduo网络库开发服务器程序
1.组合TcpServer对象
2.创建EventLoop事件循环对象的指针
3.明确TcpServer构造函数需要什么参数，输出ChatServer的构造函数
4.在当前服务器类的构造函数当中，注册处理连接的回调函数和处理读写事件的回调函数 主要关注两个回调函数的内容
5.设置合适的服务端线程数量，muduo库会自己分配I/O线程和worker线程
*/
class ChatServer{
public:
    ChatServer(EventLoop* loop,//事件循环(reactor反应堆)
            const InetAddress& listenAddr,//IP+Port
            const string& nameArg)//服务器的名字
            :_server(loop,listenAddr,nameArg)
            ,_loop(loop)
        {
            //回调就是事件发生时会通知
            //给服务器注册用户连接的创建和断开回调 void setConnectionCallback
            _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));//用绑定器绑定对象到方法中 _1是参数占位符，有一个参数
            //给服务器注册用户读写事件回调 void setMessageCallback
            _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));//要传三个参数 this是实例化的对象
            //设置服务器端的线程数量 自动会将一个线程用作io线程 工作线程3个
            _server.setThreadNum(4);
        }
        //开启事件循环
        void start(){
            _server.start();
        }

private:
    //专门处理用户的连接创建和断开 作为回调函数(见call backs.h) epoll listenfd accept
    void onConnection(const TcpConnectionPtr&conn){//成员方法        
        if(conn->connected()){//判断连接是否成功
            cout<<conn->peerAddress().toIpPort()<<"->"<<  //用来打印对端信息
        conn->localAddress().toIpPort()<<"state:online"<<endl;
        }
        else{
            cout<<conn->peerAddress().toIpPort()<<"->"<<  //用来打印对端信息
        conn->localAddress().toIpPort()<<"state:offline"<<endl;
        conn->shutdown();//close(fd) 回收资源
        //-loop->quit();//一个线程断了将服务断掉
        }
    }
    //专门处理用户的读写事件
    void onMessage(const TcpConnectionPtr& conn,//连接 通过连接读数据 写数据
                            Buffer* buffer,//缓冲区
                            Timestamp time)//接收到数据的时间信息
    {
        string buf=buffer->retrieveAllAsString();//封装了缓冲区的类 将数据放到字符串中
        cout<<"recv data:"<<buf<<"time:"<<time.toString()<<endl;//将时间信息转换成字符串
        conn->send(buf);

    }
    TcpServer _server;//#1
    EventLoop *_loop;//#2 epoll 注册感兴趣事件

};

int main()
{
    EventLoop loop;//epoll
    InetAddress addr("127.0.0.1",6000);
    ChatServer server(&loop,addr,"ChatServer");//先传事件循环地址，再传绑定地址，再传服务器名称
    server.start();//启动服务 listenfd epoll_ctl=epoll
    loop.loop();//epoll_wait以阻塞方式等待新用户链接，已连接用户的读写事件等
    return 0;
}