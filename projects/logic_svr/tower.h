
#pragma once

#include <stdint.h>
#include <set>
#include "hash_define.h"

typedef uint64_t MapId;			//地图id
typedef int16_t Coordinate;			//坐标

struct Point
{
    Coordinate m_x;
    Coordinate m_y;
// 	Point():m_x(0),m_y(0){}
    Point(int32_t x=0, int32_t y=0) :m_x(static_cast<Coordinate>(x)), m_y(static_cast<Coordinate>(y)){}
// 	Point(Point& newPoint)
// 	{
// 		m_x = newPoint.m_x;
// 		m_y = newPoint.m_y;
// 	}// 	Point& operator=(Point& newPoint)	//型赋值操作
// 	{
// 		m_x = newPoint.m_x;
// 		m_y = newPoint.m_y;
// 		return *this;
// 	}
};

struct Area
{
    Point m_left_up;//左上;
    Point m_right_down;//右下;
    Area(){}
    Area(Point lu,Point rd):m_left_up(lu),m_right_down(rd){}
};

//地图格子.
typedef int64_t ObjectId;
typedef int32_t CellId;
typedef int32_t TowerId;
//typedef hash_set<ObjectId> ObjectSet;
typedef std::set<ObjectId> ObjectSet;

class Tower
{
private :
    friend class TowerAoi;
private :
    void Init(Point lu,TowerId id)
    {
        m_left_up = lu;
        m_id = id;
    }
    #if 0
    void AddWatcher(ObjectId id)
    {
        m_watchers.insert(id);
        m_objs.insert(id);
    }
    void AddObject(ObjectId id)
    {
        m_objs.insert(id);
    }
    void DelObject(ObjectId id)
    {
        m_objs.erase(id);
    }
    void AddWatcher(ObjectId id)
    {
        m_watchers.erase(id);
        m_objs.erase(id);
    }
    #endif
private :
    //stArea  m_area;//左上角顶点.
    Point m_left_up;
    TowerId m_id;
    ObjectSet m_objs;
    ObjectSet m_watchers;
};