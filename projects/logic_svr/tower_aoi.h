
#pragma once

#include <vector>

#include "tower.h"
//#include "define_logic_data.h"

//说明:灯塔算法，主要有两种列表：观察者和对象；
//     对象，有些是不需要观察其他人，有些对象需要观察其他人是否进入视野范围；需要观察的对象，就是观察者。
//     一个对象，坐标发生变化，周围的需要通知这些观察者，对象列表发生变化了；
//     一个观察者，坐标发生变化，除了要通知观察者；另外，自己视野范围的对象列表也发生了变化，所以需要通知自己，对应的对象列表变化.

class TowerAoi
{
public :
    //w ,h :地图宽,高;
    //tw,th:塔  宽,高;
    //vw,vh:视野宽,高.
    TowerAoi(int32_t w,int32_t h,int32_t tw,int32_t th,int32_t vw,int32_t vh,MapId id);

    //wather operate;
    //objid:watcher的objectid;
    //p    :watcher的坐标;
    //watcher:要通知的watcher;
    //objs   :自己视野内的objects(通知自己);
    int32_t AddWatcher(ObjectId objid,Point &p,ObjectSet &watcher,ObjectSet &objs);

    //objid:watcher的objectid;
    //op/np  :watcher的old/new坐标;
    //winsert/wsame/wremove:要通知的watcher(增/不变/删);
    //addobjs/sameobjs/rmobjs:自己视野内的objects(通知自己)(增/不变/删objects);
    int32_t MoveWatcher(ObjectId objid,Point & op,Point & np,ObjectSet & winsert,ObjectSet & wsame,ObjectSet & wremove,ObjectSet&addobjs,ObjectSet&sameobjs,ObjectSet&rmobjs);

    //objid:watcher的objectid;
    //p    :watcher的坐标;
    //watcher:要通知的watcher;
    //objs   :自己视野内的objects(通知自己);
    int32_t DelWatcher(ObjectId objid,Point &p,ObjectSet &watcher,ObjectSet &objs);

    //object operate;
    //objid:watcher的objectid;
    //p    :watcher的坐标;
    //watcher:要通知的watcher;
    int32_t AddObject(ObjectId objid,Point &,ObjectSet &watcher);

    //objid:watcher的objectid;
    //op/np  :watcher的old/new坐标;
    //winsert/wsame/wremove:要通知的watcher(增/不变/删);
    int32_t MoveObject(ObjectId objid,Point &op,Point &np,ObjectSet &winsert,ObjectSet &wsame,ObjectSet &wremove);

    //objid:watcher的objectid;
    //p    :watcher的坐标;
    //watcher:要通知的watcher;
    int32_t DelObject(ObjectId objid, Point &p, ObjectSet &watcher);

    void TowerShow();
private :
    void InitTowers();
    int32_t TowerId(int32_t x,int32_t y);
    int32_t CellId(int32_t x,int32_t y);
    //tower坐标;格子坐标;两种坐标之间的相互转化.
    Point CellToTower(Point p);
    Point TowerToCell(Point p);
    //视野范围内的塔坐标.
    void ViewTowers(Point p,Area &area);
    //void cellArea2TowerArea(Area &area,Area &tarea);
    //void view_cell_area(Point p,Area &area);

    void AddOrDeleteObject(ObjectId objid,Point &p,ObjectSet &watcher,int32_t optType);
    void AddOrDeleteWatcher(ObjectId objid,Point &p,ObjectSet &watcher,ObjectSet &objs,int32_t optType);
    //求add/交/差集合.
    void MoveOject(const ObjectSet &oldt,const ObjectSet &newt,ObjectSet & insert,ObjectSet & same,ObjectSet & remove);

    
private :
    //map info;
    int32_t m_weight;
    int32_t m_hight;
    //人物视野;
    int32_t m_view_weight;
    int32_t m_view_hight;
    //塔视野;
    int32_t m_tower_view_weight;
    int32_t m_tower_view_hight;
    //tower info;
    int32_t m_towerXN;//地图有几个塔宽;
    int32_t m_towerYN;//地图有几个塔高;
    const int32_t m_tower_weight;//塔宽;
    const int32_t m_tower_hight;//塔高;

    MapId m_mapid;
    std::vector<Tower> m_towers;
};

