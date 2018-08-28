#include "mapwidget.h"
#include "ui_widget.h"
#include <QFile>
#include <QTextSTream>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //this->showFullScreen();
    connect(ui->scaleSlider,SIGNAL(sliderMoved(int)),this,SLOT(rescale()));
    connect(ui->scaleSlider,SIGNAL(actionTriggered(int)),this,SLOT(rescale()));
    connect(ui->ImportButton,SIGNAL(clicked(bool)),this,SLOT(OpenMapFile()));
    connect(ui->NavigationBox,SIGNAL(toggled(bool)),this,SLOT(navigationshow(bool)));
    viewinit();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::viewinit()
{
    ui->scaleSlider->setRange(1,100);
    ui->scaleSlider->setValue(50);
    scale=1;
    ui->scaleSlider->hide();
    ui->NavigationBox->hide();
    ui->start_select_bt->hide();
    ui->startpos->hide();
    ui->end_select_bt->hide();
    ui->endpos->hide();

    movedpos=QPoint(0,0);
    moveingpos=QPoint(0,0);
    selectflag=1;
    ui->startpos->clear();
    ui->endpos->clear();
    //初始默认路径加载Map
    //this_filename
    //OpenMapFile()
}

void Widget::paintEvent(QPaintEvent *){
       QPainter painter(this);
       painter.setBrush(Qt::black);
       painter.drawRect(this->rect());
       ui->NavigationBox->setGeometry(this->width()-241,10,241,81);
       ui->scaleSlider->setGeometry(20,this->height()-180,16,160);

       //Draw roadNet
       for(int i=0;i<wayList.size();i++)
       {
           Way way=wayList.at(i);
           painter.setPen(QPen(Qt::darkMagenta, 1));
           for(int j=1;j<way.ndlist.size();j++)
           {
               Node node1=nodeHash.value(way.ndlist.at(j-1));
               Node node2=nodeHash.value(way.ndlist.at(j));
               painter.drawLine(QPointF(geo2window(node1)),QPointF(geo2window(node2)));
           }
       }

       //Draw Nodes
       QList<Node> nodeList=nodeHash.values();
       QPointF *points=new QPointF[nodeList.size()];
       for(int i=0;i<nodeList.size();i++)
       {
           Node node=nodeList.at(i);
           points[i]=QPointF(geo2window(node));
       }
       painter.setPen(QPen(Qt::green, 2));
       painter.drawPoints(points,nodeList.size());
       delete points;

       //Draw path
       painter.setPen(QPen(Qt::blue, 4));
       if(!path.empty()&&(selectflag!=2))
           {
           auto p=path.front();
           Node node1;Node node2;
           node1.lon=p->x;
           node1.lat=p->y;
           QPoint pos1=geo2window(node1);
           for (auto &p : path)
           {
             node2.lon=p->x;
             node2.lat=p->y;
             QPoint pos2=geo2window(node2);
             painter.drawLine(pos1,pos2);
             pos1=pos2;
           }

           QPoint startpos=geo2window(start);
           painter.drawPixmap(startpos.x()-15,startpos.y()-50,30,50, QPixmap(":/res/pos_s.png"));
           QPoint endpos=geo2window(end);
           painter.drawPixmap(endpos.x()-15,endpos.y()-50,30,50, QPixmap(":/res/pos_e.png"));
           }

       //Start point
       if(selectflag==2)
       {
           QPoint startpos=geo2window(start);
           painter.drawPixmap(startpos.x()-15,startpos.y()-50,30,50, QPixmap(":/res/pos_s.png"));
       }

       //Test
       painter.setPen(QPen(Qt::red, 4));
       painter.drawPoint(geo2window(geocenter));
}

void Widget::OpenMapFile()
{

     QString this_filename = QFileDialog::getOpenFileName(this,tr("OpenFile"),"","VCI_hdMap(*.xml;*.XML);;OpenStreetMap(*.osm;*.OSM)");
    //if(this_filename.isEmpty())
        //return;
    filename=this_filename;

    OSMparser se(filename);
    bounds=se.bounds;
    nodeHash=se.nodeHash;
    wayList=se.wayList;
    points=se.points;
    //se.ShowOSM();
    geocenter.lon=(bounds.minlon+bounds.maxlon)/2;
    geocenter.lat=(bounds.minlat+bounds.maxlat)/2;

    viewinit();
    ui->scaleSlider->show();
    ui->NavigationBox->show();
    ui->NavigationBox->setChecked(true);
    ui->start_select_bt->show();
    ui->startpos->show();
    ui->end_select_bt->show();
    ui->endpos->show();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    presspos=event->pos();
    if(!ui->NavigationBox->isChecked())
        return;
    if(selectflag==1)
    {//select start node
        start=getnearstpoint(window2geo(presspos));
        ui->startpos->setText("<"+QString::number(start.lon,10,7)+","+QString::number(start.lat,10,7)+">");
        ui->endpos->setText("Input End Position...");
        selectflag=2;
    }
    else if(selectflag==2)
    {//select end node
        end=getnearstpoint(window2geo(presspos));
        ui->endpos->setText("<"+QString::number(end.lon,10,7)+","+QString::number(end.lat,10,7)+">");
        Astar astar;
        astar.InitAstar(points);
        Point s("",start.lon,start.lat);
        Point e("",end.lon,end.lat);
        path.clear();
        path = astar.GetPath(s,e, false);
        writepath2mfile(path);
        selectflag=3;
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    moveingpos=movedpos+(event->pos()-presspos)/scale;
    update();
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    movedpos+=(event->pos()-presspos)/scale;
    update();
}

void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<"DoubleClick on:"<<event->pos();
    Node node=window2geo(event->pos());
    qDebug()<<node.lat<<node.lon;
    qDebug()<<geo2window(node);
    setgeocenter(node);
}

void Widget::rescale()
{
    scale=double(ui->scaleSlider->value())/50;
    this->repaint();
}

QPoint Widget::geo2window(Node node)
{
    double dlat=node.lat-geocenter.lat;
    double dlon=node.lon-geocenter.lon;
    double x=dlon*1.1132e+05*cos(0.01745*geocenter.lat);
    double y=-dlat*1.1132e+05;
    x=x+moveingpos.x();
    y=y+moveingpos.y();
    return QPoint(x*scale+this->width()/2,y*scale+this->height()/2);
}

Node Widget::window2geo(QPoint pos)
{
    double x=pos.x()-this->width()/2;
    double y=pos.y()-this->height()/2;
    x=x/scale;
    y=y/scale;
    x=x-moveingpos.x();
    y=y-moveingpos.y();
    double dlat=-y/1.1132e+05;
    double dlon=x/(1.1132e+05*cos(0.01745*geocenter.lat));
    Node node;
    node.lat=dlat+geocenter.lat;
    node.lon=dlon+geocenter.lon;
    return node;
}

void Widget::setgeocenter(Node node)
{
    QPoint setpos=geo2window(node);
    QPoint wincenter=QPoint(this->width()/2,this->height()/2);
    moveingpos+=(wincenter-setpos)/scale;
    movedpos+=(wincenter-setpos)/scale;
    this->repaint();
}

void Widget::navigationshow(bool s)
{
    if(!s)
    {
        ui->start_select_bt->hide();
        ui->startpos->hide();
        ui->end_select_bt->hide();
        ui->endpos->hide();
    }
    else
    {
        ui->start_select_bt->show();
        ui->startpos->show();
        ui->end_select_bt->show();
        ui->endpos->show();
        if(selectflag==0)
        {
            selectflag=1;
            ui->startpos->setText("Input Start Position...");
        }
        else if(selectflag==3)
        {
            selectflag=1;
            ui->startpos->setText("Input Start Position...");
            OSMparser se(filename);
            bounds=se.bounds;
            nodeHash=se.nodeHash;
            wayList=se.wayList;
            points=se.points;
        }
    }
}

Node Widget::getnearstpoint(Node node)
{
    Node nearstnode=node;
    double dicetanceThreshold=1000;
    QList<Node> nodeList=nodeHash.values();
    for(int i=0;i<nodeList.size();i++)
    {
        Node standnode=nodeList.at(i);
        QPoint standwp=geo2window(standnode);
        QPoint nodewp=geo2window(node);
        double distance=(standwp-nodewp).manhattanLength();
        if(dicetanceThreshold>distance)
        {
            dicetanceThreshold=distance;
            nearstnode=standnode;
        }
    }
    qDebug()<<nearstnode.lon<<nearstnode.lat;

    return nearstnode;
}

bool Widget::writepath2mfile(std::list<Point *> path)
{
    QFile file("hdmap.m");   //Write2filename.m
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return false;
    QTextStream out(&file);
    QString lonarray,latarray,altarray,lspeedarray;
    for (auto &p : path)
    {
        lonarray=lonarray+tr(" ")+QString::number(p->x,10,7);
        latarray=latarray+tr(" ")+QString::number(p->y,10,7);
        altarray=altarray+tr(" ")+QString::number(200,10,0);
        lspeedarray=lspeedarray+tr(" ")+QString::number(p->lspeed,10,0);
    }
    qDebug()<<tr("Write mfile:");
    qDebug()<<lonarray;
    qDebug()<<latarray;
    qDebug()<<altarray;
    qDebug()<<lspeedarray;

    out<<tr("% -------------------------------------------------------------------")<<endl;
    out<<tr("%Generated by RoadMapv1.0 on 2018")<<endl;
    out<<tr("%VCIer you can contact us by sending email:2580253221@qq.com")<<endl;
    out<<tr("% -------------------------------------------------------------------")<<endl<<endl;
    out<<tr("HDmap = struct;")<<endl;
    out<<tr("HDmap.lonarray = [")<<lonarray<<tr("];")<<endl;
    out<<tr("HDmap.latarray = [")<<latarray<<tr("];")<<endl;
    out<<tr("HDmap.altarray = [")<<altarray<<tr("];")<<endl;
    out<<tr("HDmap.lspeedarray = [")<<lspeedarray<<tr("];")<<endl;

    file.close();
    return true;
}
