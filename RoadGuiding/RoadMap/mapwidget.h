#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "DataStruct/datastruct.h"
#include "ImPortMap/xmlparser.h"
#include "Astar/Astar.h"
#include <QMouseEvent>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void setgeocenter(Node node);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event) ;
    void mouseMoveEvent(QMouseEvent *event) ;
    void mouseReleaseEvent(QMouseEvent *event) ;
    void mouseDoubleClickEvent(QMouseEvent *event) ;

private:
    Ui::Widget *ui;
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
    void viewinit();
    Node window2geo(QPoint pos);
    QPoint geo2window(Node node);
    Node getnearstpoint(Node node);

private slots:
    void OpenMapFile();
    void rescale();
    void navigationshow(bool s);
    bool writepath2mfile(std::list<Point *> path);
};

#endif // WIDGET_H
