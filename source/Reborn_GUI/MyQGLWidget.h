#ifndef MYQGLWIDGET_H
#define MYQGLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>


#if defined(Q_WS_MAC)
# include <OpenGL/glu.h>
#else
# ifndef QT_LINUXBASE
#   include <GL/glu.h>
# endif
#endif


#define SQR(x) ((x)*(x))
#define PI 3.141592653589793238462643383279

class MyQGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    MyQGLWidget(QWidget *parent = 0);
    ~MyQGLWidget();

    int **matrix;
    int n;

    //QSize minimumSizeHint() const;
    //QSize sizeHint() const;
/*
public slots:
    void setXRotation(GLfloat angle);
    void setYRotation(GLfloat angle);
    void setZRotation(GLfloat angle);
/*
signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
*/
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void drawcircle(double x, double y, double z);
    void drawdisk(double x, double y, double z);
    void drawArc(double r, double x, double y, double z, double startangle, double endangle);
    //GLfloat xRot;
    //GLfloat yRot;
    GLfloat zRot;
    double eyeangle;
    QPoint lastPos;

    QString flagname[8];
    QColor diskcolor[8];
    QFont labelfont;
    //QColor qtPurple;
};

#endif // MYQGLWIDGET_H