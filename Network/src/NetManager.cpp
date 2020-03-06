#include <WinSock2.h>
#include "NetManager.h"
#include <thread>
#include <iostream>
#pragma comment(lib, "WS2_32")

NetManager *g_netManager = nullptr;

NetManager::NetManager() :isrun(false)
{

}

NetManager::~NetManager()
{
	isrun = false;
	for (auto it = m_WorkList.begin(); it != m_WorkList.end(); ++it){
		it->join();
	}
}

int NetManager::Init(unsigned short port)
{
	WSADATA data;
	WORD ver = MAKEWORD(2,2);

	//初始化版本库
	int ret = WSAStartup(ver, &data);

	if (ret != 0){
		int err = GetLastError();
		WSACleanup();
		return err;
	}
	//创建套接字
	/*
	_In_ int af,
	_In_ int type,
	_In_ int protocol,
	_In_opt_ LPWSAPROTOCOL_INFOW lpProtocolInfo,
	_In_ GROUP g,
	_In_ DWORD dwFlags
	*/
	// 这里需要特别注意，如果要使用重叠I/O的话，这里必须要使用WSASocket来初始化Socket  
	// 注意里面有个WSA_FLAG_OVERLAPPED参数 
	SOCKET listenSock = WSASocket(AF_INET, SOCK_STREAM, 0, nullptr,0,WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == listenSock){
		int err = GetLastError();
		WSACleanup();
		return err;
	}
	SOCKADDR_IN  bindinfo;
	bindinfo.sin_addr.s_addr = htonl(INADDR_ANY);
	bindinfo.sin_family = AF_INET;
	bindinfo.sin_port = port;

	if (bind(listenSock, (sockaddr *)&bindinfo, sizeof(sockaddr)) == SOCKET_ERROR){
		int err = GetLastError();
		WSACleanup();
		return err;
	}

	listen(listenSock, SOMAXCONN);

	m_CompletePort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	SYSTEM_INFO info;
	GetSystemInfo(&info);

	int pth = info.dwNumberOfProcessors * 2;
	isrun = true;
	for (int i = 0; i < pth;++i)
	{
		m_WorkList.push_back(std::thread(&NetManager::BaseWork, this));
	}
	return ret;
}

NetManager * NetManager::GetSingle()
{
	if (g_netManager == nullptr)
		g_netManager = new NetManager();
	return g_netManager;
}

void NetManager::BaseWork()
{
	DWORD size;
	ULONG_PTR key;
	DWORD dt = 1000;
	int ret = 0;
	LPOVERLAPPED data;
	while (isrun)
	{
		ret = GetQueuedCompletionStatus(m_CompletePort, &size, (PULONG_PTR)&key, (LPOVERLAPPED *)&data, 10);

		if (ret == 0)
		{
			//等待的操作过时
			int err = GetLastError();
			if (err != 258)
			{
				std::cout << "GetQueuedCompletionStatus Error = " << err << std::endl;

				if (data == NULL ) continue;
			}

		}
		else{

			if (size == 0)
			{
				//关闭连接
				continue;
			}

			//获取信息
		}

	}
}

void NetManager::Stop()
{

}

