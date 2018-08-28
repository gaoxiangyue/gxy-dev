#ifndef APP_H
#define APP_H

#include <QWidget>
//#include <QMovie>
#include <QPainter>
//#include <QLabel>

class App : public QWidget
{
    Q_OBJECT
public:
    explicit App(QWidget *parent = 0);
private:
    //QLabel *appLabel;
    //QMovie *appbkg;
protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:
};

#endif // APP_H
