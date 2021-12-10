/*********************************/
/*   cute_ping        (  //      */
/*   main              ( )/      */
/*   by salade         )(/       */
/*  ________________  ( /)       */
/* ()__)____________)))))   :^}  */
/*********************************/


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pingheaders.h"

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ping(new t_ping)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ping;
}

void MainWindow::setIcmpFields(QString & target)
{
    QByteArray tmp = target.toLocal8Bit();
    char * ctarget = tmp.data();
    struct sockaddr_in * servaddr;
    struct addrinfo * res;
    struct addrinfo hints = {
        AI_CANONNAME, AF_INET, SOCK_RAW, IPPROTO_ICMP, 0, NULL, NULL, NULL
    };
    void * addr;
    /* DNS resolution here */
    if (getaddrinfo(ctarget, NULL, &hints, &res) < 0)
    {
        return ;
    }
    if (res == NULL)
    {
        return ;
    }

    servaddr = reinterpret_cast<struct sockaddr_in *>(res->ai_addr);
    addr = &(servaddr->sin_addr);
    inet_ntop(res->ai_family, addr, ping->ipstr, sizeof (ping->ipstr));
    // display PING (url) in output region
    QString line = "PING";
    QString app(ping->ipstr);

    line += app;
    ui->textEdit->setPlainText(line);
    ping->url = ctarget;
    ping->sent = 0;
    ping->received = 0;
    ping->timer = new t_time;
    ping->reply = new t_reply;
    ping->pack = new t_pack;


}

void MainWindow::on_pingButton_clicked()
{
    QString targetAddr = ui->lineEdit->text();
    setIcmpFields(targetAddr);
}


void MainWindow::on_lineEdit_returnPressed()
{
    on_pingButton_clicked();
}

