#ifndef MYSERVER_H
#define MYSERVER_H

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
#include "WorkThread.h"
#pragma comment (lib, "ws2_32.lib")
#pragma execution_character_set("utf-8")
QT_BEGIN_NAMESPACE
namespace Ui { class myServer; }
QT_END_NAMESPACE
class WorkThread;
class myServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit myServer(QWidget *parent = nullptr);
    ~myServer();
    static bool ifLogin(QString,QString);
    static QString WWD(QTcpSocket*,QString);
    static bool isCunzai(const QString &name);
    static int searchId();
    static void insertUserName( const QString &name,const QString &password,const QString &secondPassword);
private slots:
    void on_openPushButton_clicked(bool checked);

    void on_closePushButton_clicked(bool checked);
public slots:
    void newConnection();
private:
    Ui::myServer *ui;
    QTcpServer *server;
    int isSavePas = 0;
    int isRemName = 0;
    QString name;
    QString password;
    QString newName;
    QString newPassword;
    QList<WorkThread*> threads;
    //WorkThread *thread;
};

#endif // MYSERVER_H
