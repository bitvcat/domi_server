/******************************************************************** 
����ʱ��:        2015/07/06 16:46
�ļ�����:        tcp_session.h
�ļ�����:        Domi
����˵��:        ��װ�Ŀͻ�������,ʵ���˴���ͻ����������Ӧ������
����˵��:        ע�⣺����ֻ�ǳ�����������ӣ���Ҫ���ϲ��߼��ľ������
				 ���磺gameserver��player������Ҫ������������ϡ�
*********************************************************************/

#pragma once
#include "network/tcp_session.h"
#include "network/tcp_context.h"

class CClientSession : public CTcpSession
{
public:
	CClientSession() {}
	~CClientSession() {}

public:
	void bindContext(CTcpContext* pContext);	// ��context

	void initialize();							// ��ʼ��
	void bindPlayer();							// ����Ҷ��󣬻�����������

	void disconnect();							// �Ͽ�����
	void onDisconnect();						// �Ͽ����ӵĻص������ʱ�������������ߴ���
	void SendMsg(_stNetMessage* pOutMsg);		// ������Ϣ

private:
	CTcpContext* m_pContext;	// �ͻ���������
};