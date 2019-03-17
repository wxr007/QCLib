#pragma  once
#include <boost/noncopyable.hpp>
#include <stdint.h>


namespace QCOOL
{

//原子操作类型，只对整数类型进行定义
template<typename T>
class AtomicInt		//原子操作的int类型
{
private:
	volatile T m_value;
public:
	AtomicInt()
		:m_value(0)
	{
	}

	__inline T Get()		//获取
	{
		return __sync_val_compare_and_swap(&m_value,0,0);//如果m_value == 0  m_value = 0 返回 m_value
	}
	__inline T GetAndAdd(const T& x)		//获取然后加
	{
		return __sync_fetch_and_add(&m_value, x);
	}
	__inline T AddAndGet(const T& x)		//加然后获取
	{
		return __sync_add_and_fetch(&m_value, x);
	}
	__inline T GetAndSub(const T& x)		//获取然后减
	{
		return __sync_fetch_and_sub(&m_value, x);
	}
	__inline T SubAndGet(const T& x)		//减然后获取
	{
		return __sync_sub_and_fetch(&m_value, x);
	}
	__inline T GetAndSet(const T& new_value)//赋值
	{
		return __sync_lock_test_and_set(&m_value, new_value);
	}
	__inline T Increment()					//原子自增相当于++i
	{
		return AddAndGet(1);
	}
	__inline T Decrement()					//原子自增相当于--i
	{
		return SubAndGet(1);
	}

	//===赋值====
	AtomicInt<T>& operator=(const AtomicInt<T>& new_value)	//本类型赋值操作
	{
		__sync_lock_test_and_set(&m_value,new_value.m_value);
		return *this;
	}
	AtomicInt<T>& operator =(const T& new_value)		//T类型赋值操作
	{
		GetAndSet(new_value);
		return *this;
	}
	//===比较===
	bool operator==(const T& x) 
	{
		return __sync_bool_compare_and_swap(&m_value,x,x);
	}
	bool operator!=(const T& x) 
	{
		return !__sync_bool_compare_and_swap(&m_value,x,x);
	}
	//////////////////////////////////////////////////////////////////////////
	//以下自增，自减的实用性有待考量
	//////////////////////////////////////////////////////////////////////////
	//===加====
	AtomicInt<T>& operator +=(const T& x)
	{
		AddAndGet(x);
		return *this;
	}
	AtomicInt<T>& operator ++()//前缀形式	++i
	{
		AddAndGet(1);
		return *this;
	}
	AtomicInt<T>& operator ++(int x)//后缀形式 i++	（int i只是用于分辨后缀形式）
	{
		GetAndAdd(1);
		return *this;
	}
	//===减====
	AtomicInt<T>& operator -=(const T& x)
	{
		SubAndGet(x);
		return *this;
	}
	AtomicInt<T>& operator --()//前缀形式	--i
	{
		SubAndGet(1);
		return *this;
	}
	AtomicInt<T>& operator --(int x)//后缀形式 i--	（int i只是用于分辨后缀形式）
	{
		GetAndSub(1);
		return *this;
	}
};

typedef	AtomicInt<int32_t> AtomicInt32;
typedef	AtomicInt<int64_t> AtomicInt64;

}
