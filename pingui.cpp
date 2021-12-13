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
#include <unistd.h>

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

void
*coolMemcpy(void *dst, const void *src, size_t n)
{
    unsigned char	*dst_ptr;
    unsigned char	*src_ptr;
    size_t			i;

    dst_ptr = (unsigned char*)dst;
    src_ptr = (unsigned char*)src;
    i = 0;
    if (src_ptr == NULL && dst_ptr == NULL && n != 0)
    {
        return (NULL);
    }
    while (i < n)
    {
        dst_ptr[i] = src_ptr[i];
        i++;
    }
    return (dst_ptr);
}


// send pkt
void PinGUI::sendPing(int const & sock)
{
    socklen_t addrsize = sizeof(const struct sockaddr);
    char recvbuf[98];
    t_pack * pack = ping->pack;
    int ret = 42;

    memset(recvbuf, '\0', 98);
    if (sendto(sock, pack, PACK_SIZE, 0, (struct sockaddr *)ping->servaddr, addrsize) < 0) {
        return ;
    } else {
        std::cout << "sendto success !" << std::endl;
    }
    ping->sent++;

    if ((ret = recvfrom(sock, recvbuf, PACK_SIZE + IP_SIZE, 0, (struct sockaddr *)ping->servaddr, &addrsize)) < 0) {
        return ;
    } else {
        std::cout << "recv success !" << std::endl;
    }
    ping->received++;
    coolMemcpy(&ping->reply->ip, recvbuf, 20);
    coolMemcpy(&ping->reply->hdr, recvbuf + 20, 64);
}

// ping loop, send pkts and receive them (duh....)
int PinGUI::pingLoop(int const & sock)
{
    auto running = true;
    auto seq = 0;

    for (auto i = 10; i > 0; i--)
    {
        if (running)
        {
            std::cout << "coolman + [" << ping->reply->hdr.un.echo.sequence <<  "]" << ping->reply->ip.ttl << std::endl;
            resetPack(seq);
            sendPing(sock);
            // TODO: displayData();
            seq++;
        }
    }
    return (0);
}

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

    ping->servaddr = servaddr;
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
        pingLoop(sock);
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

// compute checksum
unsigned int PinGUI::packSum(const void *data, unsigned int size)
{
    long sum = 0;
    int i;

    i = 0;
    while( size > 1 )  {
        sum += ((uint16_t*) data)[i];
        i++;
        size -= 2;
    }

    if( size > 0 )
        sum += * (uint8_t *) data;

    while (sum>>16)
        sum = (sum & 0xffff) + (sum >> 16);

    return ((int16_t)~sum);
}

// reset packet and
void PinGUI::resetPack(int const & seq)
{
    t_pack * pack = ping->pack;
    memset(pack, '\0', sizeof (t_pack));

    pack->hdr.type = ICMP_ECHO;
    pack->hdr.code = 0;
    pack->hdr.checksum = 0;
    pack->hdr.un.echo.id = getpid();
    pack->hdr.un.echo.sequence = seq;

    memcpy(pack->load, DATA_CONT, DATA_SIZE);
    pack->hdr.checksum = packSum(pack, PACK_SIZE);
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

