/******************************************************************** 
����ʱ��:        2015/06/22 14:07
�ļ�����:        main.cpp
�ļ�����:        Domi
����˵��:        main   
����˵��:         
*********************************************************************/

#include <iostream>
#include "wordServer.h"

int main(int argc, char* argv[])
{
	if (!CWordServer::GetSingleton().initialize(7))
	{
		CServerRoot::messageBoxOK("CGameServer", "CGameServer::Initialize()ʧ��!");
		return 0;
	}
	
	CLog::info("���߳�id = [%d]", CTcpThread::getCurrentThreadID());
	if (CWordServer::GetSingleton().startServices())
		CWordServer::GetSingleton().maintenances();
	else
		CServerRoot::messageBoxOK("��������ʧ��", ":StartServices()��������ʧ��!");

	CWordServer::GetSingleton().stopServices();
	return 0;
}