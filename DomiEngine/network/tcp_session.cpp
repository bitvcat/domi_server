/******************************************************************** 
����ʱ��:        2015/06/30 20:02
�ļ�����:        tcp_session.cpp
�ļ�����:        Domi
*********************************************************************/

#include "network/tcp_session.h"

CTcpSession::CTcpSession()
{
	m_apply_key = 0;
}

// ��ʼ��
void CTcpSession::initialize()
{
	++m_apply_key;
}

void CTcpSession::SendMsg(_stNetMessage* pOutMsg)
{
	printf("warn��CTcpSession::SendMsg ��Ҫ����ʵ�֣�\n");
}

void CTcpSession::onKickOut()
{

}

pc_str CTcpSession::remote_ip()
{
	return nullptr;
}

ulong CTcpSession::remote_address()
{
	return 0;
}

// �����ϵĻص�
void CTcpSession::onConnect()
{
	printf("CTcpSession::onConnect��\n");
}

// �Ͽ�����
void CTcpSession::onDisconnect()
{
	printf("CTcpSession::onDisconnect��\n");
}