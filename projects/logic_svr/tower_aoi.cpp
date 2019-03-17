
#include <string.h>
#include "tower_aoi.h"

//check for map;
#define CHECKMIN(v)   if (v < 0){v=0;}
#define CHECKMAX(v,m) if (v > m){v=m;}

const int32_t TOWER_OPERATOR_NONE = 0;
const int32_t TOWER_OPERATOR_ADD = 1;
const int32_t TOWER_OPERATOR_DEL = 2;
const int32_t TOWER_OPERATOR_UPDATE = 3;

int32_t TowerAoi::TowerId(int32_t x,int32_t y)
{
    return y*m_towerYN+x;
}

int32_t TowerAoi::CellId(int32_t x,int32_t y)
{
    return y*m_weight+x;
}


TowerAoi::TowerAoi(int32_t w,int32_t h,int32_t tw,int32_t th,int32_t vw,int32_t vh,MapId id)
    :m_weight(w),m_hight(h)
    ,m_tower_weight(tw),m_tower_hight(th)
    ,m_view_weight(vw),m_view_hight(vh)
    ,m_mapid(id)
{
    InitTowers();
}

void TowerAoi::InitTowers()
{
    //alloc towers;
    m_towerXN = (m_weight+m_tower_weight-1)/m_tower_weight;
    m_towerYN = (m_hight+m_tower_hight-1)/m_tower_hight;
    
    m_tower_view_weight = (m_view_weight+m_tower_weight-1)/m_tower_weight;
    m_tower_view_hight = (m_view_hight+m_tower_hight-1)/m_tower_hight;
    
    m_towers.resize(m_towerXN*m_towerYN);
    Point p;
    int32_t towerid = 0;
    //init tower;
    for (int32_t y=0; y!=m_towerYN;++y)
    {
        for (int32_t x=0;x!=m_towerXN;++x)
        {
            p.m_y = y*m_tower_hight;
            p.m_x = x*m_tower_weight;
            towerid = TowerId(x,y);
            m_towers[towerid].Init(p,towerid);
        }
    }
}

Point TowerAoi::CellToTower(Point p)
{
    p.m_x /= m_tower_weight;
    p.m_y /= m_tower_hight;
    return p;
}

Point TowerAoi::TowerToCell(Point p)
{
    p.m_x *= m_tower_weight;
    p.m_y *= m_tower_hight;
    return p;
}

void TowerAoi::ViewTowers(Point p,Area &area)
{
    //get cell area;
    area.m_left_up.m_x = p.m_x - m_tower_view_weight;
    CHECKMIN(area.m_left_up.m_x);
    area.m_left_up.m_y = p.m_y - m_tower_view_hight;
    CHECKMIN(area.m_left_up.m_y);

    area.m_right_down.m_x = p.m_x + m_tower_view_weight+1;
    CHECKMAX(area.m_right_down.m_x,m_towerXN);
    area.m_right_down.m_y = p.m_y + m_tower_view_hight+1;
    CHECKMAX(area.m_right_down.m_y,m_towerYN);
}

// void TowerAoi::view_cell_area(Point p,Area &area)
// {
//     //get cell area;
//     area.m_left_up.m_x = p.m_x - m_view_weight;
//     CHECKMIN(area.m_left_up.m_x);
//     area.m_left_up.m_y = p.m_y - m_view_hight;
//     CHECKMIN(area.m_left_up.m_y);
// 
//     area.m_right_down.m_x = p.m_x + m_view_weight + 1;
//     CHECKMAX(area.m_right_down.m_x,m_weight);
//     area.m_right_down.m_y = p.m_y + m_view_hight + 1;
//     CHECKMAX(area.m_right_down.m_y,m_hight);
// }

// void TowerAoi::cellArea2TowerArea(Area &area,Area &tarea)
// {
//     tarea.m_left_up.m_x = area.m_left_up.m_x / m_tower_weight;
//     tarea.m_left_up.m_y = area.m_left_up.m_y / m_tower_hight;
// 
//     tarea.m_right_down.m_x = area.m_right_down.m_x / m_tower_weight;
//     tarea.m_right_down.m_y = area.m_right_down.m_y / m_tower_hight;
// }


//1-get watchers;
//2-add/rm to watchers;//calculate area;
//3-get objects;//current tower;

//add:1,[2,]3;
//rm :1,[2,];
//up :get old-w,and rm self;get new-w;compare,and add self;
//    get old-obj;get new-obj;get same obj;

int32_t TowerAoi::AddWatcher(ObjectId objid,Point &p,ObjectSet &watcher,ObjectSet &objs)
{
    AddOrDeleteWatcher(objid,p,watcher,objs,TOWER_OPERATOR_ADD);
    return 0;
}

int32_t TowerAoi::DelWatcher(ObjectId objid,Point &p,ObjectSet &watcher,ObjectSet &objs)
{
    AddOrDeleteWatcher(objid,p,watcher,objs,TOWER_OPERATOR_DEL);
    return 0;
}

void TowerAoi::AddOrDeleteWatcher(ObjectId objid,Point &p,ObjectSet &watcher,ObjectSet &objs,int32_t optType)
{
    //1-get current tower;
    Point tp = CellToTower(p);
    Tower &tower = m_towers[TowerId(tp.m_x,tp.m_y)];
    watcher.insert(tower.m_watchers.begin(),tower.m_watchers.end());
    //2-  objects;
    if (optType == TOWER_OPERATOR_DEL)
    {
        tower.m_objs.erase(objid);
	  watcher.erase(objid);
    }
    //move to down;
    //else if (optType == TOWER_OPERATOR_ADD)
    //{
    //    tower.m_objs.insert(objid);
    //}
    //3-get tower area;
    Area area,tarea;
    ViewTowers(tp,tarea);
    //view_cell_area(p,area);
    //cellArea2TowerArea(area,tarea);
    //
    for (int y=tarea.m_left_up.m_y; y!=tarea.m_right_down.m_y;++y)
    {
        for (int x=tarea.m_left_up.m_x; x!=tarea.m_right_down.m_x;++x)
        {
            Tower &tower = m_towers[TowerId(x,y)];
            objs.insert(tower.m_objs.begin(),tower.m_objs.end());
            if (optType == TOWER_OPERATOR_ADD)
            {
                tower.m_watchers.insert(objid);
            }
            else if (optType == TOWER_OPERATOR_DEL)
            {
                tower.m_watchers.erase(objid);
            }
        }
    }
    if (optType == TOWER_OPERATOR_ADD)
    {
        tower.m_objs.insert(objid);
    }
}

int32_t TowerAoi::AddObject(ObjectId objid,Point &p,ObjectSet &watcher)
{
    AddOrDeleteObject(objid,p,watcher,TOWER_OPERATOR_ADD);
    return 0;
}

int32_t TowerAoi::DelObject(ObjectId objid,Point &p,ObjectSet &watcher)
{
    AddOrDeleteObject(objid,p,watcher,TOWER_OPERATOR_DEL);
    return 0;
}

void TowerAoi::AddOrDeleteObject(ObjectId objid,Point &p,ObjectSet &watcher,int32_t optType)
{
    //1-get current tower;
    Point tp = CellToTower(p);
    Tower &tower = m_towers[TowerId(tp.m_x,tp.m_y)];
    watcher.insert(tower.m_watchers.begin(),tower.m_watchers.end());
    //2- del object;
    if (optType == TOWER_OPERATOR_ADD)
    {
        tower.m_objs.insert(objid);
    }
    else if (optType == TOWER_OPERATOR_DEL)
    {
        tower.m_objs.erase(objid);
    }
}

int32_t TowerAoi::MoveWatcher(ObjectId objid,Point & op,Point & np,ObjectSet & winsert,ObjectSet & wsame,ObjectSet & wremove,ObjectSet&addobjs,ObjectSet&sameobjs,ObjectSet&rmobjs)
{
    //1-convert point to tower;
    Point oldt = CellToTower(op);
    Point newt = CellToTower(np);
    if (0 == memcmp(&oldt, &newt,sizeof(Point)))
    {
        Area area;
        Tower &tower = m_towers[TowerId(oldt.m_x,oldt.m_y)];
        wsame.insert(tower.m_watchers.begin(),tower.m_watchers.end());
        return 0;
    }
    Tower &oldtower = m_towers[TowerId(oldt.m_x,oldt.m_y)];
    Tower &newtower = m_towers[TowerId(newt.m_x,newt.m_y)];
    oldtower.m_objs.erase(objid);
    
    //
    Area oldtarea;
    ViewTowers(oldt,oldtarea);
    int x=0,y=0;
    ObjectSet newobjs,oldobjs;
    for (y=oldtarea.m_left_up.m_y; y!=oldtarea.m_right_down.m_y;++y)
    {
        for (x=oldtarea.m_left_up.m_x; x!=oldtarea.m_right_down.m_x;++x)
        {
            Tower &tower = m_towers[TowerId(x,y)];
            oldobjs.insert(tower.m_objs.begin(),tower.m_objs.end());
            tower.m_watchers.erase(objid);
        }
    }
    MoveOject(oldtower.m_watchers,newtower.m_watchers, winsert, wsame, wremove);
    wsame.insert(objid);
    
    Area newtarea;
    ViewTowers(newt, newtarea);
    for (y=newtarea.m_left_up.m_y; y!=newtarea.m_right_down.m_y;++y)
    {
        for (x=newtarea.m_left_up.m_x; x!=newtarea.m_right_down.m_x;++x)
        {
            Tower &tower = m_towers[TowerId(x,y)];
            newobjs.insert(tower.m_objs.begin(),tower.m_objs.end());
            tower.m_watchers.insert(objid);
        }
    }
    MoveOject(oldobjs, newobjs, addobjs, sameobjs, rmobjs);
    //
    //oldtower.m_watchers.erase(objid);    
    newtower.m_objs.insert(objid);
    return 0;
}

void TowerAoi::MoveOject(const ObjectSet &oldt,const ObjectSet &newt,ObjectSet & insert,ObjectSet & same,ObjectSet & remove)
{
    ObjectSet::const_iterator coldpos,coldend,cnewpos,cnewend;
    coldpos = oldt.begin();
    coldend = oldt.end();
    cnewpos = newt.begin();
    cnewend = newt.end();
    while (coldpos != coldend && cnewpos != cnewend)
    {
        if (*coldpos > *cnewpos)
        {//new 有,old没有;
            insert.insert(*cnewpos);
            ++cnewpos;
        }
        else if (*coldpos < *cnewpos)
        {//old 有,new没有;
            remove.insert(*coldpos);
            ++coldpos;
        }
        else
        {//都有.
            same.insert(*coldpos);
            ++cnewpos;
            ++coldpos;
        }
    }
    //old 有,new没有;remove;
    if (coldpos != coldend)
    {
        remove.insert(coldpos,coldend);
    }
    //new 有,old没有;insert;
    if (cnewpos != cnewend)
    {
        insert.insert(cnewpos,cnewend);
    }
}

int32_t TowerAoi::MoveObject(ObjectId objid,Point &op,Point &np,ObjectSet &winsert,ObjectSet &wsame,ObjectSet &wremove)
{
    //1-convert point to tower;
    Point oldt = CellToTower(op);
    Point newt = CellToTower(np);
    if (0 == memcmp(&oldt, &newt,sizeof(Point)))
    {//TODO:FUCK;
        return 1;
    }
    Tower &oldtower = m_towers[TowerId(oldt.m_x,oldt.m_y)];
    Tower &newtower = m_towers[TowerId(newt.m_x,newt.m_y)];
    //oldtower.m_watchers.erase(objid);
    oldtower.m_objs.erase(objid);
    MoveOject(oldtower.m_watchers,newtower.m_watchers,winsert,wsame,wremove);
    newtower.m_objs.insert(objid);
    return 0;
}

#ifdef TOWER_TEST
#include <stdio.h>
#else
#define printf
#endif

void TowerAoi::TowerShow()
{
    printf("-------------------------------------------\nTowerShow:\n");
    ObjectSet::const_iterator cpos,cend;
    for (int y=0; y!=m_tower_hight;++y)
    {
        for (int x=0; x!=m_tower_weight;++x)
        {
            ObjectSet &temp = m_towers[TowerId(x,y)].m_watchers;
            if (temp.empty())

            {
                continue;
            }
            printf("(%.2d,%.2d)watchers:",x,y);
            cpos = temp.begin();
            cend = temp.end();
            while(cpos != cend)
            {
                printf("%lld,",*cpos);
                ++cpos;
            }
            ObjectSet &temp2 = m_towers[TowerId(x,y)].m_objs;
            if (!temp2.empty())
            {
                printf(";objects:");
                cpos = temp.begin();
                cend = temp.end();
                while(cpos != cend)
                {
                    printf("%lld,",*cpos);
                    ++cpos;
                }
            }
            printf("\n");
        }
    }
    printf("-------------------------------------------\n");
}


