/******************************************************************** 
����ʱ��:        2015/07/16 19:42
�ļ�����:        client_session.cpp
�ļ�����:        Domi
*********************************************************************/

#include "logic/logicnet/client_session.h"
#include "common/log/log.h"

void CClientSession::bindContext(CTcpContext* pContext)
{ 
	if (pContext){
		m_pContext = pContext;
	}
	else{
		CLog::error("[CClientSession],bindContextʧ�ܣ�û�пͻ��������ġ���");
	}
}

void CClientSession::disconnect()
{
	if (m_pContext){
		m_pContext->disconnect();
	}
}

void CClientSession::onDisconnect()
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

void CClientSession::initialize()
{
	CTcpSession::initialize();

	// ������һЩ�ϲ����ݵ����
	m_pContext = nullptr;
}

void CClientSession::SendMsg(_stNetMessage* pOutMsg)
{
	if (m_pContext){
		m_pContext->send(pOutMsg->m_buffer, pOutMsg->m_size);
	}
}