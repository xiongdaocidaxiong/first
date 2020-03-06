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

//template <typename _var>
//class SafeQueue{
//public:
//	void emplace(_var value);
//	_var front();
//	bool empty();
//	void pop();
//public:
//	queue<_var> m_tasks;
//	mutex	m_taskmutex;
//};
//
//template <typename _var>
//void SafeQueue<_var>::pop()
//{
//	if (empty()) return;
//	lock_guard<mutex> mlock(m_taskmutex);
//	m_tasks.pop();
//}
//
//template <typename _var>
//bool SafeQueue<_var>::empty()
//{
//	lock_guard<mutex> mlock(m_taskmutex);
//	return m_tasks.empty();
//}
//
//template <typename _var>
//_var SafeQueue<_var>::front()
//{
//	if (empty()) return _var();
//	lock_guard<mutex> mlock(m_taskmutex);
//	return m_tasks.front();
//}
//
//template <typename _var>
//void SafeQueue<_var>::emplace(_var value)
//{
//	lock_guard<mutex> mlock(m_taskmutex);
//	m_tasks.emplace(value);
//}

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
	
	list<thread> mthreads;
	queue<function<void()> > m_tasks;
	mutex m_taskmutex;
	condition_variable m_var;
	int cnt;
	int canrun;
};

template<typename _Tst, typename ...Args >
void BastThreadPool::pushTask(_Tst &&fun, Args &&... args)
{
	auto task = std::bind(std::forward<_Tst>(fun), std::forward<Args>(args)...);
	{
		unique_lock<mutex> mlock(m_taskmutex);
		if (canrun == false) return;
		m_tasks.emplace(task);
	}
	m_var.notify_one();
}

extern BastThreadPool *g_baseThreadPool;
#endif