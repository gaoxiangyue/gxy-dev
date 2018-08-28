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
    double x, y; //节点经纬度，这里为了方便按照C++的数组来计算，x代表精度，y代表纬度
    double F, G, H; //F=G+H
    Point *parent; //parent的坐标，这里没有用指针，从而简化代码
    std::vector<Point*> surroundPoints;
    Point(QString _ID,double _x, double _y) :ID(_ID), x(_x), y(_y),lspeed(36), F(0), G(0), H(0), parent(NULL)  //变量初始化
    {
    }
};

class Datastruct
{
public:
    Datastruct();
};

#endif // DATASTRUCT_H
