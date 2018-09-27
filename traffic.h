#ifndef TRAFFIC_H
#define TRAFFIC_H

#include <QWidget>
//
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QMouseEvent>
#include "DataStruct/datastruct.h"

class Traffic : public QWidget
{
    Q_OBJECT
public:
    explicit Traffic(QWidget *parent = 0);
private:
    QLabel *label_light;
    qreal scale_light;
    qreal w_car,h_car;
    int x_range;//x方向探测范围
    qreal scale_x,scale_y;//车辆坐标系x/y方向1m对应像素数量
    qreal persp_rt;//透视程度
    QPoint presspos;
    QPoint moveingpos;
    //cognition parameters
    double a[4],b[4],c[4];//4车道线方程参数
    double obj_x[5],obj_y[5],obj_ang[5],obj_width[5],obj_class[5];//5障碍物参数

    QPoint vhcXY2winXY(double vhcX,double vhcY);


protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event) ;
    void mouseMoveEvent(QMouseEvent *event) ;

signals:

public slots:
    void receive_ecu_data(ECU2HMI_DATA data);
private:
    void showNRYG_light(int nrgy);
};

#endif // TRAFFIC_H
