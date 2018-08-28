#include "app.h"

App::App(QWidget *parent) : QWidget(parent)
{/*
    appbkg=new QMovie(":/res/homeMovie.gif");
    appLabel = new QLabel(this);
    appLabel->setMovie(appbkg);
    appbkg->setSpeed(80);
    appbkg->start();
    this->repaint();
    */
}

void App::paintEvent(QPaintEvent *){
    //QPainter painter(this);
    //painter.drawImage(this->rect(), QImage(":/res/background0.png"));
    //appLabel->resize(this->size());
    //appbkg->setScaledSize(this->size());
}
