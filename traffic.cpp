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
        a[i]=0;
        b[i]=0;
        c[i]=0;
    }
    for(int j=0;j<5;j++)
    {
        obj_x[j]=0;
        obj_y[j]=0;
        obj_ang[j]=0;
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
        if(obj_x[obj]==0&&obj_y[obj]==0&&obj_width[obj]==0&&obj_ang[obj]==0&&obj_class[obj]==0)break;
        double x=obj_x[obj];
        double y=obj_y[obj];
        QPoint winpos=vhcXY2winXY(x,y);
        painter_obj.translate(winpos.x(),winpos.y());
        painter_obj.rotate(90+obj_ang[obj]*180/3.1415926);
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
}

void Traffic::receive_ecu_data(ECU2HMI_DATA data)
{
    this->showNRYG_light(data.dt8_traffic_sign);
    int i=abs(data.dt9_lane_id)%4;
    int j=abs(data.dt15_object_id)%5;
    a[i]=data.dt10_lane_a;
    b[i]=data.dt11_lane_b;
    c[i]=data.dt12_lane_c;
    obj_x[j]=data.dt16_object_x;
    obj_y[j]=data.dt17_object_y;
    obj_ang[j]=data.dt24_object_angle;
    obj_width[j]=data.dt21_object_width;
    obj_class[j]=data.dt20_object_class;
//    qDebug()<<"test"<<i<<j;
//    qDebug()<<"pos0"<<obj_x[0]<<obj_y[0];
//    qDebug()<<"pos1"<<obj_x[1]<<obj_y[1];
//    this->repaint();
}
