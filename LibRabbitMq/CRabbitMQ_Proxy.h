#pragma once

#ifdef _DEBUG
#pragma comment(lib,"rabbitmq_2015MD.lib")
#else
#pragma comment(lib,"rabbitmq_2015MR.lib")

#endif // DEBUG

#define CODE_LEN				24
#define DESC_LEN				40
#define GRAP_LEN				40
#define	PASSWORD_LEN			24
#define	USER_NAME_LEN			CODE_LEN
#define	RTDB_NAME_LEN			CODE_LEN
#define HOSTNAMELEN				CODE_LEN
#define SHORTNAME_LEN			CODE_LEN
#define FULLDESCRIBE_LEN		128
#define REASONINFO_LEN			64

#include <string>
#include <vector>
#include "MessageBody.h"
using namespace std;

class CRabbitMQ_Adapter;

class  CRabbitMQ_Proxy
{
public:

	CRabbitMQ_Proxy(string HostName = "localhost", uint32_t port = 5672, string usr = "guest", string psw = "guest");
	//Îö¹¹º¯Êý
	~CRabbitMQ_Proxy();

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
	CRabbitMQ_Adapter *adapter;
};
