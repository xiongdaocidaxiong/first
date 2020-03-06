#include "MThread.h"

BaseThread::BaseThread() :flag(1)
{

}

BaseThread::~BaseThread()
{

}

bool BaseThread::Init()
{
	return Run();
}

bool BaseThread::Run()
{
	return Stop();
}

bool BaseThread::Stop()
{
	return true;
}

BastThreadPool *g_baseThreadPool = nullptr;

BastThreadPool::BastThreadPool(int nCount) :cnt(nCount), canrun(true), waitCnt(0)
{

}

BastThreadPool::~BastThreadPool()
{
	{
		unique_lock<mutex> mlock(mutex);
		canrun = false;
	}
	m_var.notify_all();
	for (auto &worker : mthreads)
		worker->join();
	for (auto it = mthreads.begin(); it != mthreads.end();){
		thread * t = *it;
		it = mthreads.erase(it);
	}
}

BastThreadPool * BastThreadPool::GetSingle()
{
	if (g_baseThreadPool == nullptr){
		g_baseThreadPool = new BastThreadPool(10);
	}
	return g_baseThreadPool;
}

void BastThreadPool::Init()
{
	for (int i = 0; i < cnt; ++i){
		mthreads.emplace_back(new thread([this](){
			while (true)
			{
				function<void()> task;
				{
					std::unique_lock<mutex> mlock(m_taskmutex);

					this->m_var.wait(mlock, [this]()->bool{
						return canrun == false || !m_tasks.empty();
					});
					if (canrun == false && m_tasks.empty()) return;
					task = (m_tasks.front());
					m_tasks.pop();
				}
				task();
				
			}
		}));
	}
}

void BastThreadPool::Run()
{

}
