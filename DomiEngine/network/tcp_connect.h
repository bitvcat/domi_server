/******************************************************************** 
创建时间:        2015/09/01 16:24
文件名称:        tcp_connect.h
文件作者:        Domi
功能说明:        tcp connection 基类 
其他说明:        发起主动连接时，connection的封装。
*********************************************************************/

#pragma once
#include "network/libevent.h"
#include "network/net_message.h"
#include "common/thread/thread.h"
#include "network/tcp_session.h"
#include "common/thread/csLocker.h"
#include "tcp_context.h"
#include "message_queue.h"

class CTcpConnect
{
public:
	CTcpConnect();
	~CTcpConnect();

public:
	// inline
	inline int getPendingLen()	{ return m_inbufLen - m_readBegin; }	// 剩余未处理的字节数
	inline int getFreeLen()		{ return MaxBuffLen - m_inbufLen; }		// 可以append的长度
	inline void BindSession(CTcpSession* pSession) { m_pSession = pSession; }	// 绑定会话对象

	// virtual
	virtual bool Initialize();								// 初始化
	virtual bool Connect(const char* ipAddr,uint16 port);	// 主动连接远程服务器
	virtual void DisConnect();								// 断开连接
	virtual void Shutdown();								// 关闭连接
	
	// method
	bool ProcessPacket();									// 解包处理
	bool SendMsg(const char* pData,uint32 nSize);			// 发送消息
	void OnConnected();										// 建立连接后的回调
	void OnDisConnected();									// 断开连接后的回调

private:
	static void read_cb(struct bufferevent *bev, void *ctx);					// 读
	static void event_cb(struct bufferevent *bev, short events, void *ctx);		// buffervent 错误处理
	static THREAD_RETURN __thread_proc(void* pParam);							// event base 工作线程

public:
	CMessageQueue m_msgQueue;		// 接收到的消息队列

private:
	char m_szIpAddr[128];			// 远程server ip地址

	event_base* m_event_base;		// event_base 反应堆
	bufferevent* m_bufferev;		// buffer event
	CThread m_connectThread;		// connect 工作线程

	char m_inbuf[MaxBuffLen];		// buffer缓存，这里的缓存用来分包
	char m_outbuf[MaxBuffLen];		// buffer缓存，这里的缓存用组包
	int m_inbufLen;					// inbuff 结束位置
	int m_readBegin;				// 下一次开始读取的位置

	CTcpSession* m_pSession;		// 会话连接
};