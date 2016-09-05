/******************************************************************** 
创建时间:        2015/07/06 16:48
文件名称:        local_network.cpp
文件作者:        Domi
*********************************************************************/

#include "logic/logicnet/local_network.h"

CTcpSession* CLocalNetwork::ConnectNew(CTcpContext* pContext)
{
	if (!pContext)
		return nullptr;

	CServerSession* pClient = m_mapClient.allocate(pContext);
	if (pClient)
	{
		pClient->initialize();
		pClient->bindContext(pContext);
	}

	return pClient;
}

CServerSession* CLocalNetwork::GetClient(CTcpContext*_context, uint32 _key)
{
	CCritLocker clLock(m_mapClient.getLock());
	CServerSession* pClient = m_mapClient.find(_context);
	if (pClient)
	{
		//pClient->setApplyLock(true);
		return pClient;
	}

	return nullptr;
}

void CLocalNetwork::Shutdown()
{
	POOL_CLIENT::iterator it = m_mapClient.begin();
	for (; it != m_mapClient.end(); ++it){
		it->first->disablebev();
	}

	this->StopServer();
}