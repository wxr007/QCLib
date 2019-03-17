#pragma once
//sys
//#include <sys/klog.h>
#include <stdio.h>
//third Lib
#include <muduo/net/Buffer.h>
//self
#include "base_define.h"

namespace QCOOL
{
using muduo::net::Buffer;

const char FILLBUFF[8] = {0};

inline void msg2Buffer(const Message* msg,Buffer* buf)
{
	int32_t size = PACKET_SIZE(msg->m_header.m_data_len);
	//void *data = dynamic_cast<void*>(msg);
	buf->append(reinterpret_cast<const void*>(msg),size);
}

inline bool enouthData(Buffer *buf)
{
    bool ret = false;
    size_t bufsize = buf->readableBytes();
    if (bufsize > 4)
    {
        int32_t datasize = 0;
        memcpy(&datasize,buf->peek(),sizeof(int32_t));
        if (bufsize >= PACKET_SIZE(datasize))
        {
            ret = true;
        }
    }
    return ret;
}

inline Message* newMsgFormBuffer(Buffer *buf)
{
	//1-get size;
	int32_t datasize = 0;
	memcpy(&datasize,buf->peek(),sizeof(int32_t));
	//3-fill data;
	Message*ptmp = NewMessage(datasize);
	if (ptmp == NULL)
	{
		//char buf[256] = {0};
		//int32_t sz = snprintf(buf,256,"Server2.0-%s-%s:new size %d failed!",__FILE__,__FUNCTION__,PACKET_SIZE(datasize));
		//syslog(LOG_ALERT,buf,sz);
		abort();
		return NULL;
	}
	int32_t packetsize = PACKET_SIZE(datasize);
	memcpy(reinterpret_cast<void*>(ptmp),buf->peek(),packetsize);
	buf->retrieve(packetsize);

	return ptmp;
}

}//end namespace QCOOL;
