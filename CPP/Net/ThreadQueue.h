#pragma once

#include <queue>
#include <mutex>

template<T>
class ThreadQueue
{
public:
	void Push(const T& value)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		m_Queue.push(value);
	}

	template<class Iter>
	void Push(Iter begin, Iter end)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		for (; begin != end; ++begin)
			m_Queue.push(*begin);
	}

	bool Peek(T& value)
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		if (m_Queue.empty())
			return false;
		value = m_Queue.front();
	}

	void Pop()
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		if(!m_Queue.empty())
			m_Queue.pop();
	}



private:
	std::queue<T> m_Queue;
	std::mutex m_Mutex;
};