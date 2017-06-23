#pragma once

#include "SharePtrDef.h"
#include <vector>
class IOContextPool
{
public:
	IOContextPool(size_t num);
	~IOContextPool();


	void Run();

	void Stop();

	IOContext* GetIOContext();

private:

	std::vector<IOContext*> m_Pool;
	std::vector<std::thread*> m_Threads;
	size_t m_iCurContextIndex = 0;
};

