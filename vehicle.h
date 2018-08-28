#ifndef VEHICLE_H
#define VEHICLE_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>

class Vehicle : public QWidget
{
    Q_OBJECT

public:
    Vehicle(QWidget *parent = 0);
    ~Vehicle(){}
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event) ;

private:
    double vehicle_speed,engine_temp,engine_speed,remaining_energy;
    int drive_mode;
    double steering_angle;//,steerang_speed,break_pressure;
    //panel distance
    int distance;
    int show_index;
    QPoint pos1,pos2,size1,size2;
    QLabel *SysMode_Ret,*Steering_Ang;//,*SteerAng_Speed,*Breaking_Pressure;
public slots:
    void showVehicle_speed(double vehicle_speed);
    void showEngine_temp(double engine_temp);
    void showEngine_speed(double engine_speed);
    void showRemaining_energy(double remaining_energy);
    void showDriving_mode(int mode);
    void showSteering_angle(double deg);
    //void showSteerAng_speed(double degps);
    //void showBreaking_pressure(double pressure);

};

#endif // VEHICLE_H
