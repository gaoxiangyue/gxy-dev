#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include "DataStruct/datastruct.h"
#include "ImPortMap/xmlparser.h"
#include "Astar/Astar.h"
#include <QMouseEvent>
#include <QTouchEvent>
#include <QPushButton>
#include <QSlider>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>


class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = 0);
    ~MapWidget();
    QPushButton*next_step_bt;

protected:
       void paintEvent(QPaintEvent *);
       void mousePressEvent(QMouseEvent *event) ;
       void mouseMoveEvent(QMouseEvent *event) ;
       void mouseReleaseEvent(QMouseEvent *event) ;
       void mouseDoubleClickEvent(QMouseEvent *event) ;
       void touchEvent(QTouchEvent *event);
private://ui
       int WIDTH;
       int HEIGHT;
       QPushButton *importButton, *start_select_bt,*end_select_bt;
       QLabel *next_step_lb;
       QPushButton *posing,*scaleUP,*scaleDown;
       QGroupBox *NavigationBox;
       QLineEdit *startpos, *endpos;
       QPushButton *navi_bt;


private:
    Bounds bounds;
    QMultiHash <QString ,Node> nodeHash;
    QList <Way> wayList;
    QList <Relation> relationList;
    std::vector<Point *> points;
    std::list<Point *> path;
    std::list<Point *> path1;

private:
    QString filename;
    QPoint presspos;
    QPoint movedpos;
    QPoint moveingpos;
    double scale;
    Node geocenter;
    Node start,end;
    int selectflag;
    int oldPointNo;
    //----
    void viewinit();
    Node window2geo(QPoint pos);
    QPoint geo2window(Node node);
    Node getnearstpoint(Node node);

private slots:
    void OpenMapFile();
    void scaleup();
    void scaledown();
    void navigationshow(bool s);
    bool writepath2mfile(std::list<Point *> path);
    void posingcenter();

public slots:
    void setgeocenter(double lon,double lat);
    void setRoadPointNo(int No);
signals:
    void getRoadPointNo(int No);//
    void getRoadPointLon(double Lon);
    void getRoadPointLat(double Lat);
    void getRoadPointAlt(double Alt);
    void getRoadPointLspeed(int Lspeed);

};

#endif // MAPWIDGET_H
