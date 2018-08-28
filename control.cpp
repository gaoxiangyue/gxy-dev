#include "control.h"
#include<QDebug>

ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent)
{
    WIDTH=80;
    HEIGHT=80;
    int WdSize=20;
    int WdSize1=12;
    //QLabel
    QPalette pe;
    pe.setColor(QPalette::WindowText,"#ffffff");
    QFont font ( "Microsoft YaHei",WdSize, 60);//Size,B
    QFont font1 ( "Microsoft YaHei",WdSize1, 60);//Size,B
    label1 = new QLabel(this);
    label1->setMinimumSize(3*HEIGHT,HEIGHT);
    label1->setMaximumSize(4*HEIGHT,1.25*HEIGHT);
    label1->setPalette(pe);
    label1->setText(tr("自动模式:"));
    label1->setAlignment(Qt::AlignCenter|Qt::AlignRight);
    label1->setFont(font);
    label2 = new QLabel(this);
    label2->setPalette(pe);
    label2->setText(tr("巡航速度:"));
    label2->setAlignment(Qt::AlignCenter|Qt::AlignRight);
    label2->setFont(font);
    label3 = new QLabel(this);
    label3->setPalette(pe);
    label3->setText(tr("跟车时距:"));
    label3->setAlignment(Qt::AlignCenter|Qt::AlignRight);
    label3->setFont(font);
    speed=new QLabel(this);
    speed->setPalette(pe);
    speed->setAlignment(Qt::AlignLeft | Qt::AlignVCenter );
    speed->setFont(font);
    ht=new QLabel(this);
    ht->setPalette(pe);
    ht->setAlignment(Qt::AlignLeft | Qt::AlignVCenter );
    ht->setFont(font);

    //SwitchButton
    auto_button=new MySwitch(this);
    auto_button->setMinimumSize(3*HEIGHT,HEIGHT);
    auto_button->setMaximumSize(4*HEIGHT,1.25*HEIGHT);
    auto_button->setImage(":/res/swithch_off1.png",":/res/swithch_on1.png");
    auto_button->setButtonStyle(MySwitch::ButtonStyle_Image);
    auto_button->setBgColor(QColor(87, 97, 106),QColor(87, 97, 106));
    //auto_button->setButtonStyle(MySwitch::ButtonStyle_Rect);
    //auto_button->setText(tr("Manu"),tr("Auto"));
    auto_button->setChecked(false);
    auto_button->setFont(font);
    lat_button=new MySwitch(this);
    lat_button->setMinimumSize(2*HEIGHT,0.75*HEIGHT);
    lat_button->setMaximumSize(2*HEIGHT,0.75*HEIGHT);
    lat_button->setButtonStyle(MySwitch::ButtonStyle_Rect);
    lat_button->setText(tr("Lat_OFF"),tr("Lat_ON"));
    lat_button->setFont(font1);
    lat_button->setEnabled(true);
    lon_button=new MySwitch(this);
    lon_button->setMinimumSize(2*HEIGHT,0.75*HEIGHT);
    lon_button->setMaximumSize(2*HEIGHT,0.75*HEIGHT);
    lon_button->setButtonStyle(MySwitch::ButtonStyle_Rect);
    lon_button->setText(tr("Lon_OFF"),tr("Lon_ON"));
    lon_button->setEnabled(true);
    lon_button->setFont(font1);
    lat_button->setHidden(true);
    lon_button->setHidden(true);
    //QSlider
    mySpeedSlider = new QSlider(Qt::Horizontal,this);
    mySpeedSlider->setMinimumSize(2*HEIGHT,0.75*HEIGHT);
    mySpeedSlider->setStyleSheet("  \
                                 QSlider::add-page:Horizontal\
                                 {     \
                                    background-color: rgb(87, 97, 106);\
                                    height:4px;\
                                 }\
                                 QSlider::sub-page:Horizontal \
                                {\
                                    background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(7,208,255, 255), stop:1 rgba(231,80,229, 255));\
                                    height:4px;\
                                 }\
                                QSlider::groove:Horizontal \
                                {\
                                    background:transparent;\
                                    height:20px;\
                                }\
                                QSlider::handle:Horizontal \
                                {\
                                    height: 60px;\
                                    width:120px;\
                                    border-image: url(:/res/carup_g1.fw.png);\
                                    margin: -24 0px; \
                                }\
                                ");
                                 //groove 滑条属性
                                 //handle 滑块属性

    mySpeedSlider->setRange(0, 120);
    mySpeedSlider->setValue(36);
    mySpeedSlider->setPageStep(1);
    mySpeedSlider->setEnabled(true);

    speed->setText(QString::number(mySpeedSlider->value(),10,0)+tr(" (km/h)"));
    connect(mySpeedSlider,SIGNAL(valueChanged(int)),this,SLOT(changeSpeedSlider(int)));

    myHtSlider = new QSlider(Qt::Horizontal,this);
    myHtSlider->setMinimumHeight(0.75*HEIGHT);
    myHtSlider->setRange(0, 50);
    myHtSlider->setPageStep(1);
    myHtSlider->setValue(15);
    myHtSlider->setEnabled(true);
    myHtSlider->setStyleSheet(mySpeedSlider->styleSheet());

    ht->setText(QString::number(double(myHtSlider->value())/10,10,1)+tr(" (s/Veh)"));
    connect(myHtSlider,SIGNAL(valueChanged(int)),this,SLOT(changeHtSlider(int)));

    connect(auto_button,SIGNAL(checkedChanged(bool)),this,SLOT(changeAutoButton(bool)));
    connect(auto_button,&MySwitch::checkedChanged,this,&changeAutoMode);
    connect(lon_button,&MySwitch::checkedChanged,this,&changeAutoMode);
    connect(lat_button,&MySwitch::checkedChanged,this,&changeAutoMode);

    //connect(auto_button,SIGNAL(checkedChanged(bool)),lon_button,SLOT(setEnabled(bool)));
    //connect(auto_button,SIGNAL(checkedChanged(bool)),lat_button,SLOT(setEnabled(bool)));
    //connect(lon_button,SIGNAL(checkedChanged(bool)),mySpeedSlider,SLOT(setEnabled(bool)));
    //connect(lat_button,SIGNAL(checkedChanged(bool)),myHtSlider,SLOT(setEnabled(bool)));

    //Layout
    controlLayout = new QGridLayout(this);
    int i=1;
    //1
    controlLayout->addWidget(label2,i,0);
    controlLayout->addWidget(mySpeedSlider,i,1,1,3);
    controlLayout->addWidget(speed,i,4);
    //
    i++;
    controlLayout->addWidget(label3,i,0);
    controlLayout->addWidget(myHtSlider,i,1,1,3);
    controlLayout->addWidget(ht,i,4);
    //
    i++;
    controlLayout->addWidget(label1,i,0);
    controlLayout->addWidget(auto_button,i,1);
    //
    //i++;
    controlLayout->addWidget(lon_button,i,2);
    controlLayout->addWidget(lat_button,i,3);
    //last
    i++;
    controlLayout->addWidget(new QLabel("<h2><font color=white></font></h2>"),i,0,13-i,4);
    //
    controlLayout->setMargin(40);          //設定邊距為30
    controlLayout->setSpacing(30);         //設定元件間的距離為10

    //next
    next_step_bt=(new QPushButton(this));
    next_step_bt->setEnabled(true);
    next_step_bt->setIcon(QIcon(":/res/nextstep.png"));
    next_step_bt->setIconSize(QSize(1.2*WIDTH,1.2*HEIGHT));
    next_step_bt->setFlat(true);
    //next_step_bt->setHidden(true);
    //next_step_bt->setStyleSheet("border: 0px");//消除边框，取消点击效果
    next_step_lb=(new QLabel(this));
    next_step_lb->setText("下一步");
    next_step_lb->setFont(font);
    next_step_lb->setPalette(pe);
    next_step_lb->setEnabled(true);
    //next_step_lb->setHidden(true);
}

void ControlWidget::paintEvent(QPaintEvent *)
{
    next_step_bt->setGeometry(10,this->height()-HEIGHT-20,1.2*WIDTH,1.2*HEIGHT);
    next_step_lb->setGeometry(10+1.2*WIDTH,this->height()-HEIGHT-20,1.2*WIDTH,1.2*HEIGHT);
    QRect rect=this->rect();
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(rect);
}

void ControlWidget::changeAutoButton(bool on)
{
    if(!on)
    {
        lon_button->setChecked(false);
        lat_button->setChecked(false);
    }
    else
    {
        lon_button->setChecked(true);
        lat_button->setChecked(true);
    }

}

void ControlWidget::changeAutoMode()
{
    //qDebug()<<"changeAutoMode"<<endl;
    int mode=0;
    if(lon_button->getChecked())mode=mode+1;
    if(lat_button->getChecked())mode=mode+2;
    if(mode==0)
        auto_button->setChecked(false);
    else if(mode==1)
    {
        auto_button->setChecked(true);
        lat_button->setChecked(false);
    }
    else if(mode==2)
    {
        auto_button->setChecked(true);
        lon_button->setChecked(false);
    }
    else
        auto_button->setChecked(true);
    emit modeChanged(mode);
    update();
}

void ControlWidget::changeSpeedSlider(int newvalue)
{
    speed->setText(QString::number(newvalue,10,0)+tr(" (km/h)"));
}

void ControlWidget::changeHtSlider(int newvalue)
{
    ht->setText(QString::number(double(newvalue)/10,10,1)+tr(" (s/Veh)"));
}
