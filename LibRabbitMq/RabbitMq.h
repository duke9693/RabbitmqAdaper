#pragma once

#include "LibRabbitMq.h"

#include <set>
#include <iostream>       // std::cout
#include <chrono>         // std::chrono::milliseconds
#include <mutex>
#include <thread>
#include <map>

using namespace  std;

class CRabbitMq : public ILibRabbitMqBase
{
public:
	CRabbitMq();
	virtual ~CRabbitMq();
	virtual void init(ConnInfo& conninfo);
	virtual bool publishMsg(string exchangename, string queueName, char* pData, unsigned int nlen);
	virtual bool SubscribeMSg(string queueName);
	virtual bool UnSubscribeMsg(string queueName);
	virtual bool GetConnState();
private:
	static void consumer_thread(void* param, string strqueue);
	bool consumerWorker(string queuename);
	set<string> m_stSubscribe;
	mutex		m_lock;
	ConnInfo	m_conninfo;
	map<string, thread*> m_mpThread;
};
