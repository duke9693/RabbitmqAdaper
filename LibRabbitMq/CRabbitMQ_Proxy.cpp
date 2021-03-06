#include "CRabbitMQ_Proxy.h"
#include "amqp_tcp_socket.h"
#include <string>
#include <vector>
#include <iostream>
#include <vector>

#if defined (__linux)
#include <system.h>
#elif defined (WIN32)
#include  <windows.h>
#endif
using namespace std;

#include "RabbitMQ_Adapter.h"

CRabbitMQ_Proxy::CRabbitMQ_Proxy(string HostName, uint32_t port, string usr, string psw)
{
	this->adapter = new CRabbitMQ_Adapter(HostName, port, usr, psw);
}

CRabbitMQ_Proxy::~CRabbitMQ_Proxy()
{
	delete this->adapter;
}

int32_t CRabbitMQ_Proxy::Disconnect(string &ErrorReturn)
{
	return this->adapter->Disconnect(ErrorReturn);
}

int32_t CRabbitMQ_Proxy::Connect(string &ErrorReturn)
{
	return this->adapter->Connect(ErrorReturn);
}

//step1 declare an exchange
int32_t CRabbitMQ_Proxy::exchange_declare(CExchange &exchange, string &ErrorReturn)
{
	return this->adapter->exchange_declare(exchange, ErrorReturn);
}

//step2 declare a queue
int32_t CRabbitMQ_Proxy::queue_declare(CQueue &queue, string &ErrorReturn)
{
	return this->adapter->queue_declare(queue, ErrorReturn);
}

//step3 bind
int32_t CRabbitMQ_Proxy::queue_bind(CQueue &queue, CExchange &exchange, const string bind_key, string &ErrorReturn)
{
	return this->adapter->queue_bind(queue, exchange, bind_key, ErrorReturn);
}

int32_t  CRabbitMQ_Proxy::queue_unbind(CQueue &queue, CExchange &exchange, const string bind_key, string &ErrorReturn)
{
	return this->adapter->queue_unbind(queue, exchange, bind_key, ErrorReturn);
}

//step 4 publish message
int32_t CRabbitMQ_Proxy::publish(vector<CMessage> &message, string routekey, string &ErrorReturn)
{
	return this->adapter->publish(message, routekey, ErrorReturn);
}

int32_t CRabbitMQ_Proxy::publish(CMessage &message, string routkey, string &ErrorReturn)
{
	return this->adapter->publish(message, routkey, ErrorReturn);
}

int32_t CRabbitMQ_Proxy::publish(const string &message, string routekey, string &ErrorReturn)
{
	return this->adapter->publish(message, routekey, ErrorReturn);
}

int32_t CRabbitMQ_Proxy::getMessageCount(const CQueue &queue, string &ErrorReturn)
{
	return this->adapter->getMessageCount(queue, ErrorReturn);
}

int32_t CRabbitMQ_Proxy::getMessageCount(const string &queuename, string &ErrorReturn)
{
	return this->adapter->getMessageCount(queuename, ErrorReturn);
}

int32_t CRabbitMQ_Proxy::queue_delete(const string queuename, int32_t if_unused, string &ErrorReturn)
{
	return this->adapter->queue_delete(queuename, if_unused, ErrorReturn);
}
//返回0是成功 否则全是失败
int32_t CRabbitMQ_Proxy::consumer(CQueue &queue, vector<CMessage> &message, uint32_t GetNum, struct timeval *timeout, string &ErrorReturn)
{
	return this->adapter->consumer(queue, message, GetNum, timeout, ErrorReturn);
}
int32_t CRabbitMQ_Proxy::consumer(const string & queue_name, vector<string> &message_array, uint32_t GetNum, struct timeval *timeout, string &ErrorReturn)
{
	return this->adapter->consumer(queue_name, message_array, GetNum, timeout, ErrorReturn);
}

void CRabbitMQ_Proxy::setUser(const string UserName)
{
	this->adapter->setUser(UserName);
}

string CRabbitMQ_Proxy::getUser() const
{
	return this->adapter->getUser();
}

void CRabbitMQ_Proxy::setPassword(const string password)
{
	this->adapter->setPassword(password);
}
string CRabbitMQ_Proxy::getPassword() const
{
	return  this->adapter->getPassword();
}

void CRabbitMQ_Proxy::__sleep(uint32_t millsecond)
{
	this->adapter->__sleep(millsecond);
}