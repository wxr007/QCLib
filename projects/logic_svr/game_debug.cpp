#include <time.h>
#include <boost/bind.hpp>
#include "game_debug.h"
#include "thread.h"
#include "file_system.h"

extern std::stringstream g_stream;

namespace GameTest
{
JsonParser	json_parser;
JsonParser	move_count_json;

void RecordViewList( ObjectId obj_id,ObjectSet view_set )
{
	std::string temp_string;
	Json::Value	view_array;
	for (ObjectSet::iterator it = view_set.begin();it != view_set.end();it++)
	{
		ObjectId tempObjId = static_cast<ObjectId>(*it);
		g_stream << tempObjId;
		g_stream >> temp_string;
		g_stream.clear();
		g_stream.str("");
		view_array.append(Json::Value(temp_string));
		temp_string.clear();
	}

	g_stream << obj_id;
	g_stream >> temp_string;
	g_stream.clear();
	g_stream.str("");

	muduo::MutexLockGuard lock(json_parser.m_json_Lock);
	json_parser.m_json_root[temp_string] = view_array;
	json_parser.m_save_json = true;
}

void RecordLoginTime(ObjectId obj_id,int login_time)
{
	std::string temp_string;
	g_stream << obj_id;
	g_stream >> temp_string;
	g_stream.clear();
	g_stream.str("");

	muduo::MutexLockGuard lock(move_count_json.m_json_Lock);
	move_count_json.m_json_root[temp_string]["LoginTime"] = login_time;
	move_count_json.m_save_json = true;
}
void RecordAddObject(ObjectId obj_id,ObjectId target_id,int pre_count)
{
	std::string temp_string1;
	g_stream << obj_id;
	g_stream >> temp_string1;
	g_stream.clear();
	g_stream.str("");

	std::string temp_string2;
	g_stream << target_id;
	g_stream >> temp_string2;
	g_stream.clear();
	g_stream.str("");

	muduo::MutexLockGuard lock(move_count_json.m_json_Lock);
	move_count_json.m_json_root[temp_string1][temp_string2]["PreCount"] = pre_count;
	move_count_json.m_save_json = true;
}
void AddMoveCount(ObjectId obj_id,ObjectId target_id)
{
	std::string temp_string1;
	g_stream << obj_id;
	g_stream >> temp_string1;
	g_stream.clear();
	g_stream.str("");

	std::string temp_string2;
	g_stream << target_id;
	g_stream >> temp_string2;
	g_stream.clear();
	g_stream.str("");

	muduo::MutexLockGuard lock(move_count_json.m_json_Lock);
	if (move_count_json.m_json_root[temp_string1][temp_string2]["Count"].isNull())
	{
		move_count_json.m_json_root[temp_string1][temp_string2]["Count"] = 1;
	}
	else
	{
		int move_count = move_count_json.m_json_root[temp_string1][temp_string2]["Count"].asInt();
		move_count++;
		move_count_json.m_json_root[temp_string1][temp_string2]["Count"] = move_count;
	}
	move_count_json.m_save_json = true;
}

void RecordSelfMove( ObjectId obj_id,int move_count )
{
	std::string temp_string;
	g_stream << obj_id;
	g_stream >> temp_string;
	g_stream.clear();
	g_stream.str("");

	muduo::MutexLockGuard lock(move_count_json.m_json_Lock);
	move_count_json.m_json_root[temp_string]["myCount"] = move_count;
}


void SaveJsonFile()
{
	if (json_parser.m_save_json)
	{
		printf("1-write view json\n");
		json_parser.m_json_file.open("./json_debug/view.json",std::ofstream::out);
		if (json_parser.m_json_file.is_open())
		{
			printf("2-write view json");
			std::string temp_string;
			muduo::MutexLockGuard lock(json_parser.m_json_Lock);
			temp_string = json_parser.m_json_root.toStyledString();
			json_parser.m_json_file << temp_string;
			json_parser.m_json_file.close();
		}
		json_parser.m_save_json = false;
	}
	if (move_count_json.m_save_json)
	{
		printf("1-write move json\n");
		move_count_json.m_json_file.open("./json_debug/move.json",std::ofstream::out);
		if (move_count_json.m_json_file.is_open())
		{
			printf("2-write move json");
			std::string temp_string;
			muduo::MutexLockGuard lock(move_count_json.m_json_Lock);
			temp_string = move_count_json.m_json_root.toStyledString();
			move_count_json.m_json_file << temp_string;
			move_count_json.m_json_file.close();
		}
		move_count_json.m_save_json = false;
	}
	
}

void RunJsonSave(QCOOL::Thread* thisThread)
{
	std::string json_path("./json_debug");
	FileSystem::CheckDirAccess(json_path);
	while(!thisThread->IsTerminated())
	{
		SaveJsonFile();
		sleep(5);
	}
}

void StartDebugThread()
{
	QCOOL::Thread	json_save(boost::bind(GameTest::RunJsonSave,_1),"json_save");
	json_save.Start();
	json_save.Join();
}


}
