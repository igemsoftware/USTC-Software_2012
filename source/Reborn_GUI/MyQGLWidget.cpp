#include "MyQGLWidget.h"


MyQGLWidget::MyQGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{

    //xRot = 60;
    //yRot = 0;
    zRot = 0;
    eyeangle = PI/4;

    labelfont = QFont("Verdana", 12);

    flagname[0] = "Resources/flags/blue-flag.png";
    flagname[1] = "Resources/flags/cyan-flag.png";
    flagname[2] = "Resources/flags/green-flag.png";
    flagname[3] = "Resources/flags/orange-flag.png";
    flagname[4] = "Resources/flags/pink-flag.png";
    flagname[5] = "Resources/flags/purple-flag.png";
    flagname[6] = "Resources/flags/red-flag.png";
    flagname[7] = "Resources/flags/yellow-flag.png";

    diskcolor[0] = QColor(25, 146, 240);
    diskcolor[1] = QColor(22, 219, 223);
    diskcolor[2] = QColor(23, 196, 46);
    diskcolor[3] = QColor(241, 148, 0);
    diskcolor[4] = QColor(227, 24, 112);
    diskcolor[5] = QColor(149, 22, 223);
    diskcolor[6] = QColor(230, 44, 23);
    diskcolor[7] = QColor(234, 221, 23);
}


MyQGLWidget::~MyQGLWidget()
{
    for (int i = 0; i < 3; i++)
    {
        delete [] matrix[i];
    }
    delete [] matrix;
}

void MyQGLWidget::initializeGL()
{

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);

    //anti-alias
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glShadeModel(GL_SMOOTH);
    
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);    

    //line width
    glLineWidth(2.5);

    //size of the matrix
    n = 4;

    matrix = new int*[n];
    for (int i = 0; i < n; i++)
    {
        matrix[i] = new int[n];
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = i>j?1:-1;
        }
    }
    
}


void MyQGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    //map
    glColor4f(1, 1, 1, 0.6);

    bindTexture(QPixmap("Resources/map.png"), GL_TEXTURE_2D, GL_RGBA);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-157.28, -120, -2);
    glTexCoord2f(1, 0); glVertex3f(157.28, -120, -2);
    glTexCoord2f(1, 1); glVertex3f(157.28, 120, -2);
    glTexCoord2f(0, 1); glVertex3f(-157.28, 120, -2);
    glEnd();


    for (int i = 0; i < n; i++)
    {
        //coords
        double x = -100 * cos(PI * 2/n * i);
        double y = 100 * sin(PI * 2/n * i);



        glColor4f(1, 1, 1, 0.9);

        //texture
        bindTexture(QPixmap(flagname[i]), GL_TEXTURE_2D, GL_RGBA);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


        //disk
        glColor4f(diskcolor[i].redF(), diskcolor[i].greenF(), diskcolor[i].blueF(), 0.95);
        drawdisk(x, y, 1);

        glPushMatrix();
        glTranslatef(x, y, 0);
        glRotatef(-zRot, 0.0, 0.0, 1.0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //flag
        glColor4f(1, 1, 1, 0.9);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-3, 0, 0);
        glTexCoord2f(1, 0); glVertex3f(17, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(17, 0, 34);
        glTexCoord2f(0, 1); glVertex3f(-3, 0, 34);
        glEnd();

        //label
        renderText(5, 0, 5, "g" + QString::number(i+1), labelfont);

        glDisable(GL_BLEND);
        glPopMatrix();


        //relation
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j])
            {
                if (i == j)
                {

                } 
                else
                {

                    double dis = 200 * sin(PI / n * abs(i-j));

                    double ix = -100 * cos(PI * 2/n * i);
                    double jx = -100 * cos(PI * 2/n * j);
                    double iy = 100 * sin(PI * 2/n * i);
                    double jy = 100 * sin(PI * 2/n * j);

                    double mx = (ix + jx) / 2;
                    double my = (iy + jy) / 2;

                    double mdis = sqrt(SQR(mx) + SQR(my));

                    double ox = mx * (1 + dis/mdis * (i > j ? -1 : 1));
                    double oy = my * (1 + dis/mdis * (i > j ? -1 : 1));

                    if ((i == n-1 && j == 0)||(j == n-1 && i ==0))
                    {
                        double temp = ix;
                        ix = jx;
                        jx = temp;
                        temp = iy;
                        iy = jy;
                        jy = temp;
                    }

                    double startangle = atan2(iy - oy, ix - ox) + asin(5/dis/1.118) * 3;
                    double endangle = atan2(jy - oy, jx - ox) - asin(5/dis/1.118) * 3;

                    //double arrowangle = asin(5/dis/1.118) * 2;
                    double arrowx = ox + dis * 1.118 * cos(endangle);
                    double arrowy = oy + dis * 1.118 * sin(endangle);

                    double dx = arrowx - jx;
                    double dy = arrowy - jy;
                    double k = -dx/dy;
                    double ax = 6/sqrt(1 + SQR(k));
                    double ay = ax * k;

                    if (matrix[i][j] == 1)
                    {
                        glColor4f(0, 1, 0, 1);
                        glBegin(GL_LINE_STRIP);
                        glVertex3f(arrowx + 0.8 * dx + ax, arrowy + 0.8 * dy + ay, 0);
                        glVertex3f(arrowx, arrowy, 0);
                        glVertex3f(arrowx + 0.8 * dx - ax, arrowy + 0.8 * dy - ay, 0);
                        glEnd();
                    }
                    else
                    {
                        glColor4f(1, 0, 0, 1);
                        glBegin(GL_LINES);
                        glVertex3f(arrowx + ax, arrowy + ay, 0);
                        glVertex3f(arrowx - ax, arrowy - ay, 0);
                        glEnd();
                    }


                    
                    //sqrt(5)/2 = 1.118
                    drawArc(dis * 1.118, ox, oy, 0, startangle, endangle);
                }
            }
        }
    }


    glLoadIdentity();
    
    gluLookAt(0, -400 * cos(eyeangle), 400 * sin(eyeangle), 0, 0, 1, 0, 1, 0);

    //glRotatef(60, 1.0, 0.0, 0.0);
    //glRotatef(yRot, 0.0, 1.0, 0.0);
    glRotatef(zRot, 0.0, 0.0, 1.0);
}

void MyQGLWidget::resizeGL(int width, int height)
{
    
    glViewport((width - height * 1.6) / 2, 0, height * 1.6, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
    //glOrtho(-200.0, 200.0, -125.0, 125.0, -1000.0, 1000.0);
    gluPerspective(45.0, 1.6, 0.1, 2000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
}


void MyQGLWidget::drawdisk(double x, double y, double z)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    //glRotatef(60, 1.0, 0.0, 0.0);
    GLUquadricObj *quadratic = gluNewQuadric();
    gluQuadricDrawStyle(quadratic, GLU_FILL);
    //gluQuadricDrawStyle(quadratic, GLU_LINE);
    //gluQuadricTexture(quadratic, GLU_TRUE);
    gluQuadricNormals(quadratic, GLU_SMOOTH);
    gluDisk(quadratic, 0, 10, 28, 1);
    gluDeleteQuadric(quadratic);
    glPopMatrix();
}


void MyQGLWidget::drawArc(double r, double x, double y, double z, double startangle, double endangle)
{
    /*
    if (startangle > endangle)
    {
        double temp = endangle;
        endangle = startangle;
        startangle = temp;
    }
    if (endangle - startangle > PI)
    {
        startangle += 2 * PI;
        double temp = endangle;
        endangle = startangle;
        startangle = temp;
    }
    */
    if (startangle > endangle + PI)
    {
        endangle += 2 * PI;
    }

    double deltaangle = (endangle - startangle)/30;
    glBegin(GL_LINE_STRIP);
    for (double angle = startangle; angle <= endangle; angle += deltaangle)
    {
        glVertex3f(x + r * cos(angle), y + r * sin(angle), z);
    }
    glEnd();

}

/*
void MyQGLWidget::drawcircle(double x, double y, double z)  
{  
    double radius = 0.1;

    int n = 30;
    glBegin(GL_LINE_LOOP);  
    for (int i = 0; i < n; i++)
    {
        glVertex3f(x + radius * cos(i * 2 * PI / n), y + radius * sin(i * 2 * PI / n), z);
    }
    glEnd();
}
*/



void MyQGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}


void MyQGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    GLfloat dx = (GLfloat)(event->x() - lastPos.x()) / width();

    if (event->buttons() & Qt::LeftButton)
    {
        zRot += 180 * dx;
        updateGL();
    }
    lastPos = event->pos();
}

void MyQGLWidget::wheelEvent(QWheelEvent *event)
{
    eyeangle += event->delta()/3000.0;
    if (eyeangle < PI/2 && eyeangle > 0)
    {
        updateGL();
    }
    else
    {
        eyeangle -= event->delta()/3000.0;
    }
}


/*
void MyQGLWidget::setXRotation(GLfloat angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;

    if (angle != xRot) {
        xRot = angle;
        //emit xRotationChanged(angle);
        updateGL();
    }
}

void MyQGLWidget::setYRotation(GLfloat angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;

    if (angle != yRot) {
        yRot = angle;
        //emit xRotationChanged(angle);
        updateGL();
    }
}

void MyQGLWidget::setZRotation(GLfloat angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;

    if (angle != zRot) {
        zRot = angle;
        //emit xRotationChanged(angle);
        updateGL();
    }
}
*/