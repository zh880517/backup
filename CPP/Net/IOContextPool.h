#pragma once

#include "SharePtrDef.h"
#include <vector>
class IOContextPool
{
public:
	IOContextPool(uint32_t iThreadNum);
	~IOContextPool();


	void Run();

	void Stop();

	IOContext* GetIOContext();

private:

	IOContext*				m_pContext;
	uint32_t				m_iThreadNum;
	std::vector<std::thread*> m_Threads;
};

