#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "server.h"
#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Server server;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void PrintMessage(QString str);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
