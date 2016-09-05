/******************************************************************** 
����ʱ��:        2015/07/06 16:47
�ļ�����:        local_network.h
�ļ�����:        Domi
����˵��:        �η���������server
����˵��:        ���������ϵ�tcp���ӣ�Ȼ�󽻸�Ӧ�ò㴦��
				 ������client���ӣ�Ҳ�������ڲ���������������
				 ע�⣺һ������ֻ��һ���ڲ�tcpserver������������������
*********************************************************************/

#include "network/tcp_server.h"
#include "network/tcp_context.h"
#include "common/pool/map_pool.h"
#include "logic/logicnet/server_session.h"

class CLocalNetwork: public CTcpServer
{
public:
	CLocalNetwork() {}
	~CLocalNetwork() {}

private:
	typedef CMapPool<CTcpContext*, CServerSession>	POOL_CLIENT;

public:
	virtual CMutex*		 ConnectLock()							{ return &(m_mapClient.getLock()); }				// m_mapClient���߳���
	virtual CTcpSession* ConnectFind(CTcpContext* pContext)		{ return m_mapClient.find(pContext); }				// ����һ���ͻ�������
	virtual void		 ConnectDel(CTcpContext* pContext)		{ if (pContext) m_mapClient.release(pContext); }	// ɾ��һ���ͻ�������
	
	virtual CTcpSession* ConnectNew(CTcpContext* pContext);
	virtual CServerSession* GetClient(CTcpContext*_context, uint32 _key);

	void Shutdown();		// �ر�tcpserver
private:
	POOL_CLIENT	m_mapClient;
};
