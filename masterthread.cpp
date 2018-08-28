#include "masterthread.h"

//#include <QtSerialPort/QSerialPort>

#include <QTime>

#include <QDebug>

QT_USE_NAMESPACE

MasterThread::MasterThread(QObject *parent)
    : QThread(parent), waitTimeout(0), quit(false), relink(false)
{
}

//! [0]
MasterThread::~MasterThread()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    //
    if(isRunning())
       terminate();
}
//! [0]

//! [1] //! [2]
void MasterThread::transaction(const QString &portName, int baudRate, int waitTimeout, const QString &request)
{
    if(quit) return;
    //! [1]
    QMutexLocker locker(&mutex);
    if(this->portName!=portName)
    {
        this->portName = portName;
        relink=true;
    }
    if(this->baudRate!=baudRate)
    {
        this->baudRate = baudRate;
        relink=true;
    }
    this->waitTimeout = waitTimeout;
    this->request = request;
    //! [3]
    if (!isRunning())
        start();
    else
        cond.wakeOne();
}
//! [2] //! [3]

//! [4]
void MasterThread::run()
{
    bool currentPortNameChanged = false;

    mutex.lock();
    //! [4] //! [5]

    if (currentPortName != portName) {
        currentPortName = portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = waitTimeout;
    QString currentRequest = request;
    mutex.unlock();
    //! [5] //! [6]
    QSerialPort serial;

    while (!quit) {
        //![6] //! [7]
        if (currentPortNameChanged||relink) {
            serial.close();
            serial.setPortName(currentPortName);
            serial.setBaudRate(baudRate);
            serial.setParity(QSerialPort::NoParity);
            serial.setDataBits(QSerialPort::Data8);
            serial.setStopBits(QSerialPort::OneStop);
            if (!serial.open(QIODevice::ReadWrite)) {
                emit error(tr("Can't open %1, error code %2")
                           .arg(portName).arg(serial.error()));
                return;
            }
            if(relink)
                relink=false;
        }
        //! [7] //! [8]
        // write request
        QByteArray requestData = currentRequest.toLocal8Bit();
        serial.write(requestData);
        if (serial.waitForBytesWritten(waitTimeout)) {
            //! [8] //! [10]
            // read response
            if (serial.waitForReadyRead(currentWaitTimeout)) {//currentWaitTimeout内有反馈
                QByteArray responseData = serial.readAll();
                while (serial.waitForReadyRead(20))//若接收折断，提高等待时间15
                    responseData += serial.readAll();

                QString response(responseData);
                //! [12]
                emit this->response(response);
                //! [10] //! [11] //! [12]
            } else {
                emit timeout(tr("Wait read response timeout %1")
                             .arg(QTime::currentTime().toString()));
            }
            //! [9] //! [11]
        } else {
            emit timeout(tr("Wait write request timeout %1")
                         .arg(QTime::currentTime().toString()));
        }
        //! [9]  //! [13]
        mutex.lock();
        cond.wait(&mutex);
        if (currentPortName != portName) {
            currentPortName = portName;
            currentPortNameChanged = true;
        } else {
            currentPortNameChanged = false;
        }
        currentWaitTimeout = waitTimeout;
        currentRequest = request;
        mutex.unlock();
    }
    //! [13]
    //!
}
