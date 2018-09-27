#include "vehicle.h"

Vehicle::Vehicle(QWidget *parent) :
    QWidget(parent)
{
    engine_speed=0.0;//0~11kRPM
    engine_temp=90.0;
    vehicle_speed=0.0;//0~120kMPH
    remaining_energy=0.0;
    drive_mode=0;
    steering_angle=0.0;
    //steerang_speed=0.0;
    //break_pressure=0.0;
    //
    distance=50;
    show_index=1;
    //QLabel
    QPalette pe;
    pe.setColor(QPalette::WindowText,"#00ff00");
    QFont font ( "Microsoft YaHei",20, 60);//Size,B
    SysMode_Ret = new QLabel(this);
    SysMode_Ret->setPalette(pe);
    SysMode_Ret->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    SysMode_Ret->setFont(font);
    SysMode_Ret->setGeometry(10,10,240,40);
    showDriving_mode(drive_mode);

    Steering_Ang = new QLabel(this);
    Steering_Ang->setPalette(pe);
    Steering_Ang->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    Steering_Ang->setFont(font);
    Steering_Ang->setGeometry(620,10,360,40);
    showSteering_angle(steering_angle);
/*
    SteerAng_Speed= new QLabel(this);
    SteerAng_Speed->setPalette(pe);
    SteerAng_Speed->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    SteerAng_Speed->setFont(font);
    SteerAng_Speed->setGeometry(620,40,300,40);
    showSteerAng_speed(steerang_speed);

    Breaking_Pressure= new QLabel(this);
    Breaking_Pressure->setPalette(pe);
    Breaking_Pressure->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    Breaking_Pressure->setFont(font);
    Breaking_Pressure->setGeometry(10,40,180,40);
    showBreaking_pressure(break_pressure);
*/
}

void Vehicle::paintEvent(QPaintEvent *)
{
    //pos

    if(show_index==1)
    {
        size1=QPoint(this->width()/2,this->width()/2);
        pos1=QPoint (this->width()/4+distance,this->height()/2);
        size2=QPoint(this->width()/4,this->width()/4);
        pos2=QPoint (7*this->width()/8-distance,this->height()/2);
    }
    else
    {
        size1=QPoint(this->width()/4,this->width()/4);
        pos1=QPoint (this->width()/8+distance,this->height()/2);
        size2=QPoint(this->width()/2,this->width()/2);
        pos2=QPoint (3*this->width()/4-distance,this->height()/2);
    }

    //rect
    QRect rect=this->rect();
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(rect);
    //painter.drawImage(rect,QImage(":/res/background1.jpg"));

    //panel1
    QPainter painter1(this);
    QPixmap panel1(":/res/panel1.png");
    panel1.scaled(100,100,Qt::KeepAspectRatio);
    painter1.drawPixmap(QRect(pos1-size1/2,pos1+size1/2),panel1);

    QPixmap pointer1(":/res/panel1.1.png");
    pointer1.scaled(100,100,Qt::KeepAspectRatio);
    painter1.translate(QPoint(pos1.x()+0.006*size1.x(),pos1.y()+0.042*size1.y()));
    if(vehicle_speed<130)
        painter1.rotate(vehicle_speed*1.9+80);
    else
        painter1.rotate(130*1.9+80);
    painter1.drawPixmap(QRect(0,0,0.1*size1.x(),0.4*size1.y()),pointer1);

    //panel2
    QPainter painter2(this);
    QPixmap panel2(":/res/panel2.png");
    panel2.scaled(100,100,Qt::KeepAspectRatio);
    painter2.drawPixmap(QRect(pos2-size2/2,pos2+size2/2),panel2);

    QPixmap pointer2(":/res/panel2.1.png");
    pointer2.scaled(100,100,Qt::KeepAspectRatio);
    painter2.translate(pos2);
    painter2.rotate(engine_speed*19.8+31);
    painter2.drawPixmap(QRect(0,0,5.5*size2.x()/24,5.5*size2.x()/15),pointer2);

    //text
    QPainter painter3(this);
    int x=pos1.x()-size1.x()/6;
    int y=pos1.y()+size1.y()/8;
    painter3.setPen(QPen(Qt::cyan, size1.x()/40));
    painter3.setFont(QFont("Microsoft YaHei", size1.x()/8));
    if(vehicle_speed>=100)
        painter3.drawText(x,y,size1.x()/2,size1.y()/4, Qt::AlignLeft|Qt::AlignTop, QString::number(vehicle_speed,10,0));
    else
        painter3.drawText(x,y,size1.x()/2,size1.y()/4, Qt::AlignLeft|Qt::AlignTop, QString::number(vehicle_speed,10,1));

    x=pos2.x()+size2.x()/10;
    y=pos2.y()+size2.y()/7;
    painter3.setPen(QPen(Qt::cyan, size2.x()/80));
    painter3.setFont(QFont("Microsoft YaHei", size2.x()/16));
    painter3.drawText(x,y,size2.x()/2,size2.y()/4, Qt::AlignLeft|Qt::AlignTop, QString::number(engine_speed,10,2));

    painter1.setRenderHint(QPainter::SmoothPixmapTransform,true);
    painter2.setRenderHint(QPainter::SmoothPixmapTransform,true);
}

void Vehicle::mousePressEvent(QMouseEvent *event)
{
    QPoint pos=event->pos();
    int length1=(pos-pos1).manhattanLength();
    int length2=(pos-pos2).manhattanLength();
    if((size1/2.83).manhattanLength()>length1)
    {
        //qDebug()<<1;
        show_index=1;
    }
    if((size2/2.83).manhattanLength()>length2)
    {
        //qDebug()<<2;
        show_index=2;
    }
    this->repaint();
}

void Vehicle::showVehicle_speed(double vehicle_speed)
{
    //if(vehicle_speed>120)vehicle_speed=0;
    this->vehicle_speed=vehicle_speed;
    this->repaint();//Only one or hight time cost
}

void Vehicle::showEngine_temp(double engine_temp)
{
    this->engine_temp=engine_temp;
}

void Vehicle::showEngine_speed(double engine_speed)
{
    if(engine_speed>11)engine_speed=0;
    this->engine_speed=engine_speed;
}

void Vehicle::showRemaining_energy(double remaining_energy)
{
    this->remaining_energy=remaining_energy;
}

void Vehicle::showDriving_mode(int mode)
{
    drive_mode=mode;
    if(drive_mode==0)
        SysMode_Ret->setText(tr("Manu Mode"));
    else if(drive_mode==1)
        SysMode_Ret->setText(tr("Lon_Auto Mode"));
    else if(drive_mode==2)
        SysMode_Ret->setText(tr("Lat_Auto Mode"));
    else if(drive_mode==3)
        SysMode_Ret->setText(tr("Auto Mode"));
    else
        SysMode_Ret->setText(tr("Undef Mode"));
}

void Vehicle::showSteering_angle(double deg)
{
    steering_angle=deg;
    Steering_Ang->setText(tr("Steering_Ang:")+QString::number(steering_angle,10,5));
}
/*
void Vehicle::showSteerAng_speed(double degps)
{
    steerang_speed=degps;
    SteerAng_Speed->setText(tr("SteerAng_Speed:")+QString::number(steerang_speed,10,5));
}

void Vehicle::showBreaking_pressure(double pressure)
{
    break_pressure=pressure;
    Breaking_Pressure->setText(tr("Breaking_Pressure:")+QString::number(break_pressure,10,1));
}
*/
void Vehicle::receive_ecu_data(ECU2HMI_DATA data){
 this->showDriving_mode(data.dt0_drive_mode);
 this->showSteering_angle(data.dt2_vehicle_stangle);
 this->showVehicle_speed(data.dt7_vehicle_speed);
}
