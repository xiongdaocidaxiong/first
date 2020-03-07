#ifndef __MAIN__CPP__
#define __MAIN__CPP__

#include "BaseMysql.h"
#include "MThread.h"
#include "NetManager.h"
#include "ComDefine.h"

inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


//“Ï≤Ω
template<typename Fun,typename ... Args>
void asynpro(Fun fun,Args ...args){
	auto mfun = std::bind(std::forward<Fun>(fun), std::forward<Args>(args)...);
	mfun();
	//BastThreadPool::GetSingle()->Init();
	//BastThreadPool::GetSingle()->pushTask(fun, t);
}

void asynpro2(){
	asynpro(&BastThreadPool::Init, BastThreadPool::GetSingle());
	while (g_SerStart){}
}

void ShutDown(){
	SAFEDELETE(g_netManager);
	SAFEDELETE(g_baseThreadPool);
	SAFEDELETE(m_baseMysqlManaget);
}

int main()
{
	EnableMemLeakCheck();

	std::thread t(asynpro2);
	t.detach();

	//BaseMysqlManager::GetSingle()->Init();
	NetManager::GetSingle()->Init(10086);

	ShutDown();
	return 0;
}

#endif