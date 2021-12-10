#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ipv4_header.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setIcmpFields(QString & target)
{
    QByteArray tmp = target.toLocal8Bit();
    char * ctarget = tmp.data();
    ping.url = ctarget;
    ping.sent = 0;
    ping.received = 0;
    ping.timer = new t_time;
    ping.reply = new t_reply;
    ping.pack = new t_pack;
}

void MainWindow::on_pingButton_clicked()
{
    QString targetAddr = ui->lineEdit->text();
    setIcmpFields(targetAddr);
}

