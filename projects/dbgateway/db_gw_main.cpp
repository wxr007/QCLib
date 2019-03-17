
#include <pthread.h>

#include "log_adaptor.h"

#include "db_cache.h"
#include "database_adaptor.h"
#include "db_gateway.h"



DBGateway *g_db_gw = NULL;

using namespace QCOOL;
using namespace std;
Logger g_logger("",LOGLEVEL_DEBUG,"");


bool init_db();

void* db_gateway_run(void *arg);

bool init_logger()
{
    g_logger.Start();
}


int main(int argc,char* argv[])
{
    init_logger();
    if (!init_db())
    {
        return 0;
    }
    db_gateway_run(NULL);
    return 0;
}

bool init_db()
{
    DBSqlConnectInfo mysql_cfg;
    mysql_cfg.host = "172.16.9.190";
    mysql_cfg.port = 3306;
    mysql_cfg.user = "root";
    mysql_cfg.passwd = "123456";
    mysql_cfg.dbname = "dtcq_syscfg_196";
    
    RedisConnectInfo redis_cfg(mysql_cfg.host,6379);
    string configname("table2redis.xml");
    try
    {
        g_db_gw = new DBGateway(configname,&mysql_cfg,&redis_cfg);
    }
    catch(...)
    {
        ERROR("DBGateway failed!");
        return false;
    }
    DEBUG("DBGateway Sucess!");
    return true;
}

pthread_cond_t g_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

void* db_gateway_run(void *arg)
{
    if (0 != g_db_gw->MysqlToRedis())
    {
        //ERROR("DBGateway failed!");
        return 0;
    }
    struct timespec abstime;
    abstime.tv_nsec = 0;
    
    const time_t delay_second = 5*60;//5mins;
    time_t start_time = time(0);
    int32_t last_time = 0;
    abstime.tv_sec = start_time+delay_second;
    pthread_cond_timedwait(&g_cond,&g_mutex,&abstime);
    start_time = abstime.tv_sec;
    while (true)
    {
        if (g_db_gw->RedisToMysql() != 0)
        {
            //do something;
        }
        last_time = time(0);
        start_time += delay_second;
        if (start_time > last_time)
        {
            abstime.tv_sec = start_time;
            pthread_cond_timedwait(&g_cond,&g_mutex,&abstime);
        }
        else
        {
            start_time = last_time;
        }
        INFO("db_gateway_run end [%lld]",start_time);
    }
}


