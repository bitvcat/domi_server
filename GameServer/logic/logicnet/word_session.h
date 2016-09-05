/******************************************************************** 
创建时间:        2015/09/02 9:55
文件名称:        word_session.h
文件作者:        Domi
功能说明:        word server 会话。  
其他说明:         
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

	void initialize();		// 初始化

	void disconnect();		// 断开连接
	void onDisconnect();	// 断开连接的回调，这个时候可以做玩家下线处理
	void SendMsg(_stNetMessage* pOutMsg);	// 发送消息

private:
	CTcpConnect* m_pGameConnect;	// game connect
};