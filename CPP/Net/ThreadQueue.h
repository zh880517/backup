#pragma once

#include <queue>
#include <list>
#include <mutex>

template<class T>
class ThreadQueue
{
public:
	void Push(const T& value)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_Queue.push(value);
	}

	void Push(const std::list<T>& packets)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		for (auto& packet : packets)
		{
			m_Queue.push(packet);
		}
	}

	bool Peek(T& value)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		if (m_Queue.empty())
			return false;
		value = m_Queue.front();
		return true;
	}

	void Pop()
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		if(!m_Queue.empty())
			m_Queue.pop();
	}

	void Swap(std::queue<T>& out)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_Queue.swap(out);
	}

	size_t Count()
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		return m_Queue.size();
	}

private:
	std::queue<T> m_Queue;
	std::mutex m_Mutex;
};

