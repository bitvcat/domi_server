/******************************************************************** 
����ʱ��:        2015/09/02 9:55
�ļ�����:        word_session.h
�ļ�����:        Domi
����˵��:        word server �Ự��  
����˵��:         
*********************************************************************/

#pragma once
#include "network/tcp_session.h"
#include "network/tcp_connect.h"

class CWordSession : public CTcpSession
{
public:
	CWordSession() {}
	~CWordSession() {}

public:
	inline void BindConnect(CTcpConnect* pConnect) { m_pGameConnect = pConnect; }

	void initialize();		// ��ʼ��

	void disconnect();		// �Ͽ�����
	void onDisconnect();	// �Ͽ����ӵĻص������ʱ�������������ߴ���
	void SendMsg(_stNetMessage* pOutMsg);	// ������Ϣ

private:
	CTcpConnect* m_pGameConnect;	// game connect
};