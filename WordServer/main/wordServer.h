/******************************************************************** 
����ʱ��:        2015/07/10 15:50
�ļ�����:        domiServer.h
�ļ�����:        Domi
����˵��:        ��Ϸ������  
����˵��:         
*********************************************************************/

#include "common/singleton/singleton.h"
#include "common/serverroot/serverRoot.h"
#include "logic/logicnet/local_network.h"

class CWordServer : public Singleton<CWordServer>
				  , public CServerRoot
{
public:
	CWordServer();
	~CWordServer();

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
};
