/******************************************************************** 
����ʱ��:        2015/07/16 19:42
�ļ�����:        client_session.cpp
�ļ�����:        Domi
*********************************************************************/

#include "logic/logicnet/server_session.h"
#include "common/log/log.h"

void CServerSession::bindContext(CTcpContext* pContext)
{
	if (m_pContext){
		m_pContext = pContext;
	}
	else{
		CLog::error("[CClientSession],bindContextʧ�ܣ�û�пͻ��������ġ���");
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
	printf("----------->> CNetClient::onConnect��\n");
}

void CServerSession::onDisconnect()
{
	//player.logout();

	// ��Ҫ������io�̴߳����ʱ���ϲ��߼�
	/*if (m_pPlayer)
	{
	m_pPlayer->logout();
	}*/
	
	//printf("CNetClient::onDisconnect��\n");
	initialize();	// ��0������������
}

void CServerSession::initialize()
{
	CTcpSession::initialize();

	// ������һЩ�ϲ����ݵ����
}

void CServerSession::SendMsg(_stNetMessage* pOutMsg)
{
	if (m_pContext){
		m_pContext->send(pOutMsg->m_buffer, pOutMsg->m_size);
	}
}