#ifndef OSMPARSER_H
#define OSMPARSER_H
//...
#include "DataStruct/datastruct.h"

class OSMparser
{
public:
    OSMparser(QString filepath);
    void ShowOSM();
public:
    Bounds bounds;
    QMultiHash <QString ,Node> nodeHash;
    QList <Way> wayList;
    QList <Relation> relationList;
    std::vector<Point *> points;
private:
    QList <Tag> taglist;
    QList <QString> ndlist;
    QList <Member> memberlist;
    Node currentnode;
    Way currentway;
    Relation currentrelation;
    bool ParserOSM(QString filepath);
    bool GetPoints();
    bool GetBounds();
    void ShowNodeHash();
    void ShowWayList();
    void ShowRelationList();
    void ShowTaglist(QList <Tag> Taglist);
    void ShowNdlist(QList <QString> Ndlist);
    void ShowMemberlist(QList <Member> Memberlist);
};

int IsHighway(Way way);//return way type

#endif // OSMPARSER_H
