#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QtGui>
#include <QApplication>
#include <QtCore>
#include <QThread>
#include "ui_mainwindow.h"
#include "workthread.h"
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    
private slots:
    void on_btnSend_clicked();

    void on_btnListen_clicked();

    void on_btnConnect_clicked();


public slots:
    void onMsgChanged(QString);
    void onSocks(int, int);
    void onCredentials(QString, QString, QString);
    void onError(QString);

signals:

    void Conn_Type(QString, QString);


private:
    Ui::MainWindow *ui;
    class workThread *wkthread;

    int conn_in_sock;
    int out_sock;
    QString ConnType;
    QString connect_to_ip;
    char send_msg[80];
    QString RMsg;
    QString IPAddy;
    QString theirName;
    QString yourName;
};

#endif // MAINWINDOW_H
