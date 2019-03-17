#include <stdlib.h>
#include <stdint.h>
#include <time.h>


int64_t GenerateKey(int32_t nKeyIndex)
{
	int32_t nTime = time(NULL);
	int64_t n64Key = (static_cast<int64_t>(nTime) << 32) | static_cast<int64_t>(nKeyIndex);
	return n64Key;
};
