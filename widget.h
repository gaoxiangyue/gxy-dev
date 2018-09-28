#ifndef WIDGET_H
#define WIDGET_H

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
#include <QList>
#include<QMovie>


//
#include "mybutton.h"
#include "app.h"
#include "setting.h"
#include "vehicle.h"
#include "control.h"
#include "traffic.h"
#include "mapwidget.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    virtual void timerEvent( QTimerEvent *event);

private:
    //
    App *app;
    QLabel *home;
    Setting *setting;
    MapWidget *map;
    Vehicle *vehicle;
    ControlWidget *control;
    Traffic *traffic;
    QHBoxLayout *mainLayout;
    QStackedWidget *stackedWidget;
    QList < Button *> btlist;
    int left_num;
    int m_nTimerID;

private slots:
    void selectItem0();
    void selectItem1();
    void selectItem2();
    void selectItem3();
    void selectItem4();
    void selectItem5();
    void selectItem6();
private:
    void selectItem(int index);
protected:
    void paintEvent(QPaintEvent *);
};

#endif // WIDGET_H
