#include "xmlparser.h"

OSMparser::OSMparser(QString filepath)
{
    if(!ParserOSM(filepath))
        qDebug()<<"osmparser_Open file xx.XML failure";
    if(!this->GetPoints())
        qDebug()<<"osmparser_ data2points failure";
}

bool OSMparser::ParserOSM(QString filepath)
{
   taglist.clear();
   ndlist.clear();
   memberlist.clear();
   nodeHash.clear();
   wayList.clear();
   relationList.clear();

   //将xml文件读取数据
   QFile file(filepath);
   if(file.open(QIODevice::ReadOnly | QIODevice::Text))
   {
       //构建QXmlStreamReader对象
       QXmlStreamReader reader(&file);
       while(!reader.atEnd())
       {
           //判断是否是节点的开始
           if(reader.isStartElement())
           {
               //将属性读出:例如 id="3" age = "23">
               QXmlStreamAttributes attributes = reader.attributes();
               if(reader.name() == "node")
               {
                   if(attributes.hasAttribute("id"))
                   {
                       currentnode.id=attributes.value("id").toString();
                   }
                   if(attributes.hasAttribute("lat"))
                   {
                       currentnode.lat=attributes.value("lat").toDouble(nullptr);
                   }
                   if(attributes.hasAttribute("lon"))
                   {
                       currentnode.lon=attributes.value("lon").toDouble(nullptr);
                   }
                   if(attributes.hasAttribute("lspeed"))
                   {
                       currentnode.lspeed=attributes.value("lspeed").toDouble(nullptr);
                   }
                   //qDebug()<<reader.name()<<currentnode.id<<currentnode.lat<<currentnode.lon;
               }
               else if(reader.name() == "way")
               {
                   if(attributes.hasAttribute("id"))
                   {
                       currentway.id=attributes.value("id").toString();
                   }
                   //qDebug()<<reader.name()<<currentway.id;
               }
               else if(reader.name() == "relation")
               {
                   if(attributes.hasAttribute("id"))
                   {
                       currentrelation.id=attributes.value("id").toString();
                   }
                   //qDebug()<<reader.name()<<currentrelation.id;
               }
               else if(reader.name() == "tag")
               {
                   Tag tag;
                   if(attributes.hasAttribute("k"))
                   {
                       tag.k=attributes.value("k").toString();
                   }
                   if(attributes.hasAttribute("v"))
                   {
                       tag.v=attributes.value("v").toString();
                   }
                   taglist.append(tag);
                   //qDebug()<<reader.name()<<taglist.back().k<<taglist.back().v;
               }
               else if(reader.name() == "nd")
               {
                   if(attributes.hasAttribute("ref"))
                   {
                       ndlist.append(attributes.value("ref").toString());
                   }
                   //qDebug()<<reader.name()<<ndlist.back();
               }
               else if(reader.name() == "member")
               {
                   Member member;
                   if(attributes.hasAttribute("type"))
                   {
                       member.type=attributes.value("type").toString();
                   }
                   if(attributes.hasAttribute("ref"))
                   {
                       member.ref=attributes.value("ref").toString();
                   }
                   if(attributes.hasAttribute("role"))
                   {
                       member.role=attributes.value("role").toString();
                   }
                   memberlist.append(member);
                   //qDebug()<<reader.name()<<memberlist.back().type<<memberlist.back().ref;
               }
               else if(reader.name() == "bounds")
               {
                   if(attributes.hasAttribute("minlat"))
                   {
                       bounds.minlat=attributes.value("minlat").toDouble(nullptr);
                   }
                   if(attributes.hasAttribute("minlon"))
                   {
                       bounds.minlon=attributes.value("minlon").toDouble(nullptr);
                   }
                   if(attributes.hasAttribute("maxlat"))
                   {
                       bounds.maxlat=attributes.value("maxlat").toDouble(nullptr);
                   }
                   if(attributes.hasAttribute("maxlon"))
                   {
                       bounds.maxlon=attributes.value("maxlon").toDouble(nullptr);
                   }

               }
               //else;
            }
            //节点结束
            else if(reader.isEndElement())
            {
               //并且节点名字为Data（含有子节点）
               if(reader.name() == "node")
               {
                   currentnode.taglist=taglist;
                   taglist.clear();
                   nodeHash.insert(currentnode.id,currentnode);
                   //delete currentnode;
               }
               else if(reader.name() == "way")
               {
                   currentway.ndlist=ndlist;
                   currentway.taglist=taglist;
                   ndlist.clear();
                   taglist.clear();
                   wayList.append(currentway);
               }
               else if(reader.name() == "relation")
               {
                   currentrelation.memberlist=memberlist;
                   currentrelation.taglist=taglist;
                   memberlist.clear();
                   taglist.clear();
                   relationList.append(currentrelation);
               }
               //else;
            }
            reader.readNext();
       }
            file.close();
   }
   else
       return false;
   return true;
}

bool OSMparser::GetBounds()
{
    return true;
}

bool OSMparser::GetPoints()
{
    QMultiHash <QString ,Point*> pointHash;
    for(int i=0;i<wayList.size();i++)
    {
        Way way=wayList.at(i);
        if(way.ndlist.size()<2)
            break;
        //download
        for(int j=0;j<way.ndlist.size();j++)
        {
          QString IDn=way.ndlist.at(j);
          Node node=nodeHash.value(IDn);
          if(!pointHash.value(IDn))
          {
              Point *point=new Point(IDn,node.lon,node.lat);
              point->lspeed=node.lspeed;
              pointHash.insert(IDn,point);
          }
          //else;
        }
        //neighbor
        for(int j=0;j<way.ndlist.size();j++)
        {
          QString IDn=way.ndlist.at(j);
          /*
          if(j>0)
          {
              QString IDnf=way.ndlist.at(j-1);
              pointHash.value(IDn)->surroundPoints.push_back(pointHash.value(IDnf));
          }
          */
          if(j<way.ndlist.size()-1)
          {
              QString IDnr=way.ndlist.at(j+1);
              pointHash.value(IDn)->surroundPoints.push_back(pointHash.value(IDnr));
          }
        }

    }
    points.clear();
    for (auto &p : pointHash)
    points.push_back(p);
    return true;
}

void OSMparser::ShowOSM()
{

    qDebug()<<"bounds:"<<bounds.minlat<<bounds.minlon<<bounds.maxlat<<bounds.maxlon;
    qDebug()<<"----------nodeHash---------";
    this->ShowNodeHash();
    qDebug()<<"----------wayList----------";
    this->ShowWayList();
    qDebug()<<"----------RelationList-----";
    this->ShowRelationList();
    qDebug()<<"---------------------------";
}

void OSMparser::ShowNodeHash()
{
    QList<Node> nodeList=nodeHash.values();
    for(int i=0;i<nodeList.size();i++)
    {
        Node node=nodeList.at(i);
        qDebug()<<" node"<<node.id<<node.lat<<node.lon;
        this->ShowTaglist(node.taglist);
    }
}

void OSMparser::ShowWayList()
{
    for(int i=0;i<wayList.size();i++)
    {
        Way way=wayList.at(i);
        qDebug()<<" way:"<<way.id;
        this->ShowNdlist(way.ndlist);
        this->ShowTaglist(way.taglist);
    }
}

void OSMparser::ShowRelationList()
{
    for(int i=0;i<relationList.size();i++)
    {
        Relation relation=relationList.at(i);
        qDebug()<<" relation:"<<relation.id;
        this->ShowMemberlist(relation.memberlist);
        this->ShowTaglist(relation.taglist);
    }
}

void OSMparser::ShowTaglist(QList <Tag> Taglist)
{
    for(int i=0;i<Taglist.size();i++)
    {
        Tag tag=Taglist.at(i);
        qDebug()<<"  tag:"<<tag.k<<tag.v;
    }
}

void OSMparser::ShowNdlist(QList <QString> Ndlist)
{
    for(int i=0;i<Ndlist.size();i++)
    {
        QString ndref=Ndlist.at(i);
        qDebug()<<"  nd:"<<ndref;
    }
}

void OSMparser::ShowMemberlist(QList <Member> Memberlist)
{
    for(int i=0;i<Memberlist.size();i++)
    {
        Member member=Memberlist.at(i);
        qDebug()<<"  member:"<<member.ref<<member.role<<member.type;
    }
}

int IsHighway(Way way)
{
    //等级1，2，3公路，住宅道路4，无等级5，胡同service6
    QList <Tag> Taglist=way.taglist;
    for(int i=0;i<Taglist.size();i++)
    {
        Tag tag=Taglist.at(i);
        if(tag.k==("highway"))
        {
            if(tag.v==("primary"))
                return 1;
            else if(tag.v==("secondary"))
                return 2;
            else if(tag.v==("tertiary"))
                return 3;
            else if(tag.v==("residential"))
                return 4;
            else if(tag.v==("service"))
                return 5;
        }

    }
    return 0;
}
