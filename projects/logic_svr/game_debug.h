#pragma  once
#include <fstream>
#include <json/json.h>
#include <muduo/base/Mutex.h>
#include "define_logic_data.h"

namespace GameTest
{
class JsonParser
{
public:
	JsonParser()
		:m_save_json(false)
	{

	}

	muduo::MutexLock	m_json_Lock;
	Json::Value			m_json_root;
	std::ofstream		m_json_file;
	bool				m_save_json;
};

void RecordViewList(ObjectId obj_id,ObjectSet view_set);
void SaveJsonFile();
void RunJsonSave();
void StartDebugThread();

void RecordLoginTime(ObjectId obj_id,int login_time);
void RecordAddObject(ObjectId obj_id,ObjectId target_id,int pre_count);
void AddMoveCount(ObjectId obj_id,ObjectId target_id);
void RecordSelfMove( ObjectId obj_id,int move_count );
}


#define	  GAME_DEBUG

#ifdef		GAME_DEBUG
#define		START_GAME_DEBUG						GameTest::StartDebugThread();
#define		RECPRDVIEWLIST(obj_id,view_set)			GameTest::RecordViewList(obj_id,view_set);
#define		LOGININTOMAP(obj_id,login_time);		GameTest::RecordLoginTime(obj_id,login_time);
#define		ADDOBJECT(obj_id,target_id,pre_count)	GameTest::RecordAddObject(obj_id,target_id,pre_count);
#define		MOVECOUNT(obj_id,target_id)				GameTest::AddMoveCount(obj_id,target_id);
#define		MYMOVECOUNT(obj_id,move_count)			GameTest::RecordSelfMove(obj_id,move_count);
#else
#define		START_GAME_DEBUG
#define		RECPRDVIEWLIST(obj_id,view_set)
#define		ADDOBJECT(obj_id,target_id,pre_count)
#define		MOVECOUNT(obj_id,target_id)	
#define		MYMOVECOUNT(obj_id,move_count)	
#endif
