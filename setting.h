#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include "masterthread.h"
#include "udprxtx.h"
#include "myipaddredit.h"
#include <QGroupBox>
#include "DataStruct/datastruct.h"

#define RESPON_THRESHOLD 5      //无应答五次重连

class QLabel;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QComboBox;
class QTextEdit;

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
    void getSignal_ecu2hmi(ECU2HMI_DATA data);

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
