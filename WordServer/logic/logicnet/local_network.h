/******************************************************************** 
创建时间:        2015/07/06 16:47
文件名称:        local_network.h
文件作者:        Domi
功能说明:        游服本地网络server
其他说明:        处理连接上的tcp连接，然后交给应用层处理，
				 可以是client连接，也可以是内部的其他服务器。
				 注意：一个进程只有一个内部tcpserver，用来处理连接请求。
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
	virtual CMutex*		 ConnectLock()							{ return &(m_mapClient.getLock()); }				// m_mapClient的线程锁
	virtual CTcpSession* ConnectFind(CTcpContext* pContext)		{ return m_mapClient.find(pContext); }				// 查找一个客户端连接
	virtual void		 ConnectDel(CTcpContext* pContext)		{ if (pContext) m_mapClient.release(pContext); }	// 删除一个客户端连接
	
	virtual CTcpSession* ConnectNew(CTcpContext* pContext);
	virtual CServerSession* GetClient(CTcpContext*_context, uint32 _key);

	void Shutdown();		// 关闭tcpserver
private:
	POOL_CLIENT	m_mapClient;
};
