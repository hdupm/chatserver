/*muduo�������û��ṩ��������Ҫ����
TcpServer�����ڱ�д�����������
TcpClient�����ڱ�д�ͻ��˳����
epoll+�̳߳�
�ô����ܹ�������IO�Ĵ����ҵ��������ֿ�
                        �û������ӺͶϿ� �û��Ŀɶ�дʱ��
*/
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>//��������c++ͷ�ļ���
using namespace std;
using namespace muduo;
using namespace muduo::net;//muduo�кܶ������� �����������ʡ�Ժܶ�ϸ��
using namespace placeholders;//����ռλ��

/*����muduo����⿪������������
1.���TcpServer����
2.����EventLoop�¼�ѭ�������ָ��
3.��ȷTcpServer���캯����Ҫʲô���������ChatServer�Ĺ��캯��
4.�ڵ�ǰ��������Ĺ��캯�����У�ע�ᴦ�����ӵĻص������ʹ����д�¼��Ļص����� ��Ҫ��ע�����ص�����������
5.���ú��ʵķ�����߳�������muduo����Լ�����I/O�̺߳�worker�߳�
*/
class ChatServer{
public:
    ChatServer(EventLoop* loop,//�¼�ѭ��(reactor��Ӧ��)
            const InetAddress& listenAddr,//IP+Port
            const string& nameArg)//������������
            :_server(loop,listenAddr,nameArg)
            ,_loop(loop)
        {
            //�ص������¼�����ʱ��֪ͨ
            //��������ע���û����ӵĴ����ͶϿ��ص� void setConnectionCallback
            _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));//�ð����󶨶��󵽷����� _1�ǲ���ռλ������һ������
            //��������ע���û���д�¼��ص� void setMessageCallback
            _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));//Ҫ���������� this��ʵ�����Ķ���
            //���÷������˵��߳����� �Զ��Ὣһ���߳�����io�߳� �����߳�3��
            _server.setThreadNum(4);
        }
        //�����¼�ѭ��
        void start(){
            _server.start();
        }

private:
    //ר�Ŵ����û������Ӵ����ͶϿ� ��Ϊ�ص�����(��call backs.h) epoll listenfd accept
    void onConnection(const TcpConnectionPtr&conn){//��Ա����        
        if(conn->connected()){//�ж������Ƿ�ɹ�
            cout<<conn->peerAddress().toIpPort()<<"->"<<  //������ӡ�Զ���Ϣ
        conn->localAddress().toIpPort()<<"state:online"<<endl;
        }
        else{
            cout<<conn->peerAddress().toIpPort()<<"->"<<  //������ӡ�Զ���Ϣ
        conn->localAddress().toIpPort()<<"state:offline"<<endl;
        conn->shutdown();//close(fd) ������Դ
        //-loop->quit();//һ���̶߳��˽�����ϵ�
        }
    }
    //ר�Ŵ����û��Ķ�д�¼�
    void onMessage(const TcpConnectionPtr& conn,//���� ͨ�����Ӷ����� д����
                            Buffer* buffer,//������
                            Timestamp time)//���յ����ݵ�ʱ����Ϣ
    {
        string buf=buffer->retrieveAllAsString();//��װ�˻��������� �����ݷŵ��ַ�����
        cout<<"recv data:"<<buf<<"time:"<<time.toString()<<endl;//��ʱ����Ϣת�����ַ���
        conn->send(buf);

    }
    TcpServer _server;//#1
    EventLoop *_loop;//#2 epoll ע�����Ȥ�¼�

};

int main()
{
    EventLoop loop;//epoll
    InetAddress addr("127.0.0.1",6000);
    ChatServer server(&loop,addr,"ChatServer");//�ȴ��¼�ѭ����ַ���ٴ��󶨵�ַ���ٴ�����������
    server.start();//�������� listenfd epoll_ctl=epoll
    loop.loop();//epoll_wait��������ʽ�ȴ����û����ӣ��������û��Ķ�д�¼���
    return 0;
}