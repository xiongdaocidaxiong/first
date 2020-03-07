#include "MThread.h"


BaseThread::BaseThread()
{

}

BaseThread::~BaseThread()
{

}


bool BaseThread::Run()
{
	while (g_SerStart);

	return Stop();
}

bool BaseThread::Stop()
{
	return true;
}

BastThreadPool *g_baseThreadPool = nullptr;

BastThreadPool::BastThreadPool(int nCount) :cnt(nCount), canrun(true)
{

}

BastThreadPool::~BastThreadPool()
{
	{
		std::unique_lock<std::mutex> mlock(m_taskmutex);
		canrun = false;
	}
	m_var.notify_all();
	for (auto &worker : mthreads)
		worker.join();
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
		mthreads.emplace_back(([this](){
			while (canrun)
			{
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> mlock(m_taskmutex);

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
