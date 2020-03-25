#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
  MainWindow w;
   w.showFullScreen();

//    AuthConnection w;
//    w.showNormal();
    return a.exec();
}
