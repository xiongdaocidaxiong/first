
#include "BaseMysql.h"
#include "MThread.h"

int main()
{
	BaseMysqlManager::GetSingle()->Init();
	BastThreadPool::GetSingle()->Init();

	int t = 10000;
	while (true)
	{
		/*if (t > 0){
			t--;
			BastThreadPool::GetSingle()->pushTask(fun, t);
		}*/
		Sleep(1);
	}
	return 0;
}