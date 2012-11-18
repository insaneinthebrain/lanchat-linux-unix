#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>
#include <QObject>
#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QtCore>
#include "mainwindow.h"
#include "ui_mainwindow.h"
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

class workThread : public QThread
{
    Q_OBJECT
public:
    explicit workThread(QObject *parent = 0);
    void run();

signals:
    void MsgChanged(QString);
    void Socks(int,int);
    void Credentials(QString, QString, QString);
    void ReportError(QString);
    
public slots:
    void onConn_Type(QString, QString);

private:
      void ChatBrains();
      void set_incoming();
      void set_outgoing();
      void listen_for();
      void connect_to();
      void Recieve_Message();
      class MainWindow *mainW;
      QString qMsg;
      QString ConnType;
      QString Q2IP;
      struct sockaddr_in in_address;
      struct sockaddr_in conn_address;
      struct sockaddr_in out_address;
      char connect_to_ip[15];
      int in_addr_len;
      socklen_t conn_addr_len;
      int out_addr_len;
      int connection_result;
      int in_sock, conn_in_sock;
      int out_sock;
      char recv_msg[80];
      bool serv;
      char host_ip[INET_ADDRSTRLEN];
      char host_name[80];
      char send_name[80];
      QString QhostName;
      QString QsendName;
      QString QIP;
      QString errmsg;
    
};

#endif // WORKTHREAD_H
