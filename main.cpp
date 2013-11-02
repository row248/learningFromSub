#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "sqlprovider.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    SqlProvider *sql = new SqlProvider();
    sql->deleteWord("dog");

    return a.exec();
}
