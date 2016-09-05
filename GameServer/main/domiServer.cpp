/******************************************************************** 
����ʱ��:        2015/06/22 14:44
�ļ�����:        GameServer.cpp
�ļ�����:        Domi
*********************************************************************/

#include <iostream>
#include "domiServer.h"

// ������Ϣ
static uint64 STnowTime = getSecond();
static uint32 STnum = 0;
static uint32 STCount = 0;

static void test()
{
	CLog::print("����һ��timer���� ����");
}

CDomiServer::CDomiServer()
{
#ifdef WIN32
	dSprintf(m_szEventExist, sizeof(m_szEventExist), "ExistEvent_Game");
#endif
}

CDomiServer::~CDomiServer()
{
	
}

void CDomiServer::ShowServerInfo()
{
	showToConsole("***************************************************************");
	showToConsole("* Game Server Application Ver %d.%d", 0, 0);
	showToConsole("***************************************************************");

	setServicesTitle("Game:<%.4d>", 1);
}

// ��ʼ��server
bool CDomiServer::initialize(uint16 uServerID)
{
	ShowServerInfo();

	if (!CServerRoot::initialize(uServerID)){
		return false;
	}

	if (!m_netServer.Initialize(2, 17777)){
		return false;
	}

	if (!m_connect.Initialize()){
		return false;
	}
	m_connect.BindSession(&m_wordSess);
	m_wordSess.BindConnect(&m_connect);

	return true;
}

// ����server
bool CDomiServer::startServices()
{
	CServerRoot::startServices();

	if (!m_connect.Connect("127.0.0.1", 27777)){
		return false;
	}
	
	if (!m_netServer.StartServer()){
		return false;
	}

	//CTimer::GetSingleton().addTimer(test, 100, 10);
	return true;
}

// timer�����̣߳��߼������ﴦ��
void CDomiServer::timerProcess()
{
	_stNetMessage* pMsg = nullptr;
	m_netServer.m_clMessageQueue.swap_queue();
	while (pMsg = m_netServer.m_clMessageQueue.front()){
		if (pMsg){
			CTcpSession* client = m_netServer.GetClient(pMsg->_context, pMsg->_apply_key);
			if (client){
				++STnum;
				++STCount;
				client->SendMsg(pMsg);
				//client->setApplyLock(false);
				//CLog::print("msg=%s", pMsg->m_buffer);
			}
		}
		m_netServer.m_clMessageQueue.pop();
	}

	//if ((getSecond() - STnowTime)>=60){
	//	CLog::error("tps = %d,%d", STnum / 60, STCount);
	//	STnowTime = getSecond();
	//	STnum = 0;
	//}

	// ����word server��Ϣ
	_stNetMessage* pWordMsg = nullptr;
	m_connect.m_msgQueue.swap_queue();
	while (pWordMsg = m_connect.m_msgQueue.front()){
		if (pWordMsg && (&m_wordSess != nullptr)){
			m_wordSess.setApplyLock(true);
			//CLog::print("msg=%s", pWordMsg->m_buffer);
			m_wordSess.SendMsg(pWordMsg);
			m_wordSess.setApplyLock(false);
		}

		m_connect.m_msgQueue.pop();
	}
}

// �ܷ��˳�,������������ز���
bool CDomiServer::canExitServices()
{
	showToConsole("<Waiting for shutdown ...>");
	//g_pGamePlayerMgr->shutdown();
	// �����߼���Ĺر��Լ��������

	return true;
}

// �ر�server
void CDomiServer::stopServices()
{
	m_netServer.Shutdown();
	m_connect.Shutdown();
	//dSleep(2000);
	CServerRoot::stopServices();

	// ���ֵײ��shutdown
	CLog::shutdown();
}

// �ܾ�server
void CDomiServer::denyServices()
{
	showToConsole("<Deny service ...>");

	CLog::warn("��ʼ�ܾ�������������...");
	m_netServer.RejectListen();
	m_netServer.RejectRead();
	m_connect.DisConnect();
}