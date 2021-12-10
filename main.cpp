#include "pingui.h"

#include <QApplication>
#include <netinet/ip_icmp.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PinGUI w;
    w.show();
    return a.exec();
}
