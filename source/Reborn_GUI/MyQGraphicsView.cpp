#include "MyQGraphicsView.h"


MyQGraphicsView::MyQGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    xfactor = yfactor = 1.0;
    hasdrawn = 0;
    
    numr = numind = numprot = numInputSets = 0;

    QGraphicsScene graphicsScene;


    setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform|QPainter::TextAntialiasing);
    //setDragMode(QGraphicsView::ScrollHandDrag);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setCursor(Qt::ArrowCursor);
    setStyleSheet("background:transparent; border:0px");
    setWindowFlags(Qt::FramelessWindowHint);

    pencolor[0] = QColor(1, 128, 160);
    pencolor[1] = QColor(105, 146, 55);
    pencolor[2] = QColor(166, 41, 19);
    pencolor[3] = QColor(183, 143, 48);

    lightcolor[0] = QColor(2, 177, 220);
    lightcolor[1] = QColor(166, 218, 97);
    lightcolor[2] = QColor(239, 70, 38);
    lightcolor[3] = QColor(249, 194, 82);

    axispen.setCapStyle(Qt::RoundCap);
    axispen.setCosmetic(true);

    netpen.setColor(QColor(Qt::lightGray));
    netpen.setStyle(Qt::DashLine);
    netpen.setCapStyle(Qt::RoundCap);
    netpen.setCosmetic(true);

    setMouseTracking(true);
}

MyQGraphicsView::~MyQGraphicsView()
{
    
    if (hasdrawn)
    {
        delete [] shadoweffect;
        delete [] pathitem;


        delete [] xpoints;

        for (int i = 0; i < (numind + numprot) * numInputSets; i++)
        {
            delete [] ypoints[i];
        }
        delete [] ypoints;

    }


};




void MyQGraphicsView::OpenDataFile(QString filename)
{

    std::ifstream infile;
    infile.open (filename.toStdWString().c_str());
    if (!infile.is_open())
    {
        QMessageBox::warning(NULL, "warning", "Error: unable to open input file: " + filename, QMessageBox::Ok, QMessageBox::Ok);
        infile.close();
        return;
    }


    if (hasdrawn)
    {
        delete [] shadoweffect;
        delete [] pathitem;


        delete [] xpoints;
        delete [] ymaxpoint;
        delete [] yminpoint;

        for (int i = 0; i < (numind + numprot) * numInputSets; i++)
        {
            delete [] ypoints[i];
        }
        delete [] ypoints;

    } else
    {
        hasdrawn = 1;
    }


    //	number of xpoints, inducers and proteins
    infile >>  numr >> numind >> numprot>>numInputSets;
    if (!numr)
    {
        QMessageBox::warning(NULL, "warning", "Error: empty data", QMessageBox::Ok, QMessageBox::Ok);
        infile.close();
        return;
    }

    //	number of rows, inducer plus protein
    int numy = numind + numprot;

    
    //	read dynamic data
    xpoints = new double [numr];
    ypoints = new double* [numy * numInputSets];
    ymaxpoint = new double [numy * numInputSets];
    yminpoint = new double [numy * numInputSets];

    for (int i = 0; i < numy * numInputSets; i++)
    {
        ypoints[i] = new double [numr];
    }

    for(int ir = 0; ir < numr; ir++)
    {
        infile >> xpoints[ir];

        for(int iinput = 0; iinput < numy * numInputSets; iinput++)
        {
            infile >> ypoints[iinput][ir];

        }
        //std::cout<<std::endl;
    }

    infile.close();

    DrawData(0);
}



//n is the number of the graph
void MyQGraphicsView::DrawData(int n)
{
    int numy = numind + numprot;

    double xmax, ymax, ymin;

    xmax = xpoints[0];
    ymax = ymin = ypoints[0][0];
    
    //clear label
    emit nonselected();
    //clear scene
    graphicsScene.clear();
    graphicsScene.clearFocus();

    pathitem = new QGraphicsPathItem[numy];

    shadoweffect = new QGraphicsDropShadowEffect[numy];

    mypen.setWidth(5);
    mypen.setCapStyle(Qt::RoundCap);
    mypen.setJoinStyle(Qt::RoundJoin);
    mypen.setCosmetic(true);

    
    for(int i = 0; i < numy; i++)
    {
        //graphicsScene.addItem(&itemgroup[i]);
        QPainterPath path;
        path.moveTo(xpoints[0], ypoints[i][0]);
        ymaxpoint[i] = yminpoint[i] = ypoints[i][0];

        for(int j = 1; j < numr; j++)
        {
            
            path.lineTo(xpoints[j], ypoints[i][j]);

            xmax = xmax > xpoints[j] ? xmax : xpoints[j];
            
            ymax = ymax > ypoints[i][j] ? ymax : ypoints[i][j];

            ymaxpoint[i] = ymaxpoint[i] > ypoints[i][j] ? ymaxpoint[i] : ypoints[i][j];
            yminpoint[i] = yminpoint[i] < ypoints[i][j] ? yminpoint[i] : ypoints[i][j];
        }

        ymax = ymax > ymaxpoint[i] ? ymax : ymaxpoint[i];

        pathitem[i].setPath(path);

        mypen.setColor(pencolor[i%4]);

        pathitem[i].setPen(mypen);
        pathitem[i].setZValue(1);

        shadoweffect[i].setOffset(2,2);
        pathitem[i].setGraphicsEffect(&shadoweffect[i]);

        graphicsScene.addItem(&pathitem[i]);
    }
    
    double xunit = calcunit(xmax);
    xmax = ((int)(xmax / xunit)+1) * xunit;
    double yunit = calcunit(ymax);
    ymax = ((int)(ymax / yunit)+1) * yunit;

    graphicsScene.setSceneRect(-xmax * 0.1, -ymax * 0.1, xmax * 1.2, ymax * 1.2);

    //axis
    graphicsScene.addLine(0, 0, xmax, 0, axispen);
    graphicsScene.addLine(0, 0, 0, ymax, axispen);

    QGraphicsSimpleTextItem *textitem;
    QFont textfont;
    textfont.setPointSize(9);

    //x axis
    for (int i = 0; i * xunit < xmax; i++)
    {
        //axis
        graphicsScene.addLine(i * xunit, 0, i * xunit, -5.0/height()*ymax, axispen);
        //net
        graphicsScene.addLine((i+1) * xunit, 0, (i+1) * xunit, ymax, netpen);
        //text
        textitem = graphicsScene.addSimpleText(QString::number(i*xunit, 'f', 1), textfont);
        textitem->setPos(i * xunit - 8.0/width()*xmax, -7.0/height()*ymax);
        textitem->setFlags(QGraphicsItem::ItemIgnoresTransformations);
    }

    //y axis
    for (int i = 0; i * yunit < ymax; i++)
    {
        //axis
        graphicsScene.addLine(0, i * yunit, -5.0/width()*xmax, i * yunit, axispen);
        //net
        graphicsScene.addLine(0, (i+1) * yunit, xmax, (i+1) * yunit, netpen);
        //text
        textitem = graphicsScene.addSimpleText(QString::number(i*yunit, 'f', 1), textfont);
        textitem->setPos(-26.0/width()*xmax, i * yunit + 7.0/height()*ymax);
        textitem->setFlags(QGraphicsItem::ItemIgnoresTransformations);
    }

    setScene(&graphicsScene);

    scale(width()/1.1/xmax/xfactor, -height()/1.1/ymax/yfactor);
    xfactor = width()/1.1/xmax;
    yfactor = -height()/1.1/ymax;

    centerOn(xmax * 0.5, ymax * 0.5);

    show();
}

double MyQGraphicsView::calcunit(double max)
{
    int exp = 0;
    while (max <= 5)
    {
        max *= 10;
        exp--;
    } 
    while (max > 50)
    {
        max /= 10;
        exp++;
    }
    if (max <= 10)
    {
        return 1.0 * pow(10.0, exp);
    }
    else if (max <= 20)
    {
        return 2.0 * pow(10.0, exp);
    }
    else if (max <= 40)
    {
        return 4.0 * pow(10.0, exp);
    }
    else
    {
        return 5.0 * pow(10.0, exp);
    }
}


//disable zoom
/*
void MyQGraphicsView::zoom(double factor)
{
    scale(factor, factor);

    xfactor *= factor;
    yfactor *= factor;
}
*/

void MyQGraphicsView::wheelEvent(QWheelEvent* event)
{
    /*
    if(event->delta() > 0)
    {
        //Zoom in
        zoom(1.15);
    } else
    {
        //Zooming out
        zoom(1.0 / 1.15);
    }
    */

}


void MyQGraphicsView::mousePressEvent(QMouseEvent *event)
{
    //this->setGraphicsEffect(seletedcolor);

    //QGraphicsItem *item = graphicsScene.itemAt(mapToScene(event->pos()));
    QPointF p = event->pos();
    int found = 0;

    for (int i = 0; i < numind + numprot; i++)
    {

        mypen = pathitem[i].pen();
        mypen.setWidth(5);
        mypen.setColor(pencolor[i%4]);
        pathitem[i].setZValue(1);
        
        if (!found)
        {
            for (int j = 0; j < numr - 1; j++)
            {
                QPointF q = mapFromScene(xpoints[j], ypoints[i][j]);
                QPointF q1 = mapFromScene(xpoints[j + 1], ypoints[i][j + 1]);

                double dis = SQR(p.x() - q.x()) + SQR(p.y() - q.y());

                if ((dis + SQR(p.x() - q1.x()) + SQR(p.y() - q1.y())) < ((SQR(q.x() - q1.x()) + SQR(q.y() - q1.y())))
                    || (dis < 25))
                {
                    mypen.setWidth(7);
                    mypen.setColor(lightcolor[i%4]);
                    pathitem[i].setZValue(2);
                    //info
                    QString typestring;
                    if (i < numind)
                    {
                        typestring = "inducer";

                        if (numind + numprot > 1)
                        {
                            typestring += " " + QString::number(i + 1);
                        }
                    }
                    else 
                    {
                        typestring = "protein";

                        if (numind + numprot > 1)
                        {
                            typestring += " " + QString::number(i + 1 - numind);
                        }
                    }
                    emit selected(lightcolor[i%4], typestring, i);
                    found = 1;
                    break;
                }
            }
        }

        pathitem[i].setPen(mypen);

    }
    if (!found)
    {
        emit nonselected();
    }

    QGraphicsView::mousePressEvent(event);
}



void MyQGraphicsView::mouseMoveEvent(QMouseEvent *event)
{

    QPointF p = event->pos();
    int found = 0;

    for (int i = 0; i < numind + numprot; i++)
    {
        if (pathitem[i].zValue() == 1)
        {

            mypen = pathitem[i].pen();
            mypen.setColor(pencolor[i%4]);

            if (!found)
            {
                for (int j = 0; j < numr - 1; j++)
                {
                    QPointF q = mapFromScene(xpoints[j], ypoints[i][j]);
                    QPointF q1 = mapFromScene(xpoints[j + 1], ypoints[i][j + 1]);

                    double dis = SQR(p.x() - q.x()) + SQR(p.y() - q.y());

                    if ((dis + SQR(p.x() - q1.x()) + SQR(p.y() - q1.y())) < ((SQR(q.x() - q1.x()) + SQR(q.y() - q1.y())))
                        || (dis < 25))
                    {
                        mypen.setColor(lightcolor[i%4]);
                        found = 1;
                        break;
                    }
                }
            }

            pathitem[i].setPen(mypen);
        }

    }

    QGraphicsView::mouseMoveEvent(event);
}
