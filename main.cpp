
//Гриб Дарья 6 группа


#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.adjustGraphicsViewSize();
    return a.exec();
}
