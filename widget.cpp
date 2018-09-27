#include "widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    left_num=7;
    setWindowTitle(tr("IVHMI vci1.0"));
    this->resize(1280,800);
    //this->resize(1440,900);
    //this->resize(640,400);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->showFullScreen();

    QMovie *homeMovie=new QMovie(":/res/homeMovie.gif");
    home = new QLabel(this);
    home->setMovie(homeMovie);
    homeMovie->setScaledSize(QSize(this->width()-this->height()/left_num-18,this->height()));
    homeMovie->start();
    app=new App(this);
    setting=new Setting(this);
    map=new MapWidget(this);
    vehicle=new Vehicle(this);
    control=new ControlWidget(this);
    traffic=new Traffic(this);

    //stackedWidget重叠控件
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(map);
    stackedWidget->addWidget(control);
    stackedWidget->addWidget(traffic);
    stackedWidget->addWidget(vehicle);
    stackedWidget->addWidget(home);
    stackedWidget->addWidget(setting);
    stackedWidget->addWidget(app);

    Button *mylabel0=new Button(":/res/mappos0.png",":/res/mappos1.png",":/res/mappos4.png");
    Button *mylabel1=new Button(":/res/steering_wheel0.png",":/res/steering_wheel1.png",":/res/steering_wheel4.png"); 
    Button *mylabel2=new Button(":/res/traffic_sensor0.png",":/res/traffic_sensor1.png",":/res/traffic_sensor2.png");
    Button *mylabel3=new Button(":/res/car0.png",":/res/car1.png",":/res/car2.png");
    Button *mylabel4=new Button(":/res/home0.png",":/res/home1.png",":/res/home2.png");
    Button *mylabel5=new Button(":/res/setting0.png",":/res/setting1.png",":/res/setting2.png");
    Button *mylabel6=new Button(":/res/App0.png",":/res/App1.png",":/res/App2.png");

    connect(mylabel0,SIGNAL(clicked(bool)),this,SLOT(selectItem0()));
    connect(mylabel1,SIGNAL(clicked(bool)),this,SLOT(selectItem1()));
    connect(mylabel2,SIGNAL(clicked(bool)),this,SLOT(selectItem2()));
    connect(mylabel3,SIGNAL(clicked(bool)),this,SLOT(selectItem3()));
    connect(mylabel4,SIGNAL(clicked(bool)),this,SLOT(selectItem4()));
    connect(mylabel5,SIGNAL(clicked(bool)),this,SLOT(selectItem5()));
    connect(mylabel6,SIGNAL(clicked(bool)),this,SLOT(selectItem6()));
    btlist.append(mylabel0);
    btlist.append(mylabel1);
    btlist.append(mylabel2);
    btlist.append(mylabel3);
    btlist.append(mylabel4);
    btlist.append(mylabel5);
    btlist.append(mylabel6);

    QSize size=QSize(this->height()/left_num,0.7*this->height()/left_num);
    QVBoxLayout *LeftLayout = new QVBoxLayout;
    for(int i=0;i<btlist.size();i++)
    {
        Button *bt=btlist.at(i);
        bt->setIconSize(size);
        LeftLayout->addWidget(bt,0,Qt::AlignTop);
    }
    LeftLayout->addStretch();
    LeftLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    LeftLayout->setMargin(5);
    LeftLayout->setSpacing(3);

    QHBoxLayout *mainLayout =new QHBoxLayout(this);
    mainLayout->addLayout(LeftLayout);
    mainLayout->addWidget(stackedWidget,0,Qt::AlignJustify);
    mainLayout->setStretchFactor(LeftLayout,1);   //伸縮時左右分別為1：7拉伸
    mainLayout->setStretchFactor(stackedWidget,left_num-1);
    mainLayout->setMargin(0);          //設定邊距為5
    mainLayout->setSpacing(0);         //設定元件間的距離為5
    selectItem0();

    //HMI2ECU
    connect(control,SIGNAL(modeChanged(int)),setting,SLOT(settingSysMode(int)));
    connect(control->mySpeedSlider,SIGNAL(valueChanged(int)),setting,SLOT(settingCruSpeed(int)));
    connect(control->myHtSlider,SIGNAL(valueChanged(int)),setting,SLOT(settingTimeHeadway(int)));
    connect(map,SIGNAL(getRoadPointNo(int)),setting,SLOT(settingRoadPointNo(int)));
    connect(map,SIGNAL(getRoadPointLon(double)),setting,SLOT(settingRoadPointLon(double)));
    connect(map,SIGNAL(getRoadPointLat(double)),setting,SLOT(settingRoadPointLat(double)));
    connect(map,SIGNAL(getRoadPointAlt(double)),setting,SLOT(settingRoadPointAlt(double)));
    connect(map,SIGNAL(getRoadPointLspeed(int)),setting,SLOT(settingRoadPointLspeed(int)));
    connect(map->next_step_bt,SIGNAL(clicked(bool)),this,SLOT(selectItem1()));
    connect(control->next_step_bt,SIGNAL(clicked(bool)),this,SLOT(selectItem2()));
    //ECU2HMI
    connect(setting, SIGNAL(getSignal_ecu2hmi(ECU2HMI_DATA)),vehicle,SLOT(receive_ecu_data(ECU2HMI_DATA)));
    connect(setting, SIGNAL(getSignal_ecu2hmi(ECU2HMI_DATA)),map,SLOT(receive_ecu_data(ECU2HMI_DATA)));
    connect(setting, SIGNAL(getSignal_ecu2hmi(ECU2HMI_DATA)),traffic,SLOT(receive_ecu_data(ECU2HMI_DATA)));


}

Widget::~Widget()
{

}

void Widget::selectItem(int index)
{
    for(int i=0;i<btlist.size();i++)
    {
        Button *bt=btlist.at(i);
        if(i==index)
            bt->setFlag(2);
        else
            bt->setFlag(0);
    }
}

void Widget::selectItem0()
{
    this->selectItem(0);
    stackedWidget->setCurrentIndex(0);
}

void Widget::selectItem1()
{
    this->selectItem(1);
    stackedWidget->setCurrentIndex(1);
}

void Widget::selectItem2()
{
    this->selectItem(2);
    stackedWidget->setCurrentIndex(2);
}

void Widget::selectItem3()
{
    this->selectItem(3);
    stackedWidget->setCurrentIndex(3);
}

void Widget::selectItem4()
{
    this->selectItem(4);
    stackedWidget->setCurrentIndex(4);
}

void Widget::selectItem5()
{
    this->selectItem(5);
    stackedWidget->setCurrentIndex(5);
}

void Widget::selectItem6()
{
    this->selectItem(6);
    stackedWidget->setCurrentIndex(6);
}

void Widget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.drawImage(this->rect(), QImage(":/res/background0.png"));
    app->setGeometry(this->rect());

}
