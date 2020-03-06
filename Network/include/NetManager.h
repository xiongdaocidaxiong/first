#ifndef __NETMANAGET__
#define  __NETMANAGET__

#include <windows.h>
#include "MThread.h"

class NetManager{

public:
	int Init(unsigned short port);

	void Stop();
public:
	HANDLE m_CompletePort;

};

class WorkThread:public BaseThread{
public:

};
#endif