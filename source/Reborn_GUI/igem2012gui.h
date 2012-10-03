#ifndef IGEM2012GUI_H
#define IGEM2012GUI_H

#include <QtGui/QMainWindow>
#include <QWebView>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFtp>
#include <QTime>
#include <QDate>
#include <QProcess>
#include <QDir>

#include "ui_igem2012gui.h"
#include "ui_Report.h"
#include "ui_Sandbox.h"
#include "ui_Console.h"
#include "ui_ftp.h"
#include "MyQGraphicsView.h"
#include "MyQGLWidget.h"

class iGEM2012GUI : public QMainWindow
{
    Q_OBJECT

public:
    iGEM2012GUI(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~iGEM2012GUI();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void selectinputfile();
    void setinfo(QColor color, QString type, int n);
    void clearinfo();
    void setdefault();
    void openftpdialog();
    void uploadtoftp();
    void reborn();
    void updatestdout();
    void on_cmdexit(int exitCode, QProcess::ExitStatus exitStatus);
    void on_cmderror(QProcess::ProcessError error);

    void ftpupfile(QString url, QString filepath);
    void uploadprogress(qint64 bytesSent, qint64 bytesTotal);
    void uploadfinished();
    void uploaderror(QNetworkReply::NetworkError);

    void switchto1();
    void switch1to2();
    void switch3to2();
    void switch4to2();
    void switchto3();
    void switchto4();

    void on_spinbox_value_change(int i);

private:
    Ui::iGEM2012GUIClass ui;
    Ui_Console ui2;
    Ui_MainWindow ui3;
    Ui_Sandbox ui4;
    Ui_FTPDialog ftpui;
    QDialog *ftpdlg;
    QMainWindow *ui4window;
    MyQGraphicsView *graphicsView;
    MyQGraphicsView *graphicsViewReport[10];
    MyQGLWidget *qgl;
    QWidget *tabpage[10];
    QWebView *textview[10];
    QPoint lastPos;
    int outputcellnum;
    int uploaderrorcode;
    QFile* upfile;
    QNetworkAccessManager *uploadman;
    QNetworkReply* reply;
    QString timestring;
    QString inputfilename;
    QProcess cmdline;

    void setupmap();
    void setupconsole();
    void deleteconsole();
    void createjson(int ig);
};




#endif // IGEM2012GUI_H
