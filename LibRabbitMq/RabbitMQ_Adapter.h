#pragma once

#include <string>
#include <vector>

using namespace std;

#include "amqp.h"
#include "MessageBody.h"

/**
*   @brief 消息队列工具类
*
*   class CRabbitMQ in "RabbitMQ.h"
**/
class CRabbitMQ_Adapter
{
private:
	string                  m_hostName;    //消息队列主机
	uint32_t                m_port;        //消息队列端口
	amqp_socket_t           *m_sock;
	amqp_connection_state_t m_conn;
	string					m_user;
	string					m_psw;
	uint32_t				m_channel;

	string m_routkey;
	CExchange *m_exchange;
	CQueue    *m_queue;

public:
	CRabbitMQ_Adapter(string HostName = "192.168.203.129", uint32_t port = 5672, string usr = "guest", string psw = "guest");
	//析构函数
	~CRabbitMQ_Adapter();

	int32_t Connect(string &ErrorReturn = string(""));

	int32_t Disconnect(string &ErrorReturn = string(""));

	int32_t exchange_declare(CExchange &exchange, string &ErrorReturn = string(""));

	int32_t queue_declare(CQueue &queue, string &ErrorReturn = string(""));

	int32_t queue_bind(CQueue &queue, CExchange &exchange, const string bind_key, string &ErrorReturn = string(""));

	int32_t queue_unbind(CQueue &queue, CExchange &exchange, const string bind_key, string &ErrorReturn = string(""));

	int32_t publish(vector<CMessage> &message, string routkey, string &ErrorReturn = string(""));

	int32_t publish(CMessage &message, string routkey, string &ErrorReturn = string(""));

	int32_t publish(const string &message, string routkey, string &ErrorReturn = string(""));
	int32_t consumer(CQueue &queue, vector<CMessage> &message, uint32_t GetNum = 1, struct timeval *timeout = NULL, string &ErrorReturn = string(""));
	int32_t consumer(const string & queue_name, vector<string> &message_array, uint32_t GetNum = 1000, struct timeval *timeout = NULL, string &ErrorReturn = string(""));

	int32_t queue_delete(const string queuename, int32_t if_unused = 0, string &ErrorReturn = string(""));

	int32_t getMessageCount(const CQueue &queue, string &ErrorReturn = string(""));
	int32_t getMessageCount(const string &queuename, string &ErrorReturn = string(""));

	void setUser(const string UserName);

	string getUser() const;

	void setPassword(const string password);

	string getPassword() const;

	void __sleep(uint32_t millsecond);

private:
	int32_t read(const string QueueName, vector<string> &message, uint32_t GetNum = 1, struct timeval *timeout = NULL, string &ErrorReturn = string(""));
	void setChannel(const uint32_t channel);
	uint32_t getChannel()const;
private:
	//返回1成功，其他是错误
	int32_t CRabbitMQ_Adapter::AssertError(amqp_rpc_reply_t x, string context, string &ErrorReturn);
	CRabbitMQ_Adapter(const CRabbitMQ_Adapter &other) //拷贝构造函数
	{
	}
	CRabbitMQ_Adapter &operator=(const CRabbitMQ_Adapter &oter) //赋值函数
	{
		return *this;
	}
};
