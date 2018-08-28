#include "widget.h"
#include <QPainter>

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>

#include <QtSerialPort/QSerialPortInfo>

#include <QTimerEvent>

#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
  , transactionCount(0)
  , serialPortLabel(new QLabel(tr("Serial port:")))
  , serialPortComboBox(new QComboBox())
  , waitResponseLabel(new QLabel(tr("Wait response, msec:")))
  , waitResponseSpinBox(new QSpinBox())
  , requestLabel(new QLabel(tr("Request:")))
  , requestLineEdit(new QLineEdit(tr("<123456789011121314151617181920/>")))
  , trafficLabel(new QLabel(tr("No traffic.")))
  , statusLabel(new QLabel(tr("Status: Not running.")))
  , runButton(new QPushButton(tr("Start")))
  , normal(RESPON_THRESHOLD)

{
    auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        serialPortComboBox->addItem(info.portName());
    waitResponseSpinBox->setRange(0, 100);
    waitResponseSpinBox->setValue(50);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(serialPortLabel, 0, 0);
    mainLayout->addWidget(serialPortComboBox, 0, 1);
    mainLayout->addWidget(waitResponseLabel, 1, 0);
    mainLayout->addWidget(waitResponseSpinBox, 1, 1);
    mainLayout->addWidget(runButton, 0, 2, 2, 1);
    mainLayout->addWidget(requestLabel, 2, 0);
    mainLayout->addWidget(requestLineEdit, 2, 1, 1, 3);
    mainLayout->addWidget(trafficLabel, 3, 0, 1, 4);
    mainLayout->addWidget(statusLabel, 4, 0, 1, 5);
    setLayout(mainLayout);

    setWindowTitle(tr("SerialCI Master"));
    resize(500,300);
    serialPortComboBox->setFocus();

    connect(runButton, &QPushButton::clicked, this, &transaction);
    connect(&thread, &MasterThread::response, this, &showResponse);
    connect(&thread, &MasterThread::error, this, &processError);
    connect(&thread, &MasterThread::timeout, this, &processTimeout);

    m_nTimerID = this->startTimer(TIMER_TIMEOUT);
}

Widget::~Widget()
{
    //killTimer(m_nTimerID);
}

void Widget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setBrush(Qt::magenta);
    painter.drawRect(this->rect());
}

void Widget::transaction()
{
    if(!normal)
    {
        thread.relink=true;
        statusLabel->setText(tr("Status: Relinking to port %1...")
                             .arg(serialPortComboBox->currentText()));
    }
    else
    statusLabel->setText(tr("Status: Running, connected to port %1.")
                         .arg(serialPortComboBox->currentText()));
    thread.transaction(serialPortComboBox->currentText(),
                       waitResponseSpinBox->value(),
                       requestLineEdit->text());
}

void Widget::showResponse(const QString &s)
{
    setControlsEnabled(true);
    trafficLabel->setText(tr("Traffic, transaction #%1:"
                             "\n\r-request: %2"
                             "\n\r-response: %3")
                          .arg(++transactionCount).arg(requestLineEdit->text()).arg(s));
    normal=RESPON_THRESHOLD;
}

void Widget::processError(const QString &s)
{
    setControlsEnabled(true);
    statusLabel->setText(tr("Status: Not running, %1.").arg(s));
    trafficLabel->setText(tr("No traffic."));

    normal=0;
}

void Widget::processTimeout(const QString &s)
{
    setControlsEnabled(true);
    statusLabel->setText(tr("Status: Running, %1.").arg(s));
    trafficLabel->setText(tr("No traffic."));

    if(normal>0)normal--;
}

void Widget::setControlsEnabled(bool enable)
{
    runButton->setEnabled(enable);
    serialPortComboBox->setEnabled(enable);
    waitResponseSpinBox->setEnabled(enable);
    requestLineEdit->setEnabled(enable);
}

void Widget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_nTimerID){
        handleTimeout();
    }
}

void Widget::handleTimeout()
{
    this->transaction();
}
