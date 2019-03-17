#pragma once

#include <stdint.h>

#include "define_logic_data.h"
#include "tower_aoi.h"

struct MapHeader
{
	MapOrigID 	m_map_origin_id;
	int32_t		m_map_width;
	int32_t		m_map_height;
	char		m_map_name[_MAX_NAME_LEN_];
	MapHeader()
	{
		ZEROSELF;
	}

};

struct MapGrid				//地图格子
{
	union
	{
		uint16_t chFlag;
		struct
		{
			uint8_t bitCanWalk: 1;			//是否能行走0可行
			uint8_t bitCanFireFly: 1;		//是否能飞
			uint8_t bitSafePk: 1;			//是否切磋区域
			uint8_t bitSafeZone: 1;			//是否是挖宝区
			uint8_t bit4: 1;
			uint8_t bit5: 1;
			uint8_t bit6: 1;
			uint8_t bit7: 1;
			uint8_t cellgroup;				//用于刷怪分组
		};
	};

	MapGrid()
	{
		chFlag = 0;
	}
};

//地图传送点结构
struct MapGate
{

	MapOrigID 	m_dst_map_origin_id;
	Coordinate 	m_x;
	Coordinate	m_y;
	MapGate()
	{
		ZEROSELF;
	}
};

struct MapGridInfo
{ 
	MapGrid		m_flag;
	MapGate*	m_map_gate;
};

typedef std::map<int,TowerAoi*>		CopyGameMap;

class GameMap
{
public:
	GameMap()
	:m_map_grid_info(NULL)
	{}
	~GameMap()
	{ 
		if (m_map_grid_info)
		{
			delete []m_map_grid_info;
			m_map_grid_info =NULL;
		}
	}
	void TestTnit()
	{
		m_map_id = 1;
		m_map_info.m_map_origin_id = 1;
		m_map_info.m_map_width = 100;
		m_map_info.m_map_height = 100;
		strncpy(m_map_info.m_map_name,"testmap",_MAX_NAME_LEN_-1);
		InitMapGrids();
		m_toweraoi =new TowerAoi(m_map_info.m_map_width,m_map_info.m_map_height,5,5,20,20,m_map_id);
	}
	bool LoadMapData(const char* szMapFile = "")
	{
		if (InitMapGrids())
		{
			return true;
		}
		return false;
	}
	bool InitMapGrids()
	{
		if((m_map_info.m_map_width > 1) && (m_map_info.m_map_height > 1))
		{
				int ncellsize = m_map_info.m_map_width * m_map_info.m_map_height;
				m_map_grid_info = new MapGridInfo[ncellsize];
				return true;
		}
		return false;
	}
	__inline const char* GetMapName()
	{
		return m_map_info.m_map_name;
	}
	__inline MapOrigID GetMapOrigId()
	{
		return m_map_info.m_map_origin_id;
	}
	__inline MapId GetMapId()
	{
		return m_map_id;
	}
	bool  GetGridInfo(Point point,MapGridInfo& Grid)
	{
		if (point.m_x < m_map_info.m_map_width && point.m_y < m_map_info.m_map_height)
		{
			Grid = m_map_grid_info[point.m_y*m_map_info.m_map_width+point.m_x];
			return true;
		}
		return false;
	}
	TowerAoi* GetTowerAoi()
	{
		return m_toweraoi;
	}
private:
	MapHeader		m_map_info;
	MapGridInfo*	m_map_grid_info;
	MapId			m_map_id;
	TowerAoi*		m_toweraoi;
	CopyGameMap		m_copy_map_list;
};