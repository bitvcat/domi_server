/******************************************************************** 
����ʱ��:        2015/06/30 20:02
�ļ�����:        tcp_content.h
�ļ�����:        Domi
����˵��:        tcp session ���� 
����˵��:        �Ự���࣬�����ͻ��˻Ự���ڲ��������Ự��
*********************************************************************/

#pragma once
#include "network/libevent.h"
#include "network/net_message.h"
#include "common/thread/csLocker.h"

class CTcpSession
{
public:
	CTcpSession();
	~CTcpSession() {}

public:
	virtual void initialize();
	virtual void disconnect() = 0;
	virtual pc_str remote_ip();
	virtual ulong remote_address();

	// �ص�
	virtual void onKickOut();
	virtual void onConnect();		// �����ϵĻص�
	virtual void onDisconnect();	// �Ͽ�����
	virtual void SendMsg(_stNetMessage* pOutMsg) = 0;	// ��Ҫ����ʵ��

public:
	// ��������
	//inline void	bindPlayer(){ }
	inline CMutex* getApplyMutex()					{ return &m_apply_mutex; }
	inline uint32  getApplyKey()					{ return m_apply_key;    }
	inline void	setApplyLock(bool _lock)			{ if (_lock)m_apply_mutex.lock(); else m_apply_mutex.unlock();}

private:
	uint32		m_apply_key;	//����key,ȷ����������
	CMutex		m_apply_mutex;	//ȷ���̰߳�ȫ
};