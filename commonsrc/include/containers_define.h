#pragma once
#include <stdio.h>
#include <deque>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include "thread.h"
#include "log_adaptor.h"

namespace QCOOL
{

template<typename T>
class BlockQueue : public Condition
{
public:
	typedef std::deque<T>	Queue;
	BlockQueue()
	{
	}
	~BlockQueue()
	{

	}
	void PutData(const T& x)
	{
		Lock();
		m_queue.push_back(x);
		ERROR("set m_queue %u",m_queue.size());
		notify();
		Unlock();
	}
	void GetOne(T& x)
	{
		Lock();
		while (m_queue.empty())
		{
			Wait();
		}
		x = m_queue.front();
		m_queue.pop_front();
		Unlock();
	}
	void GetData(Queue& queue)
	{
		Lock();
		while (m_queue.empty())
		{
			Wait();
		}
		ERROR("get1 m_queue %u",m_queue.size());
		m_queue.swap(queue);
		ERROR("get2 m_queue %u",m_queue.size());
		Unlock();
	}
 private:
	std::deque<T>     m_queue;
};

class FunctionBase
{
public:
	FunctionBase(){};
	~FunctionBase(){};
	virtual void Call() = 0;
};

template <typename Handler>
class FunctionHandler : public FunctionBase
{
public:
	FunctionHandler(Handler& handler) : m_handler(handler) {}
	~FunctionHandler() {} 
	virtual void Call()
	{
		m_handler();
	}
	Handler m_handler; 
};

class FunctionQueue
{
public:
	FunctionQueue()
	{
	}
	~FunctionQueue()
	{
		for (std::deque<FunctionBase*>::iterator i = handler_queue.begin();
			i != handler_queue.end(); i++)
		{
			delete *i;
		}
	}

	template <typename Handler>
	void AddFunc(Handler handler)
	{
		FunctionHandler<Handler>* h = new FunctionHandler<Handler>(handler);
		handler_queue.push_back(dynamic_cast<FunctionBase*>(h));
	}
	void CallAllFunc()
	{
		std::deque<FunctionBase*>::iterator it = handler_queue.begin();
		for (;it != handler_queue.end();it++)
		{
			(*it)->Call();
			delete *it;
		}
		handler_queue.clear();
	}
	bool empty()
	{
		return handler_queue.empty();
	}
	void swap(FunctionQueue& queue)
	{
		handler_queue.swap(queue.handler_queue);
	}
private:
	std::deque<FunctionBase*>	handler_queue;
};

template<typename T>
class FuncAndDateBlockQueue : public Condition
{
public:
	typedef std::deque<T>	Queue;
	FuncAndDateBlockQueue()
	{

	}
	~FuncAndDateBlockQueue()
	{

	}
	void PutData(const T& x)
	{
		Lock();
		m_datas.push_back(x);
		notify();
		Unlock();
	}
	template <typename Handler>
	void PutFunc(Handler handler)
	{
		Lock();
		m_functors.AddFunc(handler);
		notify();
		Unlock();
	}
	void GetData(Queue& datas)
	{
		Lock();
		while (m_datas.empty() && m_functors.empty())
		{
			Wait();
		}
		datas.swap(m_datas);
		FunctionQueue functors;
		functors.swap(m_functors);
		Unlock();

		functors.CallAllFunc();
	}
private:
	FunctionQueue			m_functors;
	std::deque<T>			m_datas;
};



#define _Default_Len_			1024
template <class _Type>
class LoopArray					//环形列表
{
public:
	typedef _Type*	iterator;
	typedef _Type*	const_iterator;
	typedef LoopArray<_Type> _Myt;
protected:
	size_t m_head;				//头部
	size_t m_tail;				//尾部
	size_t m_size;				//当前大小
	size_t m_capacity;			//最大容量
	_Type *pBuf;
public:
	LoopArray()
		:m_head(0), m_tail(0), m_size(0)
	{
		pBuf = new _Type[_Default_Len_];//默认1024
		m_capacity = _Default_Len_;
	}
	LoopArray(size_t bufsize)
		: m_head(0), m_tail(0), m_size(0)
	{
		if( bufsize < 1)
		{
			pBuf = new _Type[_Default_Len_];
			m_capacity = _Default_Len_;
		}
		else
		{
			pBuf = new _Type[bufsize];
			m_capacity = bufsize;
		}
	}
	virtual ~LoopArray()
	{
		delete[] pBuf;
		pBuf = NULL;
		m_head = m_tail = m_size = m_capacity = 0;
	}
	_Type pop_front()//弹出一个元素
	{
		if( IsEmpty() )
		{
			return NULL;
		}

		size_t old_head = m_head;
		m_head = (m_head + 1) % m_capacity;
		--m_size;
		return pBuf[old_head];
	}

	bool push_back( _Type item)//压入一个元素
	{
		if ( IsFull() )
		{
			return false;
		}
		pBuf[m_tail] = item;
		m_tail = (m_tail + 1) % m_capacity;
		++m_size;
		return true;
	}

	size_t capacity() //返回容量
	{
		return m_capacity;
	}
	size_t size() //返回当前个数
	{
		return m_size;
	}

	bool IsFull() //是否满
	{
		return (m_size >= m_capacity);
	}
	bool IsEmpty()//是否空
	{
		return (m_size == 0);
	}
	void clear()
	{
		m_head = m_tail = m_size = 0;
	}
	bool Append(_Myt& loop_array)
	{
		if ( IsFull() )
		{
			return false;
		}
		if (loop_array.IsEmpty())
		{
			return false;
		}
		do
		{
			push_back(loop_array.pop_front());
		}
		while (!IsFull() && (!loop_array.IsEmpty()));
		return true;
	}
};

}