#include "usermodel.hpp"
#include "db.h"
#include <iostream>
using namespace std;

//���ݿ�api����
//User������ӷ��� 
bool UserModel::insert(User &user)
{
    //1����װsql���
    char sql[1024]={0};
    sprintf(sql,"insert into user(name, password, state) values('%s', '%s', '%s')",
            user.getName().c_str(),user.getPwd().c_str(),user.getState().c_str());   //id���������������
    MySQL mysql;
    if(mysql.connect()){
        if(mysql.update(sql)){
            //��ȡ����ɹ����û��������ɵ�����id mysql_insert_id�����ݿ��ṩ��c��api
            user.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    
    return false;

}

//�����û������ѯ�û���Ϣ
User UserModel::query(int id)
{
    //1����װsql���
    char sql[1024]={0};
    sprintf(sql, "select * from user where id = %d", id);
    
    MySQL mysql;
    if(mysql.connect()){
        //mysql.query(sql);
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)//��ȡһ�е�����
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row != nullptr)
            {
                User user;
                user.setId(atoi(row[0]));//��ȡһ������ĸ��ֶ�
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);
                mysql_free_result(res);//�ͷ���Դ
                return user;
            }
        }
        
    }

    return User();//����Ĭ��ֵ

}

// �����û���״̬��Ϣ
bool UserModel::updateState(User user)
{
    // 1.��װsql���
    char sql[1024] = {0};
    sprintf(sql, "update user set state = '%s' where id = %d", user.getState().c_str(), user.getId());

    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            return true;
        }
    }
    return false;
}

// �����û���״̬��Ϣ
void UserModel::resetState()
{
    // 1.��װsql���
    char sql[1024] = "update user set state = 'offline' where state = 'online'";

    MySQL mysql;
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}