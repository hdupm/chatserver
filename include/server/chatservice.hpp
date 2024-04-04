#ifndef CHATSERVICE_H  //��ֹͷ�ļ��ظ�����
#define CHATSERVICE_H
#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <mutex>
//#include <memory>

#include "redis.hpp"
#include "groupmodel.hpp"
#include "friendmodel.hpp"
#include "json.hpp"
#include "offlinemessagemodel.hpp"
#include "usermodel.hpp"
using namespace std;
using namespace muduo;
using namespace muduo::net;
using json=nlohmann::json;

//��ʾ������Ϣ���¼��ص���������
using MsgHandler=std::function<void(const TcpConnectionPtr& conn,json &js,Timestamp)>;

//���������ҵ����
class ChatService
{
public:
    //��ȡ��������Ľӿں���
    static ChatService *instance();
    //�����¼ҵ��
    void login(const TcpConnectionPtr& conn,json &js,Timestamp);
    //����ע��ҵ��
    void reg(const TcpConnectionPtr& conn,json &js,Timestamp);
    //һ��һ����ҵ��
    void oneChat(const TcpConnectionPtr &conn,json &js,Timestamp time);
    // ��Ӻ���ҵ��
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // ����Ⱥ��ҵ��
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // ����Ⱥ��ҵ��
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // Ⱥ������ҵ��
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // ����ע��ҵ��
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //��ȡ��Ϣ��Ӧ�Ĵ�����
    MsgHandler getHandler(int msgid);
    //����ͻ����쳣�˳�
    void clientCloseException(const TcpConnectionPtr &conn);
    //�������쳣��ҵ�����÷���
    void reset();
    // ��redis��Ϣ�����л�ȡ���ĵ���Ϣ
    void handleRedisSubscribeMessage(int, string);

private:
    ChatService();
    //�洢��Ϣid��������Ӧ��ҵ������
    unordered_map<int,MsgHandler> _msgHandlerMap;//��Ϣ��������

    //�洢�����û���ͨ������ ��Ҫ�����̰߳�ȫ������������������
    unordered_map<int,TcpConnectionPtr> _userConnMap;
    //���廥��������֤_userConnMap���̰߳�ȫ
    mutex _connMutex;

    //���ݲ��������
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;

    //redis��������
    Redis _redis;

};

#endif


