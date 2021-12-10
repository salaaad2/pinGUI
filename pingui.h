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
    Ui::MainWindow * ui;
    t_ping * ping;
    void setIcmpFields(QString & target);
    int setSockFields();
    int pingLoop(t_ping * ping, int const & sock);
    int resetPack(t_pack * pack, int & seq);
    unsigned int packSum(const void *data, unsigned int size);

};
#endif // MAINWINDOW_H
