#ifndef __MTHREAD_H__
#define __MTHREAD_H__
#pragma  once

#include "ComDefine.h"

class BaseThread
{
public:
	BaseThread();
	virtual ~BaseThread();
	template<typename _Fun,typename ... Args>
	bool Init(_Fun ,Args ...);
	virtual bool Run();
	virtual bool Stop();
public:
};

template<typename _Fun, typename ... Args>
bool BaseThread::Init(_Fun fun, Args ...args)
{
	auto myfun = std::bind(std::forward<_Tst>(fun), std::forward<Args>(args)...);
	myfun();
	return Run();
}

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
	
	std::list<std::thread>				mthreads;
	std::queue<std::function<void()> >	m_tasks;
	std::mutex							m_taskmutex;
	std::condition_variable				m_var;
	int									cnt;
	int									canrun;
};

template<typename _Tst, typename ...Args >
void BastThreadPool::pushTask(_Tst &&fun, Args &&... args)
{
	auto task = std::bind(std::forward<_Tst>(fun), std::forward<Args>(args)...);
	{
		std::unique_lock<mutex> mlock(m_taskmutex);
		if (canrun == false) return;
		m_tasks.emplace(task);
	}
	m_var.notify_one();
}

extern BastThreadPool *g_baseThreadPool;
#endif