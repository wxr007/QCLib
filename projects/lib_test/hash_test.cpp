#include <stdio.h>
#include <map> 
#if __GNUC__>2
#include <ext/hash_map>
#include <ext/hash_set>
using namespace __gnu_cxx;
#else
#include <hash_map>
#include <hash_set>
using namespace stdext;
#endif

struct comp  
{  
	typedef std::pair<int, int> value_type;  
	bool operator () (const value_type& ls, const value_type& rs)  
	{  
		printf("<%d,%d>:<%d,%d> \n",ls.first,ls.second,rs.first,rs.second);

		if (rs.first == 0)
		{
			printf("1\n");
			return ls.second < rs.second;  
		}
		if (rs.second == 0)
		{
			printf("2\n");
			return ls.first < rs.first;
		}
		if (ls.first == rs.first)
		{
			printf("3\n");
			return false;
		}
		if (ls.second == rs.second)
		{
			printf("4\n");
			return false;
		}
		printf("5\n");
		return true;
	}  
};  

void TestMap()
{
	typedef std::map<std::pair<int, int>, int, comp>   MultiKeyMap;
	MultiKeyMap		res;
	printf("insert:\n");
	res.insert(std::make_pair(std::make_pair(1,2), 12));
	printf("insert:\n");
	res.insert(std::make_pair(std::make_pair(2,2), 34)); 
	printf("insert:\n");
	res.insert(std::make_pair(std::make_pair(5,6), 56)); 

	printf("loop:\n");
	for (MultiKeyMap::iterator it = res.begin(); it != res.end(); it++)
	{
		printf("%d %d %d \n", it->first.first, it->first.second, it->second);  
	}

	printf("find:\n");
	std::map<std::pair<int, int>, int, comp>::iterator it = res.find(std::make_pair(1,0));  
	if (it == res.end())  
		printf("NULL\n");  
	else  
		printf("%d %d %d \n", it->first.first, it->first.second, it->second);  
}

struct MyHashKey
{
	int m_key1;
	int m_key2;
	MyHashKey(int key1,int key2):m_key1(key1),m_key2(key2){};
};
struct hashfunc
{
	size_t operator()(const class MyHashKey& key) const
	{
		printf("hashfunc <%d,%d>\n",key.m_key1,key.m_key2);
		return key.m_key1 * 100 + key.m_key2;
	}
};
struct compfunc
{
	bool operator()(const class MyHashKey& key1, const class MyHashKey& key2) const
	{
		printf("compfunc\n");
		return key1.m_key1 == key2.m_key1;
	}
};

void TestHash()
{
	typedef  hash_map<MyHashKey,int,hashfunc,compfunc>		MultiKeyHash;

	MultiKeyHash mkh;

	printf("insert:\n");
	mkh.insert(std::make_pair(MyHashKey(1,2),12));
	mkh.insert(std::make_pair(MyHashKey(1,3),13));


	printf("loop:\n");
	for (MultiKeyHash::iterator it = mkh.begin(); it != mkh.end(); it++)
	{
		printf("%d \n", it->second);  
	}

// 	printf("find:\n");
// 	MultiKeyHash::iterator it = mkh.find(MyHashKey(1,0));  
// 	if (it == mkh.end())  
// 		printf("NULL\n");  
// 	else  
// 		printf("%d\n", it->second);  
}

int main()  
{
	//TestMap();
	TestHash();
	return 0;
}  


class MultiHash
{

}