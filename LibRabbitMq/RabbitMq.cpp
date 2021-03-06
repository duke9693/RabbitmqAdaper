#include "RabbitMq.h"

#include  <iostream>

#include <windows.h>
#include <fstream>

#include <string>
#include <vector>

#include "CRabbitMQ_Proxy.h"
#include <Ctime>
#include <time.h>
#define SLEEPTIME 1000

CRabbitMq::CRabbitMq()
{
}

CRabbitMq::~CRabbitMq()
{
}

void CRabbitMq::init(ConnInfo& conninfo)
{
	m_conninfo = conninfo;
}

void CRabbitMq::consumer_thread(void* param, string strqueue)
{
	CRabbitMq* pThis = (CRabbitMq*)param;
	while (true)
	{
		if (pThis->consumerWorker(strqueue))
		{
		}
		else
			return;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

bool CRabbitMq::consumerWorker(string queuename)
{
	m_lock.lock();
	set<string>::iterator iter = m_stSubscribe.find(queuename);
	if (iter != m_stSubscribe.end())
	{
		m_lock.unlock();
		CRabbitMQ_Proxy pro(m_conninfo.strServerAddress, m_conninfo.nPort, m_conninfo.strUserName, m_conninfo.strPassword);
		string queue_name(queuename);
		::timeval tvb = { 1,10 };
		string err;
		if (pro.Connect(err) < 0)
			return true;
		vector<string> vgetmsg;
		vgetmsg.clear();
		int get_number = 10;
		if (pro.consumer(queue_name, vgetmsg, get_number, &tvb, err) < 0)
			return true;
		else
		{
			SubscribeMsg subMsg;
			subMsg.vcMsg = &vgetmsg;
			subMsg.strQueueName = queuename;
			if (m_conninfo.pfunCB != 0)
			{
				m_conninfo.pfunCB(MQ_MSGCB, &subMsg, m_conninfo.pCaller);
			}
		}
		pro.__sleep(10);
		pro.Disconnect();
		return true;
	}
	else
	{
		m_lock.unlock();
		return false;
	}
}

bool CRabbitMq::publishMsg(string exchangename, string queueName, char* pData, unsigned int nlen)
{
	string err;
	::timeval tvb = { 1,10 };
	CRabbitMQ_Proxy pro(m_conninfo.strServerAddress, m_conninfo.nPort, m_conninfo.strUserName, m_conninfo.strPassword);
	CExchange exchange(exchangename);
	string queue_name(queueName);
	CQueue queue_temp(queue_name);
	if (pro.Connect(err) < 0)
	{
		return false;
	}
	//tp1  声明一个交换机
	if (pro.exchange_declare(exchange, err) < 0)
	{
		//return false;
	}
	//声明一个队列
	if ((pro.queue_declare(queue_temp, err) < 0))
	{
		return false;
	}
	//step3 将交换机绑定到队列，
	if ((pro.queue_bind(queue_temp, exchange, queue_name, err) < 0))
	{
		return false;
	}
	string strpublishmsg;
	strpublishmsg.assign(pData, nlen);
	//将序列化之后的二进制消息放到指定路由对应的消息队列中
	if (pro.publish(strpublishmsg, queue_name, err) < 0)
	{
		pro.__sleep(10);
		pro.Disconnect();
		return false;
	}
	pro.__sleep(10);
	pro.Disconnect();
	return true;;
}

bool CRabbitMq::SubscribeMSg(string queueName)
{
	m_lock.lock();
	set<string>::iterator iter = m_stSubscribe.find(queueName);
	if (iter == m_stSubscribe.end())
	{
		m_stSubscribe.insert(queueName);
		m_lock.unlock();
		thread* pthconsumer = new(std::nothrow) thread(CRabbitMq::consumer_thread, this, queueName);
		if (pthconsumer == 0)
			return false;
		m_mpThread[queueName] = pthconsumer;
		return true;;
	}
	m_lock.unlock();
	return false;
}

bool CRabbitMq::UnSubscribeMsg(string queueName)
{
	m_lock.lock();
	m_stSubscribe.erase(queueName);
	map<string, thread*>::iterator iter = m_mpThread.find(queueName);
	if (iter != m_mpThread.end())
	{
		iter->second->detach();
		delete iter->second;
		m_mpThread.erase(queueName);
	}
	m_lock.unlock();
	return true;
}

bool CRabbitMq::GetConnState()
{
	CRabbitMQ_Proxy pro(m_conninfo.strServerAddress, m_conninfo.nPort);
	string err;
	if (pro.Connect(err) < 0)
	{
		return false;
	}
	return true;
}

ILibRabbitMqBase* CreateLibRabbitMqBase()
{
	ILibRabbitMqBase* pBase = new CRabbitMq;
	return pBase;
}
void DeleteLibRabbitMqBase(ILibRabbitMqBase* pM)
{
	if (pM != 0)
	{
		delete pM;
		pM = 0;
	}
}