/******************************************************************** 
创建时间:        2015/06/30 20:02
文件名称:        tcp_session.cpp
文件作者:        Domi
*********************************************************************/

#include "network/tcp_session.h"

CTcpSession::CTcpSession()
{
	m_apply_key = 0;
}

// 初始化
void CTcpSession::initialize()
{
	++m_apply_key;
}

void CTcpSession::SendMsg(_stNetMessage* pOutMsg)
{
	printf("warn：CTcpSession::SendMsg 需要子类实现！\n");
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

// 连接上的回调
void CTcpSession::onConnect()
{
	printf("CTcpSession::onConnect！\n");
}

// 断开连接
void CTcpSession::onDisconnect()
{
	printf("CTcpSession::onDisconnect！\n");
}