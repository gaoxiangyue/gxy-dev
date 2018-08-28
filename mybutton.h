#ifndef LEFTITEMS_H
#define LEFTITEMS_H
#include <QToolButton>
#include <QObject>
#include <QPainter>
#include <QDebug>

class Button: public QToolButton
{
public:
    Button(QString filepath0,QString filepath1,QString filepath2);
    ~Button();
    void setFlag(int flag);
protected:
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    //void mousePressEvent(QMouseEvent *) ;
private:
    int mouse_flag;
    QString path0,path1,path2;
};

#endif // LEFTITEMS_H
