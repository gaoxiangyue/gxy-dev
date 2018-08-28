#ifndef TRAFFIC_H
#define TRAFFIC_H

#include <QWidget>
//
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QMouseEvent>

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
    double obj_dis[5],obj_ang[5],obj_hdg[5],obj_width[5],obj_class[5];//5障碍物参数

    QPoint vhcXY2winXY(double vhcX,double vhcY);


protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event) ;
    void mouseMoveEvent(QMouseEvent *event) ;

signals:

public slots:
    void showNRYG_light(int nrgy);
    //Lane
    void show_L0_a(double a0);
    void show_L0_b(double b0);
    void show_L0_c(double c0);
    void show_L1_a(double a1);
    void show_L1_b(double b1);
    void show_L1_c(double c1);
    void show_L2_a(double a2);
    void show_L2_b(double b2);
    void show_L2_c(double c2);
    void show_L3_a(double a3);
    void show_L3_b(double b3);
    void show_L3_c(double c3);
    //Obj
    void show_Obj0_dis(double dis0);
    void show_Obj0_angle(double angle0);
    void show_Obj0_heading(double heading0);
    void show_Obj0_width(double width0);
    void show_Obj0_class(double class0);
    void show_Obj1_dis(double dis1);
    void show_Obj1_angle(double angle1);
    void show_Obj1_heading(double heading1);
    void show_Obj1_width(double width1);
    void show_Obj1_class(double class1);
    void show_Obj2_dis(double dis2);
    void show_Obj2_angle(double angle2);
    void show_Obj2_heading(double heading2);
    void show_Obj2_width(double width2);
    void show_Obj2_class(double class2);
    void show_Obj3_dis(double dis3);
    void show_Obj3_angle(double angle3);
    void show_Obj3_heading(double heading3);
    void show_Obj3_width(double width3);
    void show_Obj3_class(double class3);
    void show_Obj4_dis(double dis4);
    void show_Obj4_angle(double angle4);
    void show_Obj4_heading(double heading4);
    void show_Obj4_width(double width4);
    void show_Obj4_class(double class4);

};

#endif // TRAFFIC_H
