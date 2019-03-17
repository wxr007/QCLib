#ifndef __HIREDIS_LIBMUDUO_H__
#define __HIREDIS_LIBMUDUO_H__
#include <muduo/net/EventLoop.h>
#include <muduo/net/Channel.h>
#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>
#include <hiredis.h>
#include <async.h>

namespace QCOOL
{

typedef struct redisLibMuduoEvents 
{
	redisAsyncContext *context;
	boost::scoped_ptr<muduo::net::Channel> channel_;

} redisLibMuduoEvents;

static void redisLibMuduoReadEvent(void *arg) 
{
	redisLibMuduoEvents *e = static_cast<redisLibMuduoEvents*>(arg);
	redisAsyncHandleRead(e->context);
}

static void redisLibMuduoWriteEvent(void *arg)
{
	redisLibMuduoEvents *e = static_cast<redisLibMuduoEvents*>(arg);
	redisAsyncHandleWrite(e->context);
}

static void redisMuduoAddRead(void *privdata)
{
	redisLibMuduoEvents *e = static_cast<redisLibMuduoEvents*>(privdata);
	e->channel_->enableReading();
}
static void redisMuduoDelRead(void *privdata)
{
	redisLibMuduoEvents *e = static_cast<redisLibMuduoEvents*>(privdata);
	e->channel_->disableReading();
}
static void redisMuduoAddWrite(void *privdata)
{
	redisLibMuduoEvents *e = static_cast<redisLibMuduoEvents*>(privdata);
	e->channel_->enableWriting();
}
static void redisMuduoDelWrite(void *privdata)
{
	redisLibMuduoEvents *e = static_cast<redisLibMuduoEvents*>(privdata);
	e->channel_->disableWriting();
}
static void redisMuduoCleanup(void *privdata)
{
	redisLibMuduoEvents *e = static_cast<redisLibMuduoEvents*>(privdata);
	e->channel_->disableAll();
}

static int redisLibMuduoAttach(redisAsyncContext *ac, muduo::net::EventLoop* loop) 
{
	redisContext *c = &(ac->c);

	if (ac->ev.data != NULL)
		return REDIS_ERR;

	redisLibMuduoEvents *e = new redisLibMuduoEvents();
	e->context = ac;

	e->channel_.reset(new muduo::net::Channel(loop, c->fd));

	ac->ev.addRead = redisMuduoAddRead;
	ac->ev.delRead = redisMuduoDelRead;
	ac->ev.addWrite = redisMuduoAddWrite;
	ac->ev.delWrite = redisMuduoDelWrite;
	ac->ev.cleanup = redisMuduoCleanup;
	ac->ev.data = e;

	e->channel_->setReadCallback(boost::bind(&redisLibMuduoReadEvent,e));
	e->channel_->setWriteCallback(boost::bind(&redisLibMuduoWriteEvent,e));
}

}
#endif