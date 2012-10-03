#include "igem2012gui.h"
#include <QtGui/QApplication>
#include <QPlastiqueStyle>

int main(int argc, char *argv[])
{
    QApplication::setStyle(new QPlastiqueStyle);
    QApplication a(argc, argv);
    iGEM2012GUI w;
    //w.setAttribute(Qt::WA_TranslucentBackground);
    //w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();
    return a.exec();
}
