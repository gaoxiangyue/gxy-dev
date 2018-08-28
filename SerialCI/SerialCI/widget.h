#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "masterthread.h"

#define TIMER_TIMEOUT	(100)   //通信周期ms
#define RESPON_THRESHOLD 5      //无应答五次重连

class QLabel;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QComboBox;


class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void  handleTimeout();
    virtual void timerEvent( QTimerEvent *event);
protected:
    void paintEvent(QPaintEvent *);
private slots:
    void transaction();
    void showResponse(const QString &s);
    void processError(const QString &s);
    void processTimeout(const QString &s);
private:
    void setControlsEnabled(bool enable);
private:
    MasterThread thread;
    int transactionCount;
    int m_nTimerID;
    int normal;

    QLabel *serialPortLabel;
    QComboBox *serialPortComboBox; //串口编号
    QLabel *waitResponseLabel;
    QSpinBox *waitResponseSpinBox; //接收等候时间[<TIMER_TIMEOUT]
    QLabel *requestLabel;
    QLineEdit *requestLineEdit;    //发送字符串
    QLabel *trafficLabel;
    QLabel *statusLabel;
    QPushButton *runButton;
};

#endif // WIDGET_H
