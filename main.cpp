#include "mainwindow.h"
#include <QApplication>

#include <exception>
#include <iostream>
#include <boost/version.hpp>
#include <iomanip>

int main(int argc, char *argv[])
{
    int exit = -1;


        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        exit = a.exec();


    return exit;
}
