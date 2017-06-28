#include "IOContextPool.h"
#include "NetInclude.h"
#include <iostream>

IOContextPool::IOContextPool(uint32_t iThreadNum)
	:m_iThreadNum(iThreadNum)
{
	m_pContext = new IOContext();
	if (m_iThreadNum == 0)
	{
		m_iThreadNum = 1;
	}
}


IOContextPool::~IOContextPool()
{
	if (m_Threads.size() > 0)
	{
		Stop();
	}
	delete m_pContext;
}

void IOContextPool::Run()
{
	for (uint32_t i=0; i< m_iThreadNum; ++i)
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
		}, m_pContext);
		m_Threads.push_back(pThread);
	}
}

void IOContextPool::Stop()
{
	m_pContext->Context.stop();
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
	return m_pContext;
}
