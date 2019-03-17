#pragma once

#include <map>
#include <list>

#include <boost/shared_ptr.hpp>

#include "base_define.h"
#include "tcp_client.h"

using std::map;
using std::list;

namespace QCOOL
{

using boost::shared_ptr;

typedef shared_ptr<CTcpClient> CTcpClientPtr;

typedef std::map<TServerId,CTcpClientPtr>::iterator ItrCli;

class CClientManager
{
public :
	CClientManager()
		:nClientCount(0)
	{

	}

    void SendMessage(TServerId srvid,stMessage *msg)
    {
    	MutexLockGuard gurad(m_mutex);
    	ItrCli pos = m_clientMap.find(srvid);
    	if (pos != m_clientMap.end())
    	{
    		pos->second->SendMessage(msg);
    	}
    }

    void ReceiveMessage(TMessageQueue &lst)
    {
    	std::map<TServerId,CTcpClientPtr> temp;
    	{
			MutexLockGuard gurad(m_mutex);
			temp = m_clientMap;
    	}
    	//get message;
    	ItrCli pos = m_lstClient.begin();
    	while (pos != m_lstClient.end())
    	{
    		pos->second->ReceiveMessage(lst);
    		++pos;
    	}
    }

    void RcvEvtFromClient(EventList &lstEvt)
    {
    	std::map<TServerId,CTcpClientPtr> temp;
    	{
			MutexLockGuard gurad(m_mutex);
			temp = m_clientMap;
    	}
    	//get event;
    	ItrCli pos = m_lstClient.begin();
    	while (pos != m_lstClient.end())
    	{
    		pos->second->ReceiveEvent(lstEvt);
    		++pos;
    	}
    }
    //by other thread;
    bool addClient(TServerId srvid,const std::string &srvip,uint16_t srvport)
    {
    	if (m_lstClient.find(srvid) != m_lstClient.end())
    	{
    		//DEBUG("Server id [%d] has exist!",srvid);
    		return false;
    	}
    	CTcpClientPtr ptr(new CTcpClient(srvip,srvport,srvid));
    	assert(ptr);
    	m_lstClient[srvid] = ptr;
    	ptr->Start();
		nClientCount++;
    }

public ://交给外部接口;用作连接事件.
    void OnConnection(TServerId srvid)
    {
    	MutexLockGuard gurad(m_mutex);
    	ItrCli pos = m_lstClient.find(srvid);
    	if (pos == m_lstClient.end())
    	{
    		m_clientMap[srvid] = pos->second;
    		//start message;
    		//pos->SendMessage(msg);
    	}
    }

    void OnClose(TServerId srvid)
    {
    	MutexLockGuard gurad(m_mutex);
    	ItrCli pos = m_lstClient.find(srvid);
    	if (pos != m_lstClient.end())
    	{
    		m_clientMap.erase(pos);
    	}
    }
	int64_t nClientCount;

private :
    std::map<TServerId,CTcpClientPtr> m_lstClient;
    std::map<TServerId,CTcpClientPtr> m_clientMap;
    MutexLock m_mutex;
};

}//END QCOOL;
