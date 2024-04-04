#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
using namespace muduo;
using namespace muduo::net;

//�������������
class ChatServer
{
public:
    //��ʼ���������������
    ChatServer(EventLoop* loop,//�¼�ѭ��(reactor��Ӧ��)
            const InetAddress& listenAddr,//IP+Port
            const string& nameArg);//������������

    //��������
    void start();

private:
    //�ϱ����������Ϣ�Ļص�����
    void onConnection(const TcpConnectionPtr&);

    //�ϱ���д�¼������Ϣ�Ļص�����
    void onMessage(const TcpConnectionPtr&,//���� ͨ�����Ӷ����� д����
                            Buffer*,//������
                            Timestamp);
    TcpServer _server;//��ϵ�muduo�⣬ʵ�ַ��������ܵ������
    EventLoop *_loop;//ָ���¼�ѭ�������ָ��
};

#endif