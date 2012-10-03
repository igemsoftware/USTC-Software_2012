#ifndef MYQGRAPHICSVIEW_H
#define MYQGRAPHICSVIEW_H


#include <fstream>
#include <cmath>
#include <QFileDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPen>
#include <QPainterPath>
#include <QGraphicsEffect>

#define SQR(x) ((x)*(x))
//#include "MyQGraphicsPathItem.h"


class MyQGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MyQGraphicsView(QWidget *parent = NULL);
    ~MyQGraphicsView();
    //void operator= (MyQGraphicsView &MyQGraphicsView);
    void DrawData(int ynum);

    QGraphicsPathItem *pathitem;
    double *ymaxpoint, *yminpoint;
    int numr, numind, numprot, numInputSets;
    double *xpoints;
    double **ypoints;

    int hasdrawn;

public slots:
    void OpenDataFile(QString filename);

    //void zoom(double factor);

signals:
    void selected(QColor color, QString type, int n);
    void nonselected();

protected:
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    double calcunit(double max);
    QGraphicsScene graphicsScene;

    double xfactor, yfactor;

    QGraphicsDropShadowEffect *shadoweffect;

    QColor pencolor[4], lightcolor[4];
    QPen mypen, axispen, netpen;
};


#endif // MYQGRAPHICSVIEW_H