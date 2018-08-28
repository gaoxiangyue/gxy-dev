#include "mybutton.h"

Button::Button(QString filepath0,QString filepath1,QString filepath2):
    mouse_flag(0)
{
    path0=filepath0;
    path1=filepath1;
    path2=filepath2;
    this->setCheckable(true);
    this->setIcon(QIcon(path0));
}

Button::~Button()
{

}

void Button::setFlag(int flag)
{
    mouse_flag=flag;
    this->repaint();
}

void Button::enterEvent(QEvent *)
{
    if(mouse_flag==0)
    {
        setFlag(1);
        this->setIcon(QIcon(path1));
    }
}

void Button::leaveEvent(QEvent *)
{
    if(mouse_flag==1)
    {
        setFlag(0);
        this->setIcon(QIcon(path0));
    }
}
/*
void Button::mousePressEvent(QMouseEvent *)
{
    setFlag(2);
}
*/
void Button::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(mouse_flag==0)
        painter.drawImage(this->rect(), QImage(path0));
    else if(mouse_flag==1)
        painter.drawImage(this->rect(), QImage(path1));
    else
        painter.drawImage(this->rect(), QImage(path2));
}
