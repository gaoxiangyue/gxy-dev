#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include "masterthread.h"
#include "udprxtx.h"
#include "myipaddredit.h"
#include <QGroupBox>

#define RESPON_THRESHOLD 5      //无应答五次重连

class QLabel;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QComboBox;
class QTextEdit;

typedef struct HMI2ECU_DATA{
        int dt0_systemMode;
        int dt1_cruisingSpeed;
        int dt2_timeHeadway;
        int dt3_RoadPointNo;
        double dt4_RoadPointLon;
        double dt5_RoadPointLat;
        double dt6_RoadPointAlt;
        int dt7_RoadPointLspeed;
        int dt8_ReservedRet;
        int dt9_ReservedRet;
        int dt10_ReservedRet;
    } HMI2ECU_DATA;

typedef struct ECU2HMI_DATA{
        int dt0_systemMode;
        int dt1_warningRet;
        double dt2_SteeringAngle;
        int dt3_RoadPointNo;
        double dt4_CurrentLon;
        double dt5_CurrentLat;
        double dt6_CurrentAlt;
        double dt7_VehicleSpeed;
        int dt8_TafficSignal;
        //Lane_Parameters
        double dt9_L0_A0_Ret;
        double dt10_L0_B0_Ret;
        double dt11_L0_C0_Ret;
        double dt12_L1_A1_Ret;
        double dt13_L1_B1_Ret;
        double dt14_L1_C1_Ret;
        double dt15_L2_A2_Ret;
        double dt16_L2_B2_Ret;
        double dt17_L2_C2_Ret;
        double dt18_L3_A3_Ret;
        double dt19_L3_B3_Ret;
        double dt20_L3_C3_Ret;
        //Object_Parameters
        double dt21_Obj0_Dis;
        double dt22_Obj0_Angle;
        double dt23_Obj0_Heading;
        double dt24_Obj0_Width;
        double dt25_Obj0_Class;
        double dt26_Obj1_Dis;
        double dt27_Obj1_Angle;
        double dt28_Obj1_Heading;
        double dt29_Obj1_Width;
        double dt30_Obj1_Class;
        double dt31_Obj2_Dis;
        double dt32_Obj2_Angle;
        double dt33_Obj2_Heading;
        double dt34_Obj2_Width;
        double dt35_Obj2_Class;
        double dt36_Obj3_Dis;
        double dt37_Obj3_Angle;
        double dt38_Obj3_Heading;
        double dt39_Obj3_Width;
        double dt40_Obj3_Class;
        double dt41_Obj4_Dis;
        double dt42_Obj4_Angle;
        double dt43_Obj4_Heading;
        double dt44_Obj4_Width;
        double dt45_Obj4_Class;

        double dt46_ReservedRet;
    } ECU2HMI_DATA;

enum ConnectType{UNKNOWN_Communication,SER_Communication,UDP_Communication};

class Setting : public QWidget
{
    Q_OBJECT
public:
    explicit Setting(QWidget *parent = 0);
    ~Setting();

    void  handleTimeout();
    virtual void timerEvent( QTimerEvent *event);
protected:
    void paintEvent(QPaintEvent *);
signals:
    void getSystem_mode(int dt0);
    void getSystem_warning(int dt1);
    void getSteering_Angle(double dt2);
    void getRoadPoint_No(int dt3);
    void getCurrent_Pos(double dt4,double dt5);
    void getCurrent_Alt(double dt6);
    void getVehicle_speed(double dt7);
    void getTraffic_signal(int dt8);
    void getSignal_dt9(double);
    void getSignal_dt10(double);
    void getSignal_dt11(double);
    void getSignal_dt12(double);
    void getSignal_dt13(double);
    void getSignal_dt14(double);
    void getSignal_dt15(double);
    void getSignal_dt16(double);
    void getSignal_dt17(double);
    void getSignal_dt18(double);
    void getSignal_dt19(double);
    void getSignal_dt20(double);
    void getSignal_dt21(double);
    void getSignal_dt22(double);
    void getSignal_dt23(double);
    void getSignal_dt24(double);
    void getSignal_dt25(double);
    void getSignal_dt26(double);
    void getSignal_dt27(double);
    void getSignal_dt28(double);
    void getSignal_dt29(double);
    void getSignal_dt30(double);
    void getSignal_dt31(double);
    void getSignal_dt32(double);
    void getSignal_dt33(double);
    void getSignal_dt34(double);
    void getSignal_dt35(double);
    void getSignal_dt36(double);
    void getSignal_dt37(double);
    void getSignal_dt38(double);
    void getSignal_dt39(double);
    void getSignal_dt40(double);
    void getSignal_dt41(double);
    void getSignal_dt42(double);
    void getSignal_dt43(double);
    void getSignal_dt44(double);
    void getSignal_dt45(double);
    void getSignal_dt46(double);

private slots:
    void transaction();
    void showResponse(const QString &s);
    void processError(const QString &s);
    void processTimeout(const QString &s);
    void refreshPort();
    void timerChanged(int t);
    void rebindudp();
public slots:
    void settingSysMode(int dt0);
    void settingCruSpeed(int dt1);
    void settingTimeHeadway(int dt2);
    void settingRoadPointNo(int dt3);
    void settingRoadPointLon(double dt4);
    void settingRoadPointLat(double dt5);
    void settingRoadPointAlt(double dt6);
    void settingRoadPointLspeed(int dt7);


private:
    ConnectType myConnectType;
    void setControlsEnabled(bool enable);
    ECU2HMI_DATA ParseRString(QString s);
    void refreshRequestLine();

private:
    MasterThread thread;
    UdpRxTx udp;
    int transactionCount;
    int transfailureCount;
    int m_nTimerID;
    int normal;
    HMI2ECU_DATA hmi2ecu_data;
    ECU2HMI_DATA ecu2hmi_data;

    //串口控件-对标[野人.串口调试助手V4.2 http://www.cmsoft.cn]
    QGroupBox *serGroup;
    QLabel *serialPortLabel;
    QComboBox *serialPortComboBox; //串口号
    QLabel *serialBaudLabel;
    QComboBox *serialBaudComboBox; //波特率
    QLabel *waitResponseLabel;
    QSpinBox *waitResponseSpinBox; //应答周期

    //网络控件-对标[野人.网络调试助手V4.2 http://www.cmsoft.cn]
    QGroupBox *udpGroup;
    QLabel *udpPortLabel0;
    QLabel *udpPortLabel1;
    QLabel *udpPortLabel2;
    QLabel *udpPortLabel3;
//    MyIpAddrEdit  *hostEdit0;
    QComboBox *hostPortComboBox;
    MyIpAddrEdit  *hostEdit1;
    QLineEdit *portEdit0;
    QLineEdit *portEdit1;

    //
    QGroupBox *dispGroup;
    QLabel *requestLabel;
    QTextEdit *requestLineEdit;    //发送字符串
    QLabel *trafficLabel;
    QLabel *statusLabel;
    QPushButton *runButton;
};

#endif // SETTING_H
