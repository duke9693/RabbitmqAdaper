// LibRabbitMq.cpp : 定义控制台应用程序的入口点。
//

#include "LibRabbitMq.h"
#include <iostream>
using namespace std;

void FunLibAMQP(AMQP_type type, void* pParam, void* pCaller)
{
	SubscribeMsg* msg = (SubscribeMsg*)pParam;
	for (int i = 0; i < msg->vcMsg->size(); i++)
	{
		cout << (*(*msg).vcMsg)[i].c_str() << endl;
	}
}

int main()
{
	ConnInfo info;
	info.strServerAddress = "192.168.203.129";
	info.nPort = 5672;
	info.strUserName = "admin";
	info.strPassword = "admin";
	info.pfunCB = FunLibAMQP;
	ILibRabbitMqBase* mq_publisher = CreateLibRabbitMqBase();
	mq_publisher->init(info);
	for (int i = 0; i < 100; i++)
	{
		string str = "hello world ";
		str += std::to_string(i);
		mq_publisher->publishMsg("exchange", string("queue"), (char*)str.c_str(), str.length());
	}
	mq_publisher->SubscribeMSg("queue");
	while (true)
	{
		char d = getchar();
		if (d == 's')
		{
			mq_publisher->UnSubscribeMsg("queue");
		}
	}
	return 0;
}