/*********************************/
/*   cute_ping        (  //      */
/*   main              ( )/      */
/*   by salade         )(/       */
/*  ________________  ( /)       */
/* ()__)____________)))))   :^}  */
/*********************************/


#include "pingui.h"
#include "ui_mainwindow.h"
#include "pingheaders.h"

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
// constructor & destructor
PinGUI::PinGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ping(new t_ping)
{
    ui->setupUi(this);
}

PinGUI::~PinGUI()
{
    delete ui;
    delete ping->timer;
    delete ping->pack;
    delete ping->reply;
    delete ping;
}

// ping loop, send pkts and receive them (duh....)

// dns and t_ping init
void PinGUI::setIcmpFields(QString & target)
{
    // get text from input and cast it to char for subsequent use
    QByteArray tmp = target.toLocal8Bit();
    char * ctarget = tmp.data();

    // socket boilerplate
    struct sockaddr_in * servaddr;
    struct addrinfo * res;
    struct addrinfo hints = {
        AI_CANONNAME, AF_INET, SOCK_RAW, IPPROTO_ICMP, 0, NULL, NULL, NULL
    };
    void * addr;

    // DNS resolution here
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
    QString line = "PING: ";
    QString app(ping->ipstr);

    line += app;
    ui->textEdit->setPlainText(line);
    ping->url = ctarget;
    ping->sent = 0;
    ping->received = 0;
    ping->timer = new t_time;
    ping->reply = new t_reply;
    ping->pack = new t_pack;
    int sock = setSockFields();
    if (sock != -1)
    {
        pingLoop(ping, sock);
    }
}

// socket options
int PinGUI::setSockFields()
{
    const int hdr = 1;
    int sockfd;
    struct timeval rcv_timeout =
        {1, 1};
    rcv_timeout.tv_sec = 1;
    rcv_timeout.tv_usec = 0;
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
    {
        return (-1);
    }
    if ((setsockopt(sockfd, SOL_IP, IP_RECVERR, &hdr, sizeof(hdr))) != 0)
    {
        printf("%s %d\n", strerror(errno), errno);
        std::cerr << "setsockopt() REVERR failed" << std::endl;
        return (-1);
    }
    if ((setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &rcv_timeout, sizeof(rcv_timeout))) != 0)
    {
        std::cerr << "setsockopt() timer failed" << std::endl;
        return (-1);
    }
    return (sockfd);
}
// qt slots()
void PinGUI::on_pingButton_clicked()
{
    QString targetAddr = ui->lineEdit->text();
    setIcmpFields(targetAddr);
}


void PinGUI::on_lineEdit_returnPressed()
{
    on_pingButton_clicked();
}

