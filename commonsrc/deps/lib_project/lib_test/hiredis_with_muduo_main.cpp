#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <string>

#include <hiredis.h>
#include <async.h>
#include "libmuduo_adapter.h"

void getCallback(redisAsyncContext *c, void *r, void *privdata) {
	redisReply *reply = (redisReply *)r;
	if (reply == NULL) return;

	switch (reply->type)
	{
	case REDIS_REPLY_STRING:
		{
			printf("argv[%s]:%s\n", (char*)privdata,reply->str);
		}
		break;
	case REDIS_REPLY_ARRAY:
		{
			for (int i = 0; i < reply->elements; i++) 
			{
				printf("%u) [%d] %s - %d\n", i, reply->element[i]->type,reply->element[i]->str,reply->element[i]->len);
			}
		}
		break;
	}

	printf("\n");
/*	freeReplyObject(reply);*/
	/* Disconnect after receiving the reply to GET */
//	redisAsyncDisconnect(c);
}

void connectCallback(const redisAsyncContext *c, int status) {
	if (status != REDIS_OK) {
		printf("Error: %s\n", c->errstr);
		return;
	}
	printf("Connected...\n");
}

void disconnectCallback(const redisAsyncContext *c, int status) {
	if (status != REDIS_OK) {
		printf("Error: %s\n", c->errstr);
		return;
	}
	printf("Disconnected...\n");
}

void test(redisAsyncContext *ac)
{
	std::string temp;
	temp.append("value1\0value1\0",15);

	printf("temp_size = %d,temp_len = %d",temp.size(),temp.length());

// 	redisAsyncCommand(ac,NULL,NULL,"HMSET %s %s %s","test_table","key1","value1");
// 	redisAsyncCommand(ac, getCallback, (char*)"test", "HMGET %s %s","test_table","key1");

/*
	char table_name[] = "test_table";
	redisAsyncCommand(ac,NULL,NULL,"HMSET %s %s %b",table_name,"key1","value1\0value1",15);
	redisAsyncCommand(ac,NULL,NULL,"HMSET %s %s %s",table_name,"key2","value2\0value2");

	redisAsyncCommand(ac, getCallback, (char*)"test", "HMGET %s %s","test_table","key1");
 	redisAsyncCommand(ac, getCallback, (char*)"test", "HGETALL %s",table_name);
*/

}

int main (int argc, char **argv) {
	signal(SIGPIPE, SIG_IGN);
	muduo::net::EventLoop loop;

	redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);
	if (c->err) {
		/* Let *c leak for now... */
		printf("Error: %s\n", c->errstr);
		return 1;
	}

	redisLibMuduoAttach(c,&loop);
	redisAsyncSetConnectCallback(c,connectCallback);
	redisAsyncSetDisconnectCallback(c,disconnectCallback);
// 	redisAsyncCommand(c, NULL, NULL, "SET key %b", argv[argc-1], strlen(argv[argc-1]));
// 	redisAsyncCommand(c, getCallback, (char*)"end-1", "GET key");
	test(c);
	loop.loop();
	return 0;
}