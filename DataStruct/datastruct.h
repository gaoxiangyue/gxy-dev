#ifndef DATASTRUCT_H
#define DATASTRUCT_H
//...
#include <QMultiHash>
#include <vector>
#include <list>
#include <string>
//...
#include <QFile>
#include <QXmlStreamReader>
#include <QFileDialog>
#include <QPainter>
#include <QDebug>
#include <QString>

//...
    typedef struct Bounds{
        double minlat;
        double minlon;
        double maxlat;
        double maxlon;
    } Bounds;
    typedef struct Tag{
        QString k;
        QString v;
    } Tag;
    typedef struct Member{
        QString type;
        QString ref;
        QString role;
    } Member;
    typedef struct Node{
        QString id;
        double lat;
        double lon;
        int lspeed=36;
        QList <Tag> taglist;
    } Node;
    typedef struct Way{
        QString id;
        QList <QString> ndlist;
        QList <Tag> taglist;
    } Way;
    typedef struct Relation{
        QString id;
        QList <Member> memberlist;
        QList <Tag> taglist;
    } Relation;

//...
struct Point
{
    QString ID;
    int lspeed;
    double x, y; //节点经纬度，这里为了方便按照C++的数组来计算，x代表经度，y代表纬度
    double F, G, H; //F=G+H
    Point *parent; //parent的坐标，这里没有用指针，从而简化代码
    std::vector<Point*> surroundPoints;
    Point(QString _ID,double _x, double _y) :ID(_ID), x(_x), y(_y),lspeed(36), F(0), G(0), H(0), parent(NULL)  //变量初始化
    {
    }
};


typedef struct HMI2ECU_DATA{
        int dt0_systemMode;
        int dt1_cruisingSpeed;
        int dt2_timeHeadway;
        int dt3_RoadPointNo;
        double dt4_RoadPointLon;
        double dt5_RoadPointLat;
        double dt6_RoadPointAlt;
        int dt7_RoadPointLspeed;
        int dt8_ReservedRet;
        int dt9_ReservedRet;
    } HMI2ECU_DATA;

typedef struct ECU2HMI_DATA{
        int dt0_drive_mode;
        int dt1_system_state;
        int dt2_vehicle_stangle;
        int dt3_navi_id;
        double dt4_vehicle_lon;
        double dt5_vehicle_lat;
        double dt6_vehicle_alt;
        double dt7_vehicle_speed;
        int dt8_traffic_sign;
        //Lane_Parameters
        int dt9_lane_id;
        double dt10_lane_a;
        double dt11_lane_b;
        double dt12_lane_c;
        int dt13_lane_class;
        double dt14_lane_width;
        //Object_Parameters
        int dt15_object_id;
        double dt16_object_x;
        double dt17_object_y;
        double dt18_object_vx;
        double dt19_object_vy;
        int dt20_object_class;
        double dt21_object_width;
        double dt22_object_length;
        double dt23_object_height;
        double dt24_object_angle;
        double dt25_reserved_ret;
    } ECU2HMI_DATA;


class Datastruct
{
public:
    Datastruct();
};

#endif // DATASTRUCT_H
