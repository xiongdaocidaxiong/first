#include <WinSock2.h>
#include "NetManager.h"


int NetManager::Init(unsigned short port)
{
	WSADATA data;
	WORD ver = MAKEWORD(2,2);

	//��ʼ���汾��
	int ret = WSAStartup(ver, &data);

	if (ret != 0){
		int err = GetLastError();
		WSACleanup();
		return err;
	}
	//�����׽���
	/*
	_In_ int af,
	_In_ int type,
	_In_ int protocol,
	_In_opt_ LPWSAPROTOCOL_INFOW lpProtocolInfo,
	_In_ GROUP g,
	_In_ DWORD dwFlags
	*/
	// ������Ҫ�ر�ע�⣬���Ҫʹ���ص�I/O�Ļ����������Ҫʹ��WSASocket����ʼ��Socket  
	// ע�������и�WSA_FLAG_OVERLAPPED���� 
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

	for (int i = 0; i < pth;++i)
	{

	}
	return ret;
}

void NetManager::Stop()
{

}

