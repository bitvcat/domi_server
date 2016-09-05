/******************************************************************** 
����ʱ��:        2015/07/10 15:50
�ļ�����:        domiServer.h
�ļ�����:        Domi
����˵��:        ��Ϸ������  
����˵��:         
*********************************************************************/

#include "common/singleton/singleton.h"
#include "common/serverroot/serverRoot.h"
#include "network/tcp_connect.h"
#include "logic/logicnet/local_network.h"
#include "logic/logicnet/word_session.h"


class CDomiServer : public Singleton<CDomiServer>
				  , public CServerRoot
{
public:
	CDomiServer();
	~CDomiServer();

public:
	void ShowServerInfo();

	bool initialize(uint16 uServerID);	// ��ʼ��server
	bool startServices();				// ����server	
	void timerProcess();				// timer�����̣߳��߼������ﴦ��
	bool canExitServices();				// �ܷ��˳�,������������ز���
	void stopServices();				// �ر�server
	void denyServices();				// �ܾ�server

public:
	CLocalNetwork m_netServer;			// ����clien���ӵ�tcpserver
	CTcpConnect m_connect;				// �������������ڲ�������
	CWordSession m_wordSess;			// word server session
};
