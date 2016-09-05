/******************************************************************** 
����ʱ��:        2015/09/01 16:24
�ļ�����:        tcp_connect.h
�ļ�����:        Domi
����˵��:        tcp connection ���� 
����˵��:        ������������ʱ��connection�ķ�װ��
*********************************************************************/

#pragma once
#include "network/libevent.h"
#include "network/net_message.h"
#include "common/thread/thread.h"
#include "network/tcp_session.h"
#include "common/thread/csLocker.h"
#include "tcp_context.h"
#include "message_queue.h"

class CTcpConnect
{
public:
	CTcpConnect();
	~CTcpConnect();

public:
	// inline
	inline int getPendingLen()	{ return m_inbufLen - m_readBegin; }	// ʣ��δ������ֽ���
	inline int getFreeLen()		{ return MaxBuffLen - m_inbufLen; }		// ����append�ĳ���
	inline void BindSession(CTcpSession* pSession) { m_pSession = pSession; }	// �󶨻Ự����

	// virtual
	virtual bool Initialize();								// ��ʼ��
	virtual bool Connect(const char* ipAddr,uint16 port);	// ��������Զ�̷�����
	virtual void DisConnect();								// �Ͽ�����
	virtual void Shutdown();								// �ر�����
	
	// method
	bool ProcessPacket();									// �������
	bool SendMsg(const char* pData,uint32 nSize);			// ������Ϣ
	void OnConnected();										// �������Ӻ�Ļص�
	void OnDisConnected();									// �Ͽ����Ӻ�Ļص�

private:
	static void read_cb(struct bufferevent *bev, void *ctx);					// ��
	static void event_cb(struct bufferevent *bev, short events, void *ctx);		// buffervent ������
	static THREAD_RETURN __thread_proc(void* pParam);							// event base �����߳�

public:
	CMessageQueue m_msgQueue;		// ���յ�����Ϣ����

private:
	char m_szIpAddr[128];			// Զ��server ip��ַ

	event_base* m_event_base;		// event_base ��Ӧ��
	bufferevent* m_bufferev;		// buffer event
	CThread m_connectThread;		// connect �����߳�

	char m_inbuf[MaxBuffLen];		// buffer���棬����Ļ��������ְ�
	char m_outbuf[MaxBuffLen];		// buffer���棬����Ļ��������
	int m_inbufLen;					// inbuff ����λ��
	int m_readBegin;				// ��һ�ο�ʼ��ȡ��λ��

	CTcpSession* m_pSession;		// �Ự����
};