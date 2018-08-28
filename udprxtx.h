#ifndef UDPRXTX_H
#define UDPRXTX_H

#include <QObject>
#include <QUdpSocket>//pro 文件里要加一句：QT += network，接着一定要 执行qmake
#include <QByteArray>
#include <QHostAddress>

class UdpRxTx : public QObject
{
    Q_OBJECT
public:
    explicit UdpRxTx(QObject *parent = 0);
    ~UdpRxTx();
    bool bind(QString address,quint16 port);
    void transaction(const QString &address, quint16 PORT, const QString &request);
private:
    QUdpSocket *uSocket;
    const quint16 PORT = 8080;
    const QString address=tr("192.168.1.1");
signals:
    void response(const QString &s);
    //void error(const QString &s);
    //void timeout(const QString &s);
public slots:
    void receive();
};

#endif // UDPRXTX_H
