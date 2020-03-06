
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

void fun(int a){
	cout << a << endl;
}
void fun2(){
	BastThreadPool::GetSingle()->Init();
	int t = 100;
	while (t--)
	{
		BastThreadPool::GetSingle()->pushTask(fun, t);
	}
}

void ShutDown(){
	SAFEDELETE(g_netManager);
	SAFEDELETE(g_baseThreadPool);
	SAFEDELETE(m_baseMysqlManaget);
}
int main()
{
	EnableMemLeakCheck();
	BaseMysqlManager::GetSingle()->Init();
	NetManager::GetSingle()->Init(10086);
	while (true)
	{

	}
	ShutDown();
	return 0;
}