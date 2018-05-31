#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // 设置 主窗口名字和图标
    w.setWindowTitle("young_serial_test");
    QIcon icon("NCHU.ico");
    w.setWindowIcon(icon);
    w.show();

    return a.exec();

}
