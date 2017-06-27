#include "IOContextPool.h"
#include "NetInclude.h"
#include <iostream>

IOContextPool::IOContextPool(size_t num)
{
	for (size_t i=0; i<num; ++i)
	{
		IOContext* pContext = new IOContext();
		m_Pool.push_back(pContext);
	}
}


IOContextPool::~IOContextPool()
{
	if (m_Threads.size() > 0)
	{
		Stop();
	}
	for (auto p : m_Pool)
	{
		delete p;
	}
}

void IOContextPool::Run()
{
	for (size_t i=0; i< m_Pool.size(); ++i)
	{
		std::thread* pThread = new std::thread([](IOContext* pContext) 
		{
			while (true)
			{
				try
				{
					pContext->Context.run();
					break;
				}
				catch (std::exception& e)
				{
					std::cout << e.what() << std::endl;
				}
				catch (...)
				{

				}
			}
		}, m_Pool[i]);
		m_Threads.push_back(pThread);
	}
}

void IOContextPool::Stop()
{
	for (size_t i=0; i< m_Pool.size(); ++i)
	{
		m_Pool[i]->Context.stop();
	}
	for (size_t i=0; i< m_Threads.size(); ++i)
	{
		auto pThread = m_Threads[i];
		if (pThread->joinable())
		{
			pThread->join();
		}
		delete pThread;
	}
	m_Threads.clear();
}

IOContext * IOContextPool::GetIOContext()
{
	if (m_iCurContextIndex >= m_Pool.size())
		return nullptr;
	IOContext* pCon = m_Pool[m_iCurContextIndex];
	if (++m_iCurContextIndex >= m_Pool.size())
	{
		m_iCurContextIndex = 0;
	}
	return pCon;
}
