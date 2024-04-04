#include "db.h"
#include <muduo/base/Logging.h>

// ���ݿ�������Ϣ
static string server = "127.0.0.1";
static string user = "root";
static string password = "123456";
static string dbname = "chat";//���ݿ�����

MySQL::MySQL()
{
    _conn = mysql_init(nullptr);
}
// �ͷ����ݿ�������Դ
MySQL::~MySQL()
{
    if (_conn != nullptr)
    mysql_close(_conn);
}
// �������ݿ�
bool MySQL::connect()
{
    MYSQL *p = mysql_real_connect(_conn, server.c_str(), user.c_str(),
    password.c_str(), dbname.c_str(), 3306, nullptr, 0);
    if (p != nullptr)
    {
        //c��c++����Ĭ�ϵı����ַ���ASCII��������ǣ���mysql��������������ʾ?
        mysql_query(_conn, "set names gbk");
        LOG_INFO<<"connect mysql success";
    }
    else
    {
        LOG_INFO<<"connect mysql fail";
    }
    return p;
}
// ���²���
bool MySQL::update(string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
        << sql << "����ʧ��!";
        return false;
    }
    return true;
}
// ��ѯ����
MYSQL_RES* MySQL::query(string sql)
{
    if (mysql_query(_conn, sql.c_str()))
    {
    LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
    << sql << "��ѯʧ��!";
    return nullptr;
}
    return mysql_use_result(_conn);
}

// ��ȡ����
MYSQL* MySQL::getConnection()
{
    return _conn;
}