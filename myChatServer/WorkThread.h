#ifndef WORKTHREAD_H
#define WORKTHREAD_H
#include <QMainWindow>
#include <QWidget>
#include <iostream>
#include "winsock2.h"
#include "stdlib.h"
#include "stdio.h"
#include "string"
#include <QByteArray>
#include <QThread>
#include<QTcpServer>
#include<QTcpSocket>
#include<QSqlError>
class WorkThread : public QThread
{
    Q_OBJECT
public:
    WorkThread(QTcpSocket *socket, QObject *parent = nullptr)
        : QThread(parent), socket(socket)
    {
    }

protected:
    void run() override;
public slots:
    void readyRead();
private:
    QTcpSocket *socket;
};
#endif // WORKTHREAD_H
