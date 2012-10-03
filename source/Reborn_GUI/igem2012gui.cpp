#include "igem2012gui.h"



iGEM2012GUI::iGEM2012GUI(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    setAutoFillBackground(false);
    setAttribute(Qt::WA_TranslucentBackground);
    
    setWindowFlags(Qt::FramelessWindowHint);

    setupmap();

    //only for debug
    //outputcellnum = 5;
}



iGEM2012GUI::~iGEM2012GUI()
{
    //delete graphicsView;
}

void iGEM2012GUI::selectinputfile()
{
    inputfilename = QFileDialog::getOpenFileName(this, "Open Data File", NULL, "Text files (*.txt)");
    if (inputfilename.size() == 0)
    {
        return;
    }
    graphicsView->OpenDataFile(inputfilename);
}


void iGEM2012GUI::setinfo(QColor color, QString type, int n)
{

    QPalette mypalette;
    mypalette.setColor(ui.label_color->backgroundRole(), color);
    ui.label_color->setAutoFillBackground(true);
    ui.label_color->setPalette(mypalette);

    ui.label_type->setText(type);
    ui.label_max->setText("Max value: " + QString::number(graphicsView->ymaxpoint[n], 'f', 3));
    ui.label_min->setText("Min value: " + QString::number(graphicsView->yminpoint[n], 'f', 3));
    ui.label_total_point->setText("Total points: " + QString::number(graphicsView->numr));
}



void iGEM2012GUI::clearinfo()
{
    ui.label_color->setAutoFillBackground(false);
    ui.label_type->clear();
    ui.label_max->clear();
    ui.label_min->clear();
    ui.label_total_point->clear();
}


//reset these value
void iGEM2012GUI::setdefault()
{
    ui2.spinBox->setValue(1000);
    ui2.spinBox_2->setValue(200);
    ui2.spinBox_3->setValue(5);
    ui2.spinBox_4->setValue(10);
}



//survivals and outputs cannot larger than population
void iGEM2012GUI::on_spinbox_value_change(int i)
{
    if (ui2.spinBox_3->value() > ui2.spinBox_2->value())
    {
        ui2.spinBox_3->setValue(ui2.spinBox_2->value());
    }

    if (ui2.spinBox_4->value() > ui2.spinBox_2->value())
    {
        ui2.spinBox_4->setValue(ui2.spinBox_2->value());
    }
}



void iGEM2012GUI::uploadprogress(qint64 bytesSent, qint64 bytesTotal)
{
    if (!uploaderrorcode)
    {
        ftpui.status->setText("Uploading: " + QString::number((double)bytesSent/bytesTotal*100, 'f', 3) + "%");

    }
}



void iGEM2012GUI::uploadfinished()
{
    static int i = 0;
    if (!uploaderrorcode)
    {
        //8 js file
        if (++i == outputcellnum + 8)
        {
            //delete uploadman;
            //delete upfile;
            ftpui.status->setText("All finished!");

        }
        else
        {
            ftpui.status->setText(QString::number(i) + " file finished!");
        }
    }


    uploaderrorcode = 0;
}



void iGEM2012GUI::uploaderror(QNetworkReply::NetworkError neterror)
{
    ftpui.status->setText("Upload error! Code: " + QString::number(neterror));
    uploaderrorcode = neterror;
}




void iGEM2012GUI::openftpdialog()
{
    ftpdlg = new QDialog(this);
    ftpui.setupUi(ftpdlg);
    ftpdlg->show();


    QObject::connect(ftpui.upload, SIGNAL(clicked()), this, SLOT(uploadtoftp()));
    QObject::connect(ftpui.cancel, SIGNAL(clicked()), ftpdlg, SLOT(close()));
}

//upload a file
void iGEM2012GUI::ftpupfile(QString filename, QString filepath)
{
    QUrl uploadurl("ftp://" + ftpui.lineEdit->text() + "/" + ftpui.lineEdit_5->text() + filename);
    uploadurl.setUserName(ftpui.lineEdit_3->text()); 
    uploadurl.setPassword(ftpui.lineEdit_4->text()); 
    uploadurl.setPort(ftpui.lineEdit_2->text().toInt()); 
    QNetworkRequest upload(uploadurl);


    upfile = new QFile(filepath + filename);
    upfile->open(QIODevice::ReadOnly);
    reply = uploadman->put(upload, upfile);

    QObject::connect(reply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(uploadprogress(qint64, qint64)));
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(uploadfinished()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(uploaderror(QNetworkReply::NetworkError)));

    
}

//on click upload
void iGEM2012GUI::uploadtoftp()
{
    uploaderrorcode = 0;

    ftpui.status->setText("");

    if (ftpui.lineEdit_5->text().length() && !ftpui.lineEdit_5->text().endsWith('/'))
    {
        ftpui.lineEdit_5->setText(ftpui.lineEdit_5->text() + "/");
    }
    
    QFtp *ftp = new QFtp();
    ftp->connectToHost(ftpui.lineEdit->text());
    ftp->login(ftpui.lineEdit_3->text(), ftpui.lineEdit_4->text());
    ftp->mkdir(ftpui.lineEdit_5->text() + "js");
    ftp->close();
    
    while(ftp->state());

    uploadman = new QNetworkAccessManager(this);
    //QObject::connect(uploadman, SIGNAL(finished(QNetworkReply *)), this, SLOT(uploadfinished(QNetworkReply *)));
    for(int i = 0; i < outputcellnum; i++)
    {
        ftpupfile("Cell_" + QString::number(i) + "_Chart.html", "Result/" + timestring + "/Saves/html/");
        ftpupfile("Cell_" + QString::number(i) + "_Description.html", "Result/" + timestring + "/Saves/html/");
    }
    
    ftpupfile("cell_description_style.css", "html/");
    ftpupfile("js/html5.js", "html/");
    ftpupfile("js/zingchart-html5-zoom-min.js", "html/");
    ftpupfile("js/jquery.min.js", "html/");
    ftpupfile("js/zingchart-html5-core-min.js", "html/");
    ftpupfile("js/zingchart-html5-guide-min.js", "html/");
    ftpupfile("js/zingchart-html5-line-min.js", "html/");
    ftpupfile("js/zingchart-html5-preview-min.js", "html/");
    ftpupfile("js/zingchart-html5-xy-min.js", "html/");


}




//update stdout to QTextBrowser
void iGEM2012GUI::updatestdout()
{
    ui2.textEdit->append(QString(cmdline.readAllStandardOutput()));
}


void iGEM2012GUI::on_cmdexit(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (!exitStatus)
    {
        ui2.textEdit->append("Reborn finished!");
    }
    

    ui2.rebornButton->setText("Let's REBORN!");

}



void iGEM2012GUI::on_cmderror(QProcess::ProcessError error)
{
    ui2.textEdit->append("Error!\nCode: " + QString::number(error));
}




void iGEM2012GUI::reborn()
{
    if (cmdline.state())
    {
        cmdline.kill();
        cmdline.waitForFinished();
        ui2.textEdit->append("Process killed!");
        return;
    }
    
    //clear log
    ui2.textEdit->clear();
    
    //current time
    timestring = QDate::currentDate().toString("yyyy-MM-dd-") + QTime::currentTime().toString("HH-mm-ss");
    QDir dir;
    dir.mkpath("Result/" + timestring + "/Output");
    dir.mkpath("Result/" + timestring + "/Saves/html");

    if (!inputfilename.length())
    {
        inputfilename = QFileDialog::getOpenFileName(this, "Open Data File", NULL, "Text files (*.txt)");

    }
    

    QStringList arg;
    arg << "-f" << inputfilename << "-o" << "Result/" + timestring;
    arg << "-n" << "-m" << QString::number(1 + ui2.radioButton_2->isChecked());
    arg << "-e" << ui2.spinBox->text() << "-p" << ui2.spinBox_2->text();
    arg << "-u" << ui2.spinBox_3->text() << "-b" << ui2.spinBox_4->text();
    

    QObject::connect(&cmdline, SIGNAL(readyReadStandardOutput()), this, SLOT(updatestdout()));
    QObject::connect(&cmdline, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(on_cmdexit(int, QProcess::ExitStatus)));
    //QObject::connect(&cmdline, SIGNAL(error(QProcess::ProcessError)), this, SLOT(on_cmderror(QProcess::ProcessError)));

    //cmdline.setProcessChannelMode(QProcess::MergedChannels);
    cmdline.start("reborn_cl", arg);
    cmdline.waitForStarted();

    ui2.rebornButton->setText("Kill");

    QFile::copy("html/cell_description_style.css", "Result/" + timestring + "/Saves/html/cell_description_style.css");
    outputcellnum = ui2.spinBox_4->value();
}





void iGEM2012GUI::setupmap()
{
    ui.setupUi(this);

    graphicsView = new MyQGraphicsView(ui.centralWidget);

    graphicsView->setGeometry(QRect(90, 160, 621, 361));

    QObject::connect(ui.import_data, SIGNAL(clicked()), this, SLOT(selectinputfile()));
    QObject::connect(ui.go_to_console, SIGNAL(clicked()), this, SLOT(switch1to2()));

    QObject::connect(graphicsView, SIGNAL(selected(QColor, QString, int)), this, SLOT(setinfo(QColor, QString, int)));
    QObject::connect(graphicsView, SIGNAL(nonselected()), this, SLOT(clearinfo()));
}



void iGEM2012GUI::setupconsole()
{

    ui2.setupUi(this);

    QObject::connect(ui2.goto1, SIGNAL(clicked()), this, SLOT(switchto1()));
    QObject::connect(ui2.goto3, SIGNAL(clicked()), this, SLOT(switchto3()));
    QObject::connect(ui2.goto4, SIGNAL(clicked()), this, SLOT(switchto4()));

    QObject::connect(ui2.close, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(ui2.min, SIGNAL(clicked()), this, SLOT(showMinimized()));

    QObject::connect(ui2.defaultButton, SIGNAL(clicked()), this, SLOT(setdefault()));
    QObject::connect(ui2.rebornButton, SIGNAL(clicked()), this, SLOT(reborn()));


    QObject::connect(ui2.spinBox_2, SIGNAL(valueChanged(int)), this, SLOT(on_spinbox_value_change(int)));
    QObject::connect(ui2.spinBox_3, SIGNAL(valueChanged(int)), this, SLOT(on_spinbox_value_change(int)));
    QObject::connect(ui2.spinBox_4, SIGNAL(valueChanged(int)), this, SLOT(on_spinbox_value_change(int)));


    QObject::connect(ui2.ftp, SIGNAL(clicked()), this, SLOT(openftpdialog()));
}


void iGEM2012GUI::deleteconsole()
{
    ui2.centralwidget->close();
    delete ui2.centralwidget;
}


//Console
void iGEM2012GUI::switch1to2()
{
    
    ui.centralWidget->close();
    delete graphicsView;
    delete ui.centralWidget;

    setupconsole();
}

void iGEM2012GUI::switch3to2()
{
    for (int i = 0; i < 1; i++)
    {
        delete graphicsViewReport[i];
    }
    ui3.centralwidget->close();
    delete ui3.centralwidget;

    setupconsole();
}

void iGEM2012GUI::switch4to2()
{
    ui4.centralwidget->close();
    delete qgl;
    delete ui4.centralwidget;
    ui4window->close();
    delete ui4window;

    setupconsole();
}

//Report
void iGEM2012GUI::switchto3()
{
    


    //deleteconsole();


    ui3.setupUi(this);

    QObject::connect(ui3.pushButton, SIGNAL(clicked()), this, SLOT(switch3to2()));
    //QObject::connect(ui3.pushButton_2, SIGNAL(clicked()), this, SLOT(createjson()));
    
    for (int i = 0; i < outputcellnum; i++)
    {
        tabpage[i] = new QWidget();
        tabpage[i]->setStyleSheet("background:transparent; border:0px;");
        ui3.tabWidget->addTab(tabpage[i], "Cell " + QString::number(i+1));

        textview[i] = new QWebView(tabpage[i]);
        textview[i]->setGeometry(QRect(570, 10, 305, 480));
        textview[i]->setUrl(QUrl::fromLocalFile(QDir::currentPath() + "/Result/" + timestring + "/Saves/html/Cell_" + QString::number(i) + "_Description.html"));

        graphicsViewReport[i] = new MyQGraphicsView(tabpage[i]);
        graphicsViewReport[i]->setGeometry(QRect(0, 30, 515, 460));
        createjson(i);
    }
}

//Sandbox
void iGEM2012GUI::switchto4()
{
    deleteconsole();

    //create a new window to bypass the conflict between opengl and WA_TranslucentBackground
    ui4window = new QMainWindow(this);

    
    ui4.setupUi(ui4window);

    qgl = new MyQGLWidget(ui4.centralwidget);
    qgl->setGeometry(QRect(0, 0, 1024, 600));

    QObject::connect(ui4.pushButton, SIGNAL(clicked()), this, SLOT(switch4to2()));
    ui4window->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    ui4window->show();
}



void iGEM2012GUI::switchto1()
{
    deleteconsole();

    
    setupmap();
}


//html version report
void iGEM2012GUI::createjson(int ig)
{
    
    
    std::ifstream infile("html/chart_template.html");
    std::ofstream outfile("Result/" + timestring.toStdString() + "/Saves/html/Cell_" + QString::number(ig).toStdString() + "_Chart.html");
    if (!infile.is_open())
    {
        QMessageBox::warning(NULL, "warning", "Error: unable to open file: html/chart_template.html", QMessageBox::Ok, QMessageBox::Ok);
        infile.close();
        outfile.close();
        return;
    }
    outfile << infile.rdbuf();
    infile.close();
    
    
    infile.open("Result/" + timestring.toStdString() + "/Output/Cell_" + QString::number(ig).toStdString() + "_TimeCourses.txt");

    if (!infile.is_open())
    {
        QMessageBox::warning(NULL, "warning", "Error: unable to open input file: Cell_" + QString::number(ig) + "_TimeCourses.txt", QMessageBox::Ok, QMessageBox::Ok);
        infile.close();
        return;
    }

    

    //	number of xpoints, inducers and proteins
    infile >>  graphicsViewReport[ig]->numr >> graphicsViewReport[ig]->numind >> graphicsViewReport[ig]->numprot >> graphicsViewReport[ig]->numInputSets;
    if (!graphicsViewReport[ig]->numr)
    {
        QMessageBox::warning(NULL, "warning", "Error: empty data", QMessageBox::Ok, QMessageBox::Ok);
        infile.close();
        return;
    }


    //	number of rows, inducer plus protein
    int numy = graphicsViewReport[ig]->numind + graphicsViewReport[ig]->numprot;


    graphicsViewReport[ig]->xpoints = new double [graphicsViewReport[ig]->numr];
    graphicsViewReport[ig]->ypoints = new double* [numy * graphicsViewReport[ig]->numInputSets];
    graphicsViewReport[ig]->ymaxpoint = new double [numy * graphicsViewReport[ig]->numInputSets];
    graphicsViewReport[ig]->yminpoint = new double [numy * graphicsViewReport[ig]->numInputSets];

    for (int i = 0; i < numy * graphicsViewReport[ig]->numInputSets; i++)
    {
        graphicsViewReport[ig]->ypoints[i] = new double [graphicsViewReport[ig]->numr];
    }

    for (int i = 0; i < graphicsViewReport[ig]->numr; i++)
    {
        graphicsViewReport[ig]->xpoints[i] = i + 1;
    }


    outfile.precision(4);

    for(int iinput = 0; iinput < numy * graphicsViewReport[ig]->numInputSets; iinput++)
    {
        outfile << "{\"line-width\":\"1px\",\"text\":\"Series " << iinput+1 << "\",\"values\":[";
        //double y;
        for(int ir = 0; ir < graphicsViewReport[ig]->numr - 1; ir++)
        {
            infile >> graphicsViewReport[ig]->ypoints[iinput][ir];
            outfile << graphicsViewReport[ig]->ypoints[iinput][ir] << ",";

        }

        infile >> graphicsViewReport[ig]->ypoints[iinput][graphicsViewReport[ig]->numr - 1];
        outfile << graphicsViewReport[ig]->ypoints[iinput][graphicsViewReport[ig]->numr - 1]  << "]}";

        if (iinput != numy * graphicsViewReport[ig]->numInputSets - 1)
        {
            outfile << ",";
        }
    }


    infile.close();
    outfile << "]}]}</script></body></html>";
    outfile.close();

    graphicsViewReport[ig]->hasdrawn = 1;
    graphicsViewReport[ig]->DrawData(0);


}

//save the pos when mouse clicked
void iGEM2012GUI::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

//drag the window
void iGEM2012GUI::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        //move(x() + event->pos().x() - lastPos.x(), y() + event->pos().y() - lastPos.y());
        move(pos() + event->pos() - lastPos);
    }
    
}

