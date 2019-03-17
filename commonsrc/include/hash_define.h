#pragma once

#if __GNUC__>2
#include <ext/hash_map>
#include <ext/hash_set>
using namespace __gnu_cxx;
#else
#include <hash_map>
#include <hash_set>
using namespace stdext;
#endif
#include <string>

namespace __gnu_cxx
{
	template<> 
	struct hash<std::string>  
	{  
		size_t operator()(const std::string& str) const  
		{
			return __stl_hash_string(str.c_str());  
		}  
	};
}

