#ifndef __MTHREAD_H__
#define __MTHREAD_H__

#include <functional>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <list>
#include <future>
#include <stdexcept>
using namespace std;
class BaseThread
{
public:
	BaseThread();
	virtual ~BaseThread();
	virtual bool Init();
	virtual bool Run();
	virtual bool Stop();
public:
	int flag;
};

class BastThreadPool
{
public:
	BastThreadPool(int nCount);
	virtual ~BastThreadPool();
	static BastThreadPool *GetSingle();
	void Init();
	void Run();
	template<typename _Tst,typename ...Args>
	void pushTask(_Tst &&fun, Args &&... args);

public:
	
	list<thread*> mthreads;
	queue<function<void()> > m_tasks;
	mutex m_taskmutex;
	condition_variable m_var;
	int cnt;
	int canrun;
	int waitCnt;
};

template<typename _Tst, typename ...Args >
void BastThreadPool::pushTask(_Tst &&fun, Args &&... args)
{
	auto task = std::bind(std::forward<_Tst>(fun), std::forward<Args>(args)...);
	{
		unique_lock<mutex> mlock(mutex);
		if (canrun == false) return;
		m_tasks.emplace(task);
	}
	m_var.notify_one();
}

extern BastThreadPool *g_baseThreadPool;
#endif