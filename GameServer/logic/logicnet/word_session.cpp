/******************************************************************** 
����ʱ��:        2015/09/02 9:56
�ļ�����:        word_session.cpp
�ļ�����:        Domi
*********************************************************************/

#include "logic/logicnet/word_session.h"

void CWordSession::disconnect()
{
	if (m_pGameConnect){
		m_pGameConnect->DisConnect();
	}
}

void CWordSession::onDisconnect()
{
	initialize();	// ��0������������
}

void CWordSession::initialize()
{
	CTcpSession::initialize();

	// ������һЩ�ϲ����ݵ����
}

void CWordSession::SendMsg(_stNetMessage* pOutMsg)
{
	if (m_pGameConnect){
		m_pGameConnect->SendMsg(pOutMsg->m_buffer, pOutMsg->m_size);
	}
}