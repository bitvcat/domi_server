/******************************************************************** 
创建时间:        2015/09/02 9:56
文件名称:        word_session.cpp
文件作者:        Domi
*********************************************************************/

#include "logic/logicnet/word_session.h"

void CWordSession::disconnect()
{
	if (m_pGameConnect){
		m_pGameConnect->DisConnect();
	}
}

void CWordSession::onDisconnect()
{
	initialize();	// 清0，用来做复用
}

void CWordSession::initialize()
{
	CTcpSession::initialize();

	// 其他的一些上层数据的清空
}

void CWordSession::SendMsg(_stNetMessage* pOutMsg)
{
	if (m_pGameConnect){
		m_pGameConnect->SendMsg(pOutMsg->m_buffer, pOutMsg->m_size);
	}
}