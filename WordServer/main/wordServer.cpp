/******************************************************************** 
创建时间:        2015/06/22 14:44
文件名称:        GameServer.cpp
文件作者:        Domi
*********************************************************************/

#include <iostream>
#include "wordServer.h"

// 处理消息
static uint64 STnowTime = getSecond();

CWordServer::CWordServer()
{
#ifdef WIN32
	dSprintf(m_szEventExist, sizeof(m_szEventExist), "ExistEvent_Word");
#endif
}

CWordServer::~CWordServer()
{
	
}

void CWordServer::ShowServerInfo()
{
	showToConsole("***************************************************************");
	showToConsole("* Word Server Application Ver %d.%d", 0, 0);
	showToConsole("***************************************************************");

	setServicesTitle("Word:<%.4d>", 1);
}

// 初始化server
bool CWordServer::initialize(uint16 uServerID)
{
	ShowServerInfo();

	if (!CServerRoot::initialize(uServerID)){
		return false;
	}

	if (!m_netServer.Initialize(2, 27777)){
		return false;
	}

	return true;
}

// 启动server
bool CWordServer::startServices()
{
	CServerRoot::startServices();
	
	if (!m_netServer.StartServer()){
		return false;
	}

	return true;
}

// timer工作线程，逻辑在这里处理
void CWordServer::timerProcess()
{
	_stNetMessage* pMsg = nullptr;
	m_netServer.m_clMessageQueue.swap_queue();
	while (pMsg = m_netServer.m_clMessageQueue.front()){
		if (pMsg){
			CTcpSession* client = m_netServer.GetClient(pMsg->_context, pMsg->_apply_key);
			if (client){
				//printf("msg=%s\n", pMsg->m_buffer);
				client->SendMsg(pMsg);
				//client->setApplyLock(false);
			}
		}
		m_netServer.m_clMessageQueue.pop();
	}
}

// 能否退出,这里做数据落地操作
bool CWordServer::canExitServices()
{
	showToConsole("<Waiting for shutdown ...>");
	//g_pGamePlayerMgr->shutdown();
	// 各种逻辑层的关闭以及数据落地

	return true;
}

// 关闭server
void CWordServer::stopServices()
{
	m_netServer.Shutdown();
	dSleep(2000);
	CServerRoot::stopServices();

	// 各种底层的shutdown
	CLog::shutdown();
}

// 拒绝server
void CWordServer::denyServices()
{
	showToConsole("<Deny service ...>");

	CLog::warn("开始拒绝世界网络链接...");
	m_netServer.RejectListen();
	m_netServer.RejectRead();
}