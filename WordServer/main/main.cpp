/******************************************************************** 
创建时间:        2015/06/22 14:07
文件名称:        main.cpp
文件作者:        Domi
功能说明:        main   
其他说明:         
*********************************************************************/

#include <iostream>
#include "wordServer.h"

int main(int argc, char* argv[])
{
	if (!CWordServer::GetSingleton().initialize(7))
	{
		CServerRoot::messageBoxOK("CGameServer", "CGameServer::Initialize()失败!");
		return 0;
	}
	
	CLog::info("主线程id = [%d]", CTcpThread::getCurrentThreadID());
	if (CWordServer::GetSingleton().startServices())
		CWordServer::GetSingleton().maintenances();
	else
		CServerRoot::messageBoxOK("启动服务失败", ":StartServices()启动服务失败!");

	CWordServer::GetSingleton().stopServices();
	return 0;
}