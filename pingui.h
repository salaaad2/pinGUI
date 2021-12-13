#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pingheaders.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class PinGUI : public QMainWindow
{
    Q_OBJECT

public:
    PinGUI(QWidget *parent = nullptr);
    ~PinGUI();

private slots:
    void on_pingButton_clicked();

    void on_lineEdit_returnPressed();

private:
    // interface elements
    Ui::MainWindow * ui;

    // data. see pingheaders.h
    t_ping * ping;

    // ping logic
    int pingLoop(int const & sock);
    void sendPing(int const & sock);
    void displayData( void );

    // init and play with packets
    int setSockFields( void );
    void setIcmpFields(QString & target);
    void resetPack(int const & seq);
    unsigned int packSum(const void *data, unsigned int size);
};
#endif // MAINWINDOW_H
