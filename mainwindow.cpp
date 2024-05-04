#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&server, &Server::showMessage, this, &MainWindow::PrintMessage);
}

void MainWindow::PrintMessage(QString str){
    ui->textEdit->append(str + "\n");
}

MainWindow::~MainWindow()
{
    delete ui;
}
