/******************************************************************** 
创建时间:        2015/07/16 19:42
文件名称:        client_session.cpp
文件作者:        Domi
*********************************************************************/

#include "logic/logicnet/server_session.h"
#include "common/log/log.h"

void CServerSession::bindContext(CTcpContext* pContext)
{
	if (m_pContext){
		m_pContext = pContext;
	}
	else{
		CLog::error("[CClientSession],bindContext失败，没有客户端上下文……");
	}
}

void CServerSession::disconnect()
{
	if (m_pContext){
		m_pContext->disconnect();
	}
}

void CServerSession::onConnect()
{
	printf("----------->> CNetClient::onConnect！\n");
}

void CServerSession::onDisconnect()
{
	//player.logout();

	// 不要在网络io线程处理耗时的上层逻辑
	/*if (m_pPlayer)
	{
	m_pPlayer->logout();
	}*/
	
	//printf("CNetClient::onDisconnect！\n");
	initialize();	// 清0，用来做复用
}

void CServerSession::initialize()
{
	CTcpSession::initialize();

	// 其他的一些上层数据的清空
}

void CServerSession::SendMsg(_stNetMessage* pOutMsg)
{
	if (m_pContext){
		m_pContext->send(pOutMsg->m_buffer, pOutMsg->m_size);
	}
}