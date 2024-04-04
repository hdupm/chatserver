#include "chatserver.hpp"
#include "json.hpp"
#include "chatservice.hpp"
#include <functional>
#include <string>
using namespace std;
using namespace placeholders;
using json=nlohmann::json;

ChatServer::ChatServer(EventLoop* loop,//�¼�ѭ��(reactor��Ӧ��)
            const InetAddress& listenAddr,//IP+Port
            const string& nameArg)
            :_server(loop,listenAddr,nameArg)
            ,_loop(loop)
    {
        //ע�����ӻص�
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));

        //ע����Ϣ�ص�
        _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));

        //�����߳�����
        _server.setThreadNum(4);

    }
    //�����¼�ѭ��
void ChatServer::start()
{
    _server.start();
}

//�ϱ����������Ϣ�Ļص�����
void ChatServer::onConnection(const TcpConnectionPtr& conn)
{    
    //�ͻ��˶Ͽ����� ���û�����
    if (!conn->connected())//��������ʧ�� ��Ҫ�ͷ�socket��Դ
    {
        ChatService::instance()->clientCloseException(conn);//����ͻ����쳣�ر���Ҫ�ı�����״̬
        conn->shutdown();
    }
}

//�ϱ���д�¼������Ϣ�Ļص�����
void ChatServer::onMessage(const TcpConnectionPtr& conn,//���� ͨ�����Ӷ����� д����
                        Buffer* buffer,//������
                        Timestamp time)//���յ����ݵ�ʱ����Ϣ
{
    string buf=buffer->retrieveAllAsString();//�����������ݷŵ��ַ�����
    //���ݵķ����л�
    json js=json::parse(buf);
    //�ﵽĿ�ģ���ȫ��������ģ��Ĵ����ҵ��ģ��Ĵ���
    //ͨ��js["msgid"] ��ȡ=��ҵ��handler=��conn js time
    auto msgHandler=ChatService::instance()->getHandler(js["msgid"].get<int>());//ǿתΪ���ͻ�ö�Ӧ���¼�������
    //�ص���Ϣ�󶨺õ��¼�����������ִ����Ӧ��ҵ����
    msgHandler(conn,js,time);


}


