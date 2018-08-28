#include "udprxtx.h"
#include "qDebug"

UdpRxTx::UdpRxTx(QObject *parent) : QObject(parent)
{
    uSocket = new QUdpSocket;
    uSocket->bind(QHostAddress(this->address), this->PORT);
    connect(uSocket, SIGNAL(readyRead()), this, SLOT(receive()));
}

UdpRxTx::~UdpRxTx()
{
    delete uSocket;
}

bool UdpRxTx::bind(QString address,quint16 port)
{
    delete uSocket;
    uSocket = new QUdpSocket;
    connect(uSocket, SIGNAL(readyRead()), this, SLOT(receive()));
    return uSocket->bind(QHostAddress(address), port);
}

void UdpRxTx::transaction(const QString &address, quint16 PORT, const QString &request)
{
    QByteArray msg=request.toLatin1();
    uSocket->writeDatagram(msg, QHostAddress(address), PORT);
}

void UdpRxTx::receive()
{
    QByteArray ba;
    while(uSocket->hasPendingDatagrams())
    {
        ba.resize(uSocket->pendingDatagramSize());
        uSocket->readDatagram(ba.data(), ba.size());

        if(ba.size()==0)break;
        emit response(QString(ba));
        //qDebug()<<"UDP_REC:"<<QString(ba);
    }
}
