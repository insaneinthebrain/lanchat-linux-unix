#include "workthread.h"
#include <QThread>
#include <QObject>
#include <QtGui>
#include <QApplication>
#include <QtCore>
#include <QMainWindow>
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

workThread::workThread(QObject *parent) :
    QThread(parent)
{
}

void workThread::run()
{

     ChatBrains();
}

void workThread::ChatBrains(){
    if(ConnType=="Listen"){
        listen_for();
    }
    if(ConnType=="Connect"){
        connect_to();
    }


}


void workThread::listen_for(){

        set_incoming();
        set_outgoing();

        errmsg = "Now listenting for connections......";
        emit ReportError(errmsg);
        listen(in_sock, 5);

        conn_addr_len = sizeof(conn_address);
        conn_in_sock = accept(in_sock, (struct sockaddr*)&conn_address, &conn_addr_len);
        for(int x=0; x<(sizeof(host_ip)/sizeof(host_ip[0])); x++){
            host_ip[x] = *"";
        }
        getpeername(conn_in_sock, (struct sockaddr*)&conn_address, &conn_addr_len);
        inet_ntop(AF_INET, &conn_address.sin_addr, host_ip, sizeof host_ip);

        for(int x=0; x<80; x++){
            host_name[x] = *"";
            send_name[x] = *"";
        }
        read(conn_in_sock, &host_name, sizeof(host_name));

        gethostname(send_name, 79);
        write(conn_in_sock, &send_name, sizeof(send_name));


        QIP = QString::fromStdString(host_ip);
        QhostName = QString::fromStdString(host_name);
        QsendName = QString::fromStdString(send_name);

        emit Credentials(QIP, QhostName, QsendName);
        emit Socks(conn_in_sock, out_sock);
        serv=true;
        Recieve_Message();



}


void workThread::connect_to(){

        set_incoming();
        set_outgoing();
        connection_result = ::connect(out_sock, (struct sockaddr*)&out_address, out_addr_len);
        if(connection_result==-1){
            errmsg = "Unable to connect to " + QString::fromStdString(connect_to_ip) + "\nThey may not be listening for connections.";
            emit ReportError(errmsg);
        }else{


            for(int x=0; x<80; x++){
                host_name[x] = *"";
                send_name[x] = *"";
            }
            gethostname(send_name, 79);
            write(out_sock, &send_name, sizeof(send_name));

            read(out_sock, &host_name, sizeof(host_name));


            QIP = QString::fromStdString(connect_to_ip);
            QhostName = QString::fromStdString(host_name);
            QsendName = QString::fromStdString(send_name);

            emit Credentials(QIP, QhostName, QsendName);
            emit Socks(conn_in_sock, out_sock);
            serv=false;
            Recieve_Message();

        }
}


void workThread::Recieve_Message(){


    for(int x=0; x<80; x++){
                recv_msg[x]= *"";
            }
    if(serv==true){

        read(conn_in_sock,&recv_msg,sizeof(recv_msg));

    }else{

        read(out_sock,&recv_msg,sizeof(recv_msg));

    }

    qMsg = QString::fromStdString(recv_msg);
    emit MsgChanged(qMsg);
    Recieve_Message();

}

void workThread::set_incoming(){

        in_sock = socket(AF_INET, SOCK_STREAM, 0);

        in_address.sin_family = AF_INET;
        in_address.sin_addr.s_addr= htonl(INADDR_ANY);
        in_address.sin_port = htons(9734);
        in_addr_len = sizeof(in_address);
        bind(in_sock, (struct sockaddr*)&in_address, in_addr_len);

}

void workThread::set_outgoing(){

        out_sock = socket(AF_INET, SOCK_STREAM, 0);

        out_address.sin_family = AF_INET;
        out_address.sin_addr.s_addr=inet_addr(connect_to_ip);
        out_address.sin_port = htons(9734);
        out_addr_len = sizeof(out_address);

}




void workThread::onConn_Type(QString Ctype, QString Cip){

    ConnType = Ctype;

    for(int x=0; x<15; x++){
        connect_to_ip[x]=*"";
    }
    QByteArray tmpip = Cip.toLocal8Bit();
    for(int x = 0; x<15; x++){
        if(tmpip.data()[x] != *""){
            connect_to_ip[x]=tmpip.data()[x];
        }
    }

}
