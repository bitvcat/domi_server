/******************************************************************** 
����ʱ��:        2015/06/22 14:44
�ļ�����:        GameServer.cpp
�ļ�����:        Domi
*********************************************************************/

#include <iostream>
#include "wordServer.h"

// ������Ϣ
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

// ��ʼ��server
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

// ����server
bool CWordServer::startServices()
{
	CServerRoot::startServices();
	
	if (!m_netServer.StartServer()){
		return false;
	}

	return true;
}

// timer�����̣߳��߼������ﴦ��
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

// �ܷ��˳�,������������ز���
bool CWordServer::canExitServices()
{
	showToConsole("<Waiting for shutdown ...>");
	//g_pGamePlayerMgr->shutdown();
	// �����߼���Ĺر��Լ��������

	return true;
}

// �ر�server
void CWordServer::stopServices()
{
	m_netServer.Shutdown();
	dSleep(2000);
	CServerRoot::stopServices();

	// ���ֵײ��shutdown
	CLog::shutdown();
}

// �ܾ�server
void CWordServer::denyServices()
{
	showToConsole("<Deny service ...>");

	CLog::warn("��ʼ�ܾ�������������...");
	m_netServer.RejectListen();
	m_netServer.RejectRead();
}