#pragma once

#pragma once
/*
*  Created on: 2018-08-15
*      Author: fancongchong
*/
#define  __STDC_LIMIT_MACROS
#if (defined _WIN32 || defined _WIN64)
#   ifdef FX_EXPORTE_
#       define FX_EXPORT_API  extern "C" __declspec(dllexport)
#   else
#       define FX_EXPORT_API  extern "C" __declspec(dllimport)
#   endif
#else
#   ifdef __linux__
#       define FX_EXPORT_API
#   else
#       define FX_EXPORT_API
#   endif
#endif

#include <string>
#include <vector>

using namespace std;

enum AMQP_type
{
	MQ_MSGCB
};

typedef void(*pCBFunLibAMQP)(AMQP_type type, void* pParam, void* pCaller);

struct ConnInfo
{
	string strServerAddress;
	unsigned int nPort;
	string strUserName;
	string strPassword;
	void*		pCaller;
	pCBFunLibAMQP pfunCB;
	int			nretained;			//no use
	ConnInfo()
	{
		pCaller = 0;
		pfunCB = 0;
		nretained = 0;
	}
	ConnInfo& operator=(const ConnInfo& rhs)
	{
		strServerAddress = rhs.strServerAddress;
		nPort = rhs.nPort;
		strUserName = rhs.strUserName;
		strPassword = rhs.strPassword;
		pCaller = rhs.pCaller;
		pfunCB = rhs.pfunCB;
		nretained = rhs.nretained;
		return *this;
	}
};

struct SubscribeMsg
{
	vector<string>* vcMsg;
	string strQueueName;
};

// it is a simple and ungly rabbitmq class
class ILibRabbitMqBase
{
public:
	//all the function is bolcked,it is useless to concern about the connection states, if you want to know your machine and server state,try to call GetConnState
	virtual ~ILibRabbitMqBase() {};
	virtual void init(ConnInfo& conninfo) = 0;
	//public msg and route and queue show be dealed
	virtual bool publishMsg(string exchangename, string queueName, char* pData, unsigned int nlen) = 0;
	virtual bool SubscribeMSg(string queueName) = 0;
	virtual bool UnSubscribeMsg(string queueName) = 0;
	//get connection states
	virtual bool GetConnState() = 0;
};

FX_EXPORT_API ILibRabbitMqBase* CreateLibRabbitMqBase();
FX_EXPORT_API void DeleteLibRabbitMqBase(ILibRabbitMqBase* pM);

//#include "LibRabbitMq.h"
//#include <iostream>
//using namespace std;
//
//void FunLibAMQP(AMQP_type type, void* pParam, void* pCaller)
//{
//	SubscribeMsg* msg = (SubscribeMsg*)pParam;
//	for (int i = 0; i < msg->vcMsg->size(); i++)
//	{
//		cout << (*(*msg).vcMsg)[i].c_str() << endl;
//	}
//}
//
//int main()
//{
//	ConnInfo info;
//	info.strServerAddress = "192.168.203.129";
//	info.nPort = 5672;
//	info.strUserName = "admin";
//	info.strPassword = "admin";
//	info.pfunCB = FunLibAMQP;
//	ILibRabbitMqBase* mq_publisher = CreateLibRabbitMqBase();
//	mq_publisher->init(info);
//	for (int i = 0; i < 100; i++)
//	{
//		string str = "hello world ";
//		str += std::to_string(i);
//		mq_publisher->publishMsg(string("queue"), (char*)str.c_str(), str.length());
//	}
//	mq_publisher->SubscribeMSg("queue");
//	while (true)
//	{
//		char d = getchar();
//		if (d == 's')
//		{
//			mq_publisher->UnSubscribeMsg("queue");
//		}
//	}
//	return 0;
//}