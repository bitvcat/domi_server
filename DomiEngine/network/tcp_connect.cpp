/******************************************************************** 
����ʱ��:        2015/09/01 16:30
�ļ�����:        tcp_connect.cpp
�ļ�����:        Domi
*********************************************************************/

#include "network/tcp_connect.h"
#include "common/basic/memoryFunctions.h"
#include "common/log/log.h"

CTcpConnect::CTcpConnect()
	: m_event_base(nullptr)
	, m_bufferev(nullptr)
	, m_pSession(nullptr)
	, m_inbufLen(0)
	, m_readBegin(0)
{
	
}

CTcpConnect::~CTcpConnect()
{
	m_inbufLen = 0;
	m_readBegin = 0;

	if (m_bufferev){
		bufferevent_free(m_bufferev);
	}

	if (m_event_base){
		event_base_free(m_event_base);
	}
}

bool CTcpConnect::Initialize()
{
	m_event_base = new_event_base();
	if (m_event_base == nullptr){
		CLog::error("[CTcpConnect],event_base����ʧ�ܡ���");
		return false;
	}

	m_bufferev = bufferevent_socket_new(m_event_base, -1, BEV_OPT_CLOSE_ON_FREE | LEV_OPT_THREADSAFE);
	if (m_bufferev == nullptr){
		CLog::error("[CTcpConnect],bufferevent ����ʧ�ܡ���");
		return false;
	}

	bufferevent_setcb(m_bufferev, read_cb, NULL, event_cb, (void*)this);
	bufferevent_enable(m_bufferev, EV_READ | EV_PERSIST);

	// dispatch
	if (!m_connectThread.create(&CTcpConnect::__thread_proc, this)){
		CLog::error("[CTcpConnect],connect�̴߳���ʧ�ܡ���");
		return false;
	}

	return true;
}

bool CTcpConnect::Connect(const char* ipAddr, uint16 port)
{
	if (m_pSession== nullptr){
		CLog::error("[CTcpConnect],δ�󶨻Ự����");
		return false;
	}

	if (ipAddr == nullptr || port<1024){
		CLog::error("[CTcpConnect],Զ��ip��˿ڴ��󡭡�");
		return false;
	}

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));

	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(ipAddr);
	// ת��ip string
// 	if (evutil_inet_pton(AF_INET, ipAddr, &sin) == -1){
// 		CLog::error("[CTcpConnect],ת��ipʧ�ܡ���");
// 		return false;
// 	}

	if (bufferevent_socket_connect(m_bufferev, (struct sockaddr*)&sin, sizeof(struct sockaddr)) < 0) {
		CLog::error("[CTcpConnect],connect����Զ��serverʧ�ܡ���");
		return false;
	}

	return true;
}

void CTcpConnect::DisConnect()
{
	if (m_bufferev){
		bufferevent_lock(m_bufferev);
		bufferevent_disable(m_bufferev, EV_READ | EV_PERSIST);
		bufferevent_free(m_bufferev);
		bufferevent_unlock(m_bufferev);

		m_bufferev = nullptr;
		
		this->OnDisConnected();// �ص�
	}
}

void CTcpConnect::Shutdown()
{
	DisConnect();

	event_base_loopbreak(m_event_base);
	event_base_free(m_event_base);

	m_event_base = nullptr;

	m_connectThread.wait_exit();
}

bool CTcpConnect::ProcessPacket()
{
	while (m_inbuf && this->getPendingLen() > sizeof(PacketHead))
	{
		//һ��Э���������ͷ��û���꣬�����ѭ�������ߵȴ���һ��libeventʱ�����ѭ����
		PacketHead* pHead = (PacketHead*)(m_inbuf + m_readBegin);
		if (pHead->uPacketSize > MaxBuffLen || pHead->uPacketSize < sizeof(PacketHead)){ // ��Ϣ��ͷ���Ϸ�
			CLog::error("[CTcpConnect],��Ϣ��ͷ���Ϸ�����");
			this->DisConnect();
			return false;
		}

		if (pHead->uPacketSize > this->getPendingLen()){ // ʣ�����ݲ���һ������������
			CLog::error("[CTcpConnect],ʣ�����ݲ���һ�����������ա���");
			break;
		}

		if (m_readBegin > m_inbufLen || m_readBegin>MaxBuffLen){ // ��ȡλ�ó���
			this->DisConnect();
			return false;
		}

		// ѹ��message
		m_msgQueue.push(m_pSession->getApplyKey(), nullptr, m_inbuf + m_readBegin + sizeof(PacketHead), pHead->uPacketSize - sizeof(PacketHead));

		//������һ��Э���
		m_readBegin += pHead->uPacketSize;
	}

	// ����һ���������ݣ��ƶ�����ʼλ��
	int pending = this->getPendingLen();
	if (pending > 0)	// ����Ҫ�ƶ����ֽ�
		memmove(m_inbuf, m_inbuf + m_readBegin, pending);

	m_inbufLen = pending;
	m_readBegin = 0;

	return true;
}

bool CTcpConnect::SendMsg(const char* pData, uint32 nSize)
{
	if (!m_bufferev){
		return false;
	}

	struct evbuffer* output = bufferevent_get_output(m_bufferev);
	if (!output){
		DisConnect();
		return false;
	}
	uint32 len = evbuffer_get_length(output);
	if (evbuffer_get_length(output) >= MaxBuffLenLimit){	// ��ѹ̫����Ϣ
		CLog::error("[CTcpConnect],��ѹ̫����Ϣ��ǿ�ƹرա���");
		DisConnect();
		return false;
	}

	uint32 msgLen = sizeof(PacketHead)+nSize;
	PacketHead* head = (PacketHead*)(m_outbuf);
	head->uPacketSize = msgLen;
	head->uHeadFlag = 10;
	head->uVersion = 77;

	dMemcpy(m_outbuf + sizeof(PacketHead), sizeof(m_outbuf), pData, nSize);

	if (bufferevent_write(m_bufferev, m_outbuf, msgLen) == -1){
		DisConnect();
		return false;
	}

	return true;
}

void CTcpConnect::OnConnected()
{
	if (m_pSession){
		m_pSession->onConnect();
		SendMsg("hello",5);
	}
}

void CTcpConnect::OnDisConnected()
{
	if (m_pSession){
		m_pSession->onDisconnect();
	}
}

//////////////////////////////////////////////////////////////////////////
// static
void CTcpConnect::read_cb(struct bufferevent *bev, void *ctx)
{
	struct event_base *base = bufferevent_get_base(bev);

	CTcpConnect* pConnect = (CTcpConnect*)ctx;
	if (pConnect){
		if (base == nullptr || pConnect->m_event_base != base){
			CLog::error("[CTcpConnect],event base ����ȡ���");
			pConnect->DisConnect();
			return;
		}

		struct evbuffer* input = bufferevent_get_input(bev);
		int buffLen = evbuffer_get_length(input);	// evbuff �ڵ����ֽ���
		while (buffLen > 0)
		{
			int freeLen = pConnect->getFreeLen();
			if (freeLen <= 0){ // û�ж���Ŀռ�,�����
				if (!pConnect->ProcessPacket()){
					CLog::error("[CTcpConnect],������󣬹ر����ӡ���");
					pConnect->DisConnect();
					return;
				}
			}
			else{
				int ret = bufferevent_read(bev, pConnect->m_inbuf + pConnect->m_readBegin, freeLen);
				if (ret == -1){ // ��ȡ����
					CLog::error("[CTcpConnect],buffer ��ȡʧ�ܣ��ر����ӡ���");
					pConnect->DisConnect();
					return;
				}

				buffLen -= ret;
				pConnect->m_inbufLen += ret;
			}
		}

		if (pConnect->m_inbufLen > 0){// �Ƿ���δ��������ݣ����Խ��
			pConnect->ProcessPacket();
		}
	}
}

void CTcpConnect::event_cb(struct bufferevent *bev, short events, void *ctx)
{
	CTcpConnect* pConnect = (CTcpConnect*)ctx;

	if (events & BEV_EVENT_CONNECTED) { // ��ʾ���ӳɹ�
		CLog::warn("[CTcpConnect],���ӳɹ�����");
		pConnect->OnConnected();
		return;
	}
	else if (events & BEV_EVENT_ERROR) { // ����
		CLog::error("[CTcpConnect],BEV_EVENT_ERROR ���󡭡�");
	}else if (events & BEV_EVENT_EOF) {	// �Ͽ�����
		CLog::error("[CTcpConnect],BEV_EVENT_EOF �Ͽ����ӡ���");
	}

	pConnect->DisConnect();
}

THREAD_RETURN CTcpConnect::__thread_proc(void* pParam)
{
	CTcpConnect* pTcpConnect = reinterpret_cast<CTcpConnect*>(pParam);
	if (pTcpConnect == NULL)
		return -1;

	CLog::info("[connect]�߳̿�ʼ�������߳�id = [%d]����", CTcpThread::getCurrentThreadID());
	event_base_loop(pTcpConnect->m_event_base, EVLOOP_NO_EXIT_ON_EMPTY);
	CLog::error("[connect]�̹߳رա���");
	return ::GetCurrentThreadId();
}