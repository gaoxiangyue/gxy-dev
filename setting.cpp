#include "setting.h"
#include <QPainter>

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QTextEdit>
#include <QtSerialPort/QSerialPortInfo>
#include <QNetworkInterface>

#include <QTimerEvent>

#include <QDebug>

Setting::Setting(QWidget *parent)  :
    QWidget(parent)
  , transactionCount(0)
  , transfailureCount(0)

  , serGroup(new QGroupBox(tr("串口设置")))
  , serialPortLabel(new QLabel(tr("串口号")))
  , serialPortComboBox(new QComboBox())
  , serialBaudLabel(new QLabel(tr("波特率")))
  , serialBaudComboBox(new QComboBox())
  , waitResponseLabel(new QLabel(tr("发送周期")))
  , waitResponseSpinBox(new QSpinBox())
  , runButton(new QPushButton(tr("切换UDP通信")))

  , udpGroup(new QGroupBox(tr("网络设置")))
  , udpPortLabel0(new QLabel(tr("本地IP地址")))
  , udpPortLabel1(new QLabel(tr("目标主机")))
  , udpPortLabel2(new QLabel(tr("本地端口号")))
  , udpPortLabel3(new QLabel(tr("目标端口")))
//  , hostEdit0(new MyIpAddrEdit(this))
  , hostPortComboBox(new QComboBox(this))
  , hostEdit1(new MyIpAddrEdit(this))
  , portEdit0(new QLineEdit(tr("8080")))
  , portEdit1(new QLineEdit(tr("8080")))

  , dispGroup(new QGroupBox(tr("显示区")))
  , requestLabel(new QLabel(tr("待发数据:")))
  , trafficLabel(new QLabel(tr("无应答...")))
  , statusLabel(new QLabel(tr("状态: 未运行.")))
  , normal(RESPON_THRESHOLD)
  , requestLineEdit(new QTextEdit(tr("<SysMode,CruSpd,TimeHw,RoadP[],OtherP/>")))
  , myConnectType(UNKNOWN_Communication)
{
    int HEIGHT=60;
    int WIDTH=160;
    int WSize=20;
    int WSize1=16;

    //init hmi2ecu_data
    hmi2ecu_data.dt0_systemMode=(0);
    hmi2ecu_data.dt1_cruisingSpeed=(36);
    hmi2ecu_data.dt2_timeHeadway=(15);
    hmi2ecu_data.dt3_RoadPointNo=(-1);
    hmi2ecu_data.dt4_RoadPointLon=(125.1234567);
    hmi2ecu_data.dt5_RoadPointLat=(45.1234567);
    hmi2ecu_data.dt6_RoadPointAlt=(233.123);
    hmi2ecu_data.dt7_RoadPointLspeed=(36);
    hmi2ecu_data.dt8_ReservedRet=(100);
    hmi2ecu_data.dt9_ReservedRet=(100);
    hmi2ecu_data.dt10_ReservedRet=(100);
    this->refreshRequestLine();

    QPalette pe;
    pe.setColor(QPalette::WindowText,"#ffffff");
    QPalette pe1;
    pe1.setColor(QPalette::WindowText,"#000000");
    QFont font ( "Microsoft YaHei",WSize, 60);//Size,B
    QFont font1 ( "Microsoft YaHei",WSize1, 60);//Size,B

    //Serial Controls
    serialPortLabel->setPalette(pe);
    serialPortLabel->setFont(font);
    serialPortLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    serialPortComboBox->setMinimumSize(WIDTH,HEIGHT);
    serialPortComboBox->setMaximumSize(WIDTH,HEIGHT);
    auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        serialPortComboBox->addItem(info.portName());
    serialPortComboBox->setCurrentText("COM5");

    serialBaudLabel->setPalette(pe);
    serialBaudLabel->setFont(font);
    serialBaudLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    serialBaudComboBox->setMinimumSize(WIDTH,HEIGHT);
    serialBaudComboBox->setMaximumSize(WIDTH,HEIGHT);
    for(int bd=1;bd<33;bd=2*bd)
        serialBaudComboBox->addItem(QString::number(1200*bd,10));
    serialBaudComboBox->addItem(QString::number(1200*48,10));
    serialBaudComboBox->addItem(QString::number(1200*96,10));
    serialBaudComboBox->setCurrentIndex(7);

    waitResponseLabel->setPalette(pe);
    waitResponseLabel->setFont(font);
    waitResponseLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    waitResponseSpinBox->setMinimumSize(WIDTH,HEIGHT);
    waitResponseSpinBox->setMaximumSize(WIDTH,HEIGHT);
    waitResponseSpinBox->setRange(0, 500);
    waitResponseSpinBox->setValue(100);

    runButton->setMinimumSize(1.8*WIDTH,1.2*HEIGHT);
    runButton->setMaximumSize(1.8*WIDTH,1.2*HEIGHT);

    QGridLayout *serLayout = new QGridLayout;
    serLayout->addWidget(serialPortLabel,0,0);
    serLayout->addWidget(serialPortComboBox,0,1);
    serLayout->addWidget(serialBaudLabel,1,0);
    serLayout->addWidget(serialBaudComboBox,1,1);
    serLayout->addWidget(waitResponseLabel, 2, 0);
    serLayout->addWidget(waitResponseSpinBox, 2, 1);
    serLayout->addWidget(runButton, 3, 0,1,2);

    serGroup->setPalette(pe);
    serGroup->setFont(font);
    serGroup->setLayout(serLayout);

    //Enthenet Controls
    udpPortLabel0->setPalette(pe);
    udpPortLabel0->setFont(font);
    udpPortLabel0->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    udpPortLabel1->setPalette(pe);
    udpPortLabel1->setFont(font);
    udpPortLabel1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    udpPortLabel2->setPalette(pe);
    udpPortLabel2->setFont(font);
    udpPortLabel2->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    udpPortLabel3->setPalette(pe);
    udpPortLabel3->setFont(font);
    udpPortLabel3->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QString local_ip = "127.0.0.1";
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
          if(address.protocol() == QAbstractSocket::IPv4Protocol)
          {
              //IPv4地址
              hostPortComboBox->addItem(address.toString());
//              local_ip = address.toString();
          }
    }

//    hostEdit0->setPalette(pe1);
//    hostEdit0->settext(local_ip);
    hostPortComboBox->setPalette(pe1);
    hostPortComboBox->setMinimumSize(1.5*WIDTH,HEIGHT);
    hostPortComboBox->setMaximumSize(1.5*WIDTH,HEIGHT);
    hostEdit1->setPalette(pe1);
    hostEdit1->settext(hostPortComboBox->currentText());
    QValidator *validator = new QIntValidator(0,99999,this);  //設定輸入範圍0到100
    portEdit0->setMinimumSize(1.5*WIDTH,HEIGHT);
    portEdit0->setMaximumSize(1.5*WIDTH,HEIGHT);
    portEdit0->setValidator(validator);
    portEdit1->setMinimumSize(1.5*WIDTH,HEIGHT);
    portEdit1->setMaximumSize(1.5*WIDTH,HEIGHT);
    portEdit1->setValidator(validator);
    udp.bind(hostPortComboBox->currentText(),portEdit0->text().toInt(Q_NULLPTR,10));

    QGridLayout *udpLayout = new QGridLayout;
    udpLayout->addWidget(udpPortLabel0,0,0);
    udpLayout->addWidget(udpPortLabel1,2,0);
    udpLayout->addWidget(udpPortLabel2,1,0);
    udpLayout->addWidget(udpPortLabel3,3,0);
    udpLayout->addWidget(hostPortComboBox,0,1);
    udpLayout->addWidget(hostEdit1,2,1);
    udpLayout->addWidget(portEdit0,1,1);
    udpLayout->addWidget(portEdit1,3,1);


    udpGroup->setPalette(pe);
    udpGroup->setFont(font);
    udpGroup->setLayout(udpLayout);


    //Public Controls
    requestLabel->setPalette(pe);
    requestLabel->setFont(font1);
    requestLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    requestLineEdit->setMinimumSize(6*WIDTH,HEIGHT);
    requestLineEdit->setMaximumSize(6*WIDTH,HEIGHT);
    requestLineEdit->setFont(font1);

    trafficLabel->setPalette(pe);
    trafficLabel->setFont(font1);
    trafficLabel->setAlignment( Qt::AlignTop);
    trafficLabel->setMaximumWidth(6*WIDTH);

    statusLabel->setPalette(pe);
    statusLabel->setFont(font1);
    statusLabel->setAlignment( Qt::AlignTop);
    statusLabel->setMinimumSize(6*WIDTH,HEIGHT);
    statusLabel->setMaximumSize(6*WIDTH,HEIGHT);

    QGridLayout *dispLayout = new QGridLayout;
    dispLayout->addWidget(requestLabel,0,0);
    dispLayout->addWidget(requestLineEdit,1,0,1,6);
    dispLayout->addWidget(trafficLabel,2,0,5,8);
    dispLayout->addWidget(statusLabel,7,0,3,8);
    dispGroup->setPalette(pe);
    dispGroup->setFont(font);
    dispGroup->setLayout(dispLayout);

    //---
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(serGroup, 0, 0);
    mainLayout->addWidget(udpGroup,0,1,1,2);
    mainLayout->addWidget(dispGroup,1,0,3,8);
    mainLayout->addWidget(new QLabel(""),4,0,3,20);
    mainLayout->setMargin(30);          //設定邊距為10
    mainLayout->setSpacing(10);         //設定元件間的距離為10
    setLayout(mainLayout);

//==========ser================================================
    //connect(runButton, &QPushButton::clicked, this, &transaction);
    connect(runButton, &QPushButton::clicked, this, &refreshPort);
    connect(waitResponseSpinBox,SIGNAL(valueChanged(int)),this,SLOT(timerChanged(int)));
//==========udp================================================
    //connect(&udp, &UdpRxTx::response, this, &showResponse);
    connect(hostPortComboBox, &QComboBox::currentTextChanged,this, &rebindudp);
    connect(portEdit0, &QLineEdit::textChanged,this, &rebindudp);
//==========thread=============================================
    connect(&thread, &MasterThread::response, this, &showResponse);
    connect(&thread, &MasterThread::error, this, &processError);
    connect(&thread, &MasterThread::timeout, this, &processTimeout);
//==========timer==============================================
    m_nTimerID = this->startTimer(waitResponseSpinBox->value());
}

Setting::~Setting()
{
    //killTimer(m_nTimerID);
}

void Setting::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::darkGray);
    painter.drawRect(this->rect());
}

void Setting::rebindudp()
{
    udp.bind(hostPortComboBox->currentText(),portEdit0->text().toInt(Q_NULLPTR,10));
    qDebug()<<"rebind"<<hostPortComboBox->currentText()<<portEdit0->text().toInt(Q_NULLPTR,10);
}

void Setting::refreshPort()
{

    if(thread.quit)
        {
        thread.quit=false;
        runButton->setText("切换UDP通信");

        disconnect(&udp, &UdpRxTx::response, this, &showResponse);
        }
    else
        {
        thread.quit=true;
        runButton->setText("切换串口通信");

        //if(normal)return;
        /*
        setControlsEnabled(true);
        auto infos = QSerialPortInfo::availablePorts();
           serialPortComboBox->clear();
        for (const QSerialPortInfo &info : infos)
            serialPortComboBox->addItem(info.portName());
        */
        connect(&udp, &UdpRxTx::response, this, &showResponse);
        }
}

void Setting::transaction()
{

    if(!thread.quit)
    //---ser
    {
    thread.transaction(serialPortComboBox->currentText(),
                       serialBaudComboBox->currentText().toInt(Q_NULLPTR,10),
                       waitResponseSpinBox->value(),
                       requestLineEdit->toPlainText());
    if(!normal)
    {
        thread.relink=true;
        if(!statusLabel->text().endsWith(tr("尝试连接...")))
            statusLabel->setText(statusLabel->text()+tr("\n\r尝试连接..."));
    }
    else
        statusLabel->setText(tr("状态: 运行中,已连接端口 %1.")
                         .arg(serialPortComboBox->currentText()));
    }
    else
    //---udp
    {
        udp.transaction(hostEdit1->text(),portEdit1->text().toInt(Q_NULLPTR,10),requestLineEdit->toPlainText());
        if(normal>0)normal--;
        if(!normal)
            statusLabel->setText(tr("状态: 运行中,端口无应答."));
        else
            statusLabel->setText(tr("状态: 运行中,已连接端口 %1.")
                             .arg(hostEdit1->text()+":"+portEdit1->text()));
    }
}

void Setting::showResponse(const QString &s)
{
    QString str=s;
    setControlsEnabled(true);
    int sp=str.indexOf("<",0);
    int ep=str.indexOf("/>",sp+1);
    str=str.mid(sp,ep+2-sp);
    trafficLabel->setText(tr("通信正常: 接收计数 #%1"
                             "\n\r-HMI2ECU: \n\r%2"//2text edit
                             "\n\r-ECU2HMI: \n\r%3")
                          .arg(++transactionCount).arg(requestLineEdit->toPlainText()).arg(str));
    if(!str.isEmpty())//exam format
        ecu2hmi_data=this->ParseRString(str);
    {
        emit getSystem_mode(ecu2hmi_data.dt0_systemMode);
        emit getSystem_warning(ecu2hmi_data.dt1_warningRet);
        emit getSteering_Angle(ecu2hmi_data.dt2_SteeringAngle);
        emit getRoadPoint_No(ecu2hmi_data.dt3_RoadPointNo);
        emit getCurrent_Pos(ecu2hmi_data.dt4_CurrentLon,ecu2hmi_data.dt5_CurrentLat);
        emit getCurrent_Alt(ecu2hmi_data.dt6_CurrentAlt);
        emit getVehicle_speed(ecu2hmi_data.dt7_VehicleSpeed);
        emit getTraffic_signal(ecu2hmi_data.dt8_TafficSignal);
        //Lane_Parameters
        emit getSignal_dt9(ecu2hmi_data.dt9_L0_A0_Ret);
        emit getSignal_dt10(ecu2hmi_data.dt10_L0_B0_Ret);
        emit getSignal_dt11(ecu2hmi_data.dt11_L0_C0_Ret);
        emit getSignal_dt12(ecu2hmi_data.dt12_L1_A1_Ret);
        emit getSignal_dt13(ecu2hmi_data.dt13_L1_B1_Ret);
        emit getSignal_dt14(ecu2hmi_data.dt14_L1_C1_Ret);
        emit getSignal_dt15(ecu2hmi_data.dt15_L2_A2_Ret);
        emit getSignal_dt16(ecu2hmi_data.dt16_L2_B2_Ret);
        emit getSignal_dt17(ecu2hmi_data.dt17_L2_C2_Ret);
        emit getSignal_dt18(ecu2hmi_data.dt18_L3_A3_Ret);
        emit getSignal_dt19(ecu2hmi_data.dt19_L3_B3_Ret);
        emit getSignal_dt20(ecu2hmi_data.dt20_L3_C3_Ret);
        //Object_Parameters
        emit getSignal_dt21(ecu2hmi_data.dt21_Obj0_Dis);
        emit getSignal_dt22(ecu2hmi_data.dt22_Obj0_Angle);
        emit getSignal_dt23(ecu2hmi_data.dt23_Obj0_Heading);
        emit getSignal_dt24(ecu2hmi_data.dt24_Obj0_Width);
        emit getSignal_dt25(ecu2hmi_data.dt25_Obj0_Class);
        emit getSignal_dt26(ecu2hmi_data.dt26_Obj1_Dis);
        emit getSignal_dt27(ecu2hmi_data.dt27_Obj1_Angle);
        emit getSignal_dt28(ecu2hmi_data.dt28_Obj1_Heading);
        emit getSignal_dt29(ecu2hmi_data.dt29_Obj1_Width);
        emit getSignal_dt30(ecu2hmi_data.dt30_Obj1_Class);
        emit getSignal_dt31(ecu2hmi_data.dt31_Obj2_Dis);
        emit getSignal_dt32(ecu2hmi_data.dt32_Obj2_Angle);
        emit getSignal_dt33(ecu2hmi_data.dt33_Obj2_Heading);
        emit getSignal_dt34(ecu2hmi_data.dt34_Obj2_Width);
        emit getSignal_dt35(ecu2hmi_data.dt35_Obj2_Class);
        emit getSignal_dt36(ecu2hmi_data.dt36_Obj3_Dis);
        emit getSignal_dt37(ecu2hmi_data.dt37_Obj3_Angle);
        emit getSignal_dt38(ecu2hmi_data.dt38_Obj3_Heading);
        emit getSignal_dt39(ecu2hmi_data.dt39_Obj3_Width);
        emit getSignal_dt40(ecu2hmi_data.dt40_Obj3_Class);
        emit getSignal_dt41(ecu2hmi_data.dt41_Obj4_Dis);
        emit getSignal_dt42(ecu2hmi_data.dt42_Obj4_Angle);
        emit getSignal_dt43(ecu2hmi_data.dt43_Obj4_Heading);
        emit getSignal_dt44(ecu2hmi_data.dt44_Obj4_Width);
        emit getSignal_dt45(ecu2hmi_data.dt45_Obj4_Class);
        emit getSignal_dt46(ecu2hmi_data.dt46_ReservedRet);
    }
    normal=RESPON_THRESHOLD;
}

void Setting::processError(const QString &s)
{
    setControlsEnabled(true);
    statusLabel->setText(tr("状态: 错误, %1.").arg(s));
    trafficLabel->setText(tr("无响应."));

    normal=0;
}

void Setting::processTimeout(const QString &s)
{
    setControlsEnabled(true);
    statusLabel->setText(tr("状态: 超时, %1.").arg(s));
    trafficLabel->setText(tr("无响应."));
    //Test_Traffic_signal1 emit getTraffic_signal(tr("+000003").toInt(Q_NULLPTR,10));
    if(normal>0)normal--;
}

void Setting::setControlsEnabled(bool enable)
{
    serialPortComboBox->setEnabled(enable);
    serialBaudComboBox->setEnabled(enable);
    waitResponseSpinBox->setEnabled(enable);
    requestLineEdit->setEnabled(enable);
}

void Setting::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_nTimerID){
        handleTimeout();
    }
}

void Setting::timerChanged(int t)
{
    killTimer(m_nTimerID);
    m_nTimerID=this->startTimer(t);
}

void Setting::handleTimeout()
{
    this->transaction();
}

void Setting::settingSysMode(int dt0)
{
    hmi2ecu_data.dt0_systemMode=dt0;
    this->refreshRequestLine();
}

void Setting::settingCruSpeed(int dt1)
{
    hmi2ecu_data.dt1_cruisingSpeed=dt1;
    this->refreshRequestLine();
}

void Setting::settingTimeHeadway(int dt2)
{
    hmi2ecu_data.dt2_timeHeadway=dt2;
    this->refreshRequestLine();
}

void Setting::settingRoadPointNo(int dt3)
{
    hmi2ecu_data.dt3_RoadPointNo=dt3;
    this->refreshRequestLine();
}

void Setting::settingRoadPointLon(double dt4)
{
    hmi2ecu_data.dt4_RoadPointLon=dt4;
    this->refreshRequestLine();
}

void Setting::settingRoadPointLat(double dt5)
{
    hmi2ecu_data.dt5_RoadPointLat=dt5;
    this->refreshRequestLine();
}

void Setting::settingRoadPointAlt(double dt6)
{
    hmi2ecu_data.dt6_RoadPointAlt=dt6;
    this->refreshRequestLine();
}

void Setting::settingRoadPointLspeed(int dt7)
{
    hmi2ecu_data.dt7_RoadPointLspeed=dt7;
    this->refreshRequestLine();
}

ECU2HMI_DATA Setting::ParseRString(QString s)
{
    ECU2HMI_DATA data;
    int sp=0;
    int ep=0;
    sp=s.indexOf("<",sp);
    ep=s.indexOf(",",sp+1);
    data.dt0_systemMode=s.mid(sp+1,ep-sp-1).toInt(Q_NULLPTR,10);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt1_warningRet=s.mid(sp+1,ep-sp-1).toInt(Q_NULLPTR,10);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt2_SteeringAngle=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt3_RoadPointNo=s.mid(sp+1,ep-sp-1).toInt(Q_NULLPTR,10);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt4_CurrentLon=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt5_CurrentLat=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt6_CurrentAlt=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt7_VehicleSpeed=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt8_TafficSignal=s.mid(sp+1,ep-sp-1).toInt(Q_NULLPTR,10);
    //Lane_Parameters
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt9_L0_A0_Ret=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt10_L0_B0_Ret=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt11_L0_C0_Ret=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt12_L1_A1_Ret=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt13_L1_B1_Ret=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt14_L1_C1_Ret=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt15_L2_A2_Ret=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt16_L2_B2_Ret=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt17_L2_C2_Ret=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt18_L3_A3_Ret=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt19_L3_B3_Ret=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt20_L3_C3_Ret=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    //Object_Parameters
    //0
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt21_Obj0_Dis=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt22_Obj0_Angle=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt23_Obj0_Heading=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt24_Obj0_Width=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt25_Obj0_Class=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt26_Obj1_Dis=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt27_Obj1_Angle=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt28_Obj1_Heading=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt29_Obj1_Width=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt30_Obj1_Class=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt31_Obj2_Dis=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt32_Obj2_Angle=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt33_Obj2_Heading=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt34_Obj2_Width=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt35_Obj2_Class=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt36_Obj3_Dis=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt37_Obj3_Angle=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt38_Obj3_Heading=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt39_Obj3_Width=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt40_Obj3_Class=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt41_Obj4_Dis=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt42_Obj4_Angle=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt43_Obj4_Heading=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt44_Obj4_Width=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);
    sp=ep;
    ep=s.indexOf(",",sp+1);
    data.dt45_Obj4_Class=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);

    //...
    sp=ep;
    ep=s.indexOf("/",sp+1);
    data.dt46_ReservedRet=s.mid(sp+1,ep-sp-1).toDouble(Q_NULLPTR);

/*
    qDebug()<<"Parse_Test0~4 8~10"<<data.dt0_systemMode<<""
           <<data.dt1_warningRet<<""
           <<data.dt2_SteeringAngle<<" "
           <<data.dt3_RoadPointNo<<" "
           <<data.dt4_CurrentLon<<" "
           <<data.dt8_TafficSignal<<" "
           <<data.dt9_L0_A0_Ret<<" "
         <<data.dt10_L0_B0_Ret;
    qDebug()<<"Parse_Test41~46"<<data.dt41_Obj4_Dis<<""
           <<data.dt42_Obj4_Angle<<""
           <<data.dt43_Obj4_Heading<<" "
           <<data.dt44_Obj4_Width<<" "
          <<data.dt45_Obj4_Class<<" "
         <<data.dt46_ReservedRet;
*/
    //Test_Traffic_signal0
    //qDebug()<<"Parse|dt8_TafficSignal"<<data.dt8_TafficSignal;
    return data;
}

void Setting::refreshRequestLine()
{
    //qDebug()<<"requestLineEdit Refresh...";
    QString str=tr("<%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11/>")
            .arg(hmi2ecu_data.dt0_systemMode)
            .arg(hmi2ecu_data.dt1_cruisingSpeed)
            .arg(hmi2ecu_data.dt2_timeHeadway)
            .arg(hmi2ecu_data.dt3_RoadPointNo)
            .arg(QString::number(hmi2ecu_data.dt4_RoadPointLon,10,7))
            .arg(QString::number(hmi2ecu_data.dt5_RoadPointLat,10,7))
            .arg(QString::number(hmi2ecu_data.dt6_RoadPointAlt,10,3))
            .arg(hmi2ecu_data.dt7_RoadPointLspeed)
            .arg(hmi2ecu_data.dt8_ReservedRet)
            .arg(hmi2ecu_data.dt9_ReservedRet)
            .arg(hmi2ecu_data.dt10_ReservedRet);
    if(requestLineEdit->toPlainText()!=str)
    requestLineEdit->setText(str);
}
