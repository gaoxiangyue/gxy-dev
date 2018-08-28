#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>

//
#include <QPainter>
#include <QBitmap>
#include <QImage>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QRadioButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QSlider>
#include <QProgressBar>
#include <QGridLayout>
#include <QListWidget>
#include <QStackedWidget>
#include <Qsplitter>

//
#include "myswitch.h"
class ControlWidget: public QWidget
{
    Q_OBJECT

public:
     ControlWidget(QWidget *parent=0);

private:
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLabel *speed;
    QLabel *ht;
    QLabel *next_step_lb;
    int WIDTH;
    int HEIGHT;
    //Button
    MySwitch *auto_button;
    //Layout
    QGridLayout *controlLayout;
public:
    //Button
    MySwitch *lat_button;
    MySwitch *lon_button;
    QPushButton*next_step_bt;
    //Slider
    QSlider *mySpeedSlider;
    QSlider *myHtSlider;

private slots:
    void changeAutoButton(bool on);
    void changeAutoMode();
    void changeSpeedSlider(int newvalue);
    void changeHtSlider(int newvalue);
signals:
    void modeChanged(int mode);

protected:
    void paintEvent(QPaintEvent *);
};

#endif // CONTROLWIDGET_H
