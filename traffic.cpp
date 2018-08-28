#include "traffic.h"
#include <QDebug>

Traffic::Traffic(QWidget *parent) : QWidget(parent)
{
    scale_light=2;
    w_car=2;
    h_car=4;
    x_range=20;
    for(int i=0;i<4;i++)
    {
        a[i]=0.002;
        b[i]=0;
        c[i]=2.5-2*i;
        a[i]=0;
        b[i]=0;
        c[i]=0;
    }
    for(int j=0;j<5;j++)
    {
        obj_dis[j]=10*(j+1);
        obj_ang[j]=j-2;
        obj_hdg[j]=j;
        obj_width[j]=j+1;
        obj_class[j]=1;

        obj_dis[j]=0;
        obj_ang[j]=0;
        obj_hdg[j]=0;
        obj_width[j]=0;
        obj_class[j]=0;
    }
    QPixmap plight(":/res/trafficlight0.fw.png");
    label_light = new QLabel(this);
    label_light->setGeometry(10,10,60*scale_light,120*scale_light);
    label_light->setPixmap(plight.scaled(60*scale_light,120*scale_light,Qt::KeepAspectRatio));
}

void Traffic::paintEvent(QPaintEvent *)
{
    //background
    QRect rect=this->rect();
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(rect);

    scale_x=this->height()/x_range;
    scale_y=scale_x;

    //road
    QPainter painter_road(this);
    painter_road.setPen(QPen(Qt::white, scale_x/5));
    for(int line=0;line<4;line++)
    {
        if(a[line]==0&&b[line]==0&&c[line]==0)break;
        QPoint *points=new QPoint[x_range+30];
        for(int x=-30;x<x_range;x++)
        {
            double y=a[line]*x*x+b[line]*x+c[line];
            QPoint winpos=vhcXY2winXY(x,y);
            points[x+30]=winpos;
        }
        //painter_road.drawPolyline(points,x_range+30);
        painter_road.drawLines(points,(x_range+30)/2);
        delete points;
    }

    //obj
    for(int obj=0;obj<5;obj++)
    {
        QPainter painter_obj(this);
        QString objmap;
        if(obj_dis[obj]==0&&obj_ang[obj]==0&&obj_hdg[obj]==0&&obj_width[obj]==0&&obj_class[obj]==0)break;
        double x=obj_dis[obj]*cos(obj_ang[obj]);
        double y=obj_dis[obj]*sin(obj_ang[obj]);
        QPoint winpos=vhcXY2winXY(x,y);
        painter_obj.translate(winpos.x(),winpos.y());
        painter_obj.rotate(90+obj_hdg[obj]*180/3.1415926);
        painter_obj.translate(-obj_width[obj]*scale_x,-obj_width[obj]*scale_y/2);
        if(obj_class[obj]==0)//vehicle
            objmap=tr(":/res/car_t21.png");
        else if(obj_class[obj]==1)//pedestrian
            objmap=tr(":/res/car_t21.png");
        else if(obj_class[obj]==2)//bicycle
            objmap=tr(":/res/car_t21.png");
        else
            objmap=tr(":/res/car_t21.png");
        painter_obj.drawPixmap(QRect(0,0,2*obj_width[obj]*scale_y,
                              obj_width[obj]*scale_x),QPixmap(objmap));
    }
    //car
    QPainter painter_car(this);
    QPixmap pcar(":/res/car_t2.png");
    pcar.scaled(h_car*scale_y,w_car*scale_x,Qt::KeepAspectRatio);
    painter_car.translate(vhcXY2winXY(0,0).x(),vhcXY2winXY(0,0).y());
    painter_car.rotate(90);
    painter_car.translate(-h_car*scale_x/2,-w_car*scale_y/2);
    painter_car.drawPixmap(QRect(0,0,h_car*scale_y,w_car*scale_x),pcar);
}

void Traffic::mousePressEvent(QMouseEvent *event)
{
    presspos=event->pos();
    //repaint();
}

void Traffic::mouseMoveEvent(QMouseEvent *event)
{
    moveingpos=(event->pos()-presspos);
    x_range=x_range-moveingpos.x()/20;
    if(x_range<8)x_range=8;
    if(x_range>100)x_range=100;
    this->repaint();
}

QPoint Traffic::vhcXY2winXY(double vhcX, double vhcY)
{
    QPoint winXY;
    winXY.setX(this->width()/2-scale_y*vhcY);
    winXY.setY(this->height()-scale_x*vhcX-h_car*scale_x/2-scale_x*x_range/4);
    return winXY;
}

void Traffic::showNRYG_light(int nrgy)
{
    QPixmap plight;
    if(nrgy==1)
        plight=QPixmap(":/res/trafficlight1.fw.png");
    else if(nrgy==2)
        plight=QPixmap(":/res/trafficlight2.fw.png");
    else if(nrgy==3)
        plight=QPixmap(":/res/trafficlight3.fw.png");
    else
        plight=QPixmap(":/res/trafficlight0.fw.png");
    label_light->setPixmap(plight.scaled(60*scale_light,120*scale_light,Qt::KeepAspectRatio));
    this->repaint();//Only one or hight time cost
}

//Lane
void Traffic::show_L0_a(double a0){a[0]=a0;}
void Traffic::show_L0_b(double b0){b[0]=b0;}
void Traffic::show_L0_c(double c0){c[0]=c0;}
void Traffic::show_L1_a(double a1){a[1]=a1;}
void Traffic::show_L1_b(double b1){b[1]=b1;}
void Traffic::show_L1_c(double c1){c[1]=c1;}
void Traffic::show_L2_a(double a2){a[2]=a2;}
void Traffic::show_L2_b(double b2){b[2]=b2;}
void Traffic::show_L2_c(double c2){c[2]=c2;}
void Traffic::show_L3_a(double a3){a[3]=a3;}
void Traffic::show_L3_b(double b3){b[3]=b3;}
void Traffic::show_L3_c(double c3){c[3]=c3;}
//Obj
void Traffic::show_Obj0_dis(double dis0){obj_dis[0]=dis0;}
void Traffic::show_Obj0_angle(double angle0){obj_ang[0]=angle0;}
void Traffic::show_Obj0_heading(double heading0){obj_hdg[0]=heading0;}
void Traffic::show_Obj0_width(double width0){obj_width[0]=width0;}
void Traffic::show_Obj0_class(double class0){obj_class[0]=class0;}
void Traffic::show_Obj1_dis(double dis1){obj_dis[1]=dis1;}
void Traffic::show_Obj1_angle(double angle1){obj_ang[1]=angle1;}
void Traffic::show_Obj1_heading(double heading1){obj_hdg[1]=heading1;}
void Traffic::show_Obj1_width(double width1){obj_width[1]=width1;}
void Traffic::show_Obj1_class(double class1){obj_class[1]=class1;}
void Traffic::show_Obj2_dis(double dis2){obj_dis[2]=dis2;}
void Traffic::show_Obj2_angle(double angle2){obj_ang[2]=angle2;}
void Traffic::show_Obj2_heading(double heading2){obj_hdg[2]=heading2;}
void Traffic::show_Obj2_width(double width2){obj_width[2]=width2;}
void Traffic::show_Obj2_class(double class2){obj_class[2]=class2;}
void Traffic::show_Obj3_dis(double dis3){obj_dis[3]=dis3;}
void Traffic::show_Obj3_angle(double angle3){obj_ang[3]=angle3;}
void Traffic::show_Obj3_heading(double heading3){obj_hdg[3]=heading3;}
void Traffic::show_Obj3_width(double width3){obj_width[3]=width3;}
void Traffic::show_Obj3_class(double class3){obj_class[3]=class3;}
void Traffic::show_Obj4_dis(double dis4){obj_dis[4]=dis4;}
void Traffic::show_Obj4_angle(double angle4){obj_ang[4]=angle4;}
void Traffic::show_Obj4_heading(double heading4){obj_hdg[4]=heading4;}
void Traffic::show_Obj4_width(double width4){obj_width[4]=width4;}
void Traffic::show_Obj4_class(double class4){obj_class[4]=class4;}
