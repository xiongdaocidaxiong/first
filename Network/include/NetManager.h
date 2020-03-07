#ifndef __NETMANAGET__
#define  __NETMANAGET__
#pragma  once
#include "ComDefine.h"

class NetManager{
public:
	NetManager();
	~NetManager();
public:
	int Init(unsigned short port);
	static NetManager *GetSingle();
	void BaseWork();
	void Stop();
public:
	HANDLE m_CompletePort;
	std::list<std::thread> m_WorkList;
	int isrun;

};
extern NetManager *g_netManager;
#endif