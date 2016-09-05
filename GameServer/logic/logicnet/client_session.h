/******************************************************************** 
创建时间:        2015/07/06 16:46
文件名称:        tcp_session.h
文件作者:        Domi
功能说明:        封装的客户端连接,实现了处理客户端请求的响应函数。
其他说明:        注意：这里只是抽象出来的连接，需要绑定上层逻辑的具体对象，
				 例如：gameserver的player对象，需要绑定在这个连接上。
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
	void bindContext(CTcpContext* pContext);	// 绑定context

	void initialize();							// 初始化
	void bindPlayer();							// 绑定玩家对象，或者其他对象

	void disconnect();							// 断开连接
	void onDisconnect();						// 断开连接的回调，这个时候可以做玩家下线处理
	void SendMsg(_stNetMessage* pOutMsg);		// 发送消息

private:
	CTcpContext* m_pContext;	// 客户端上下文
};