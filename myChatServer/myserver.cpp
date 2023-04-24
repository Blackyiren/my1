#include "myserver.h"
#include "ui_myserver.h"
#include"dbManagerServer.h"
//#pragma execution_character_set("utf-8")
Ui::myServer* mui;
QTcpSocket clientSocket[1024];

SOCKADDR_IN addr = {0};
int k =0;
myServer::myServer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::myServer)
{
    ui->setupUi(this);
    mui =ui;
    server = new QTcpServer(this);
    //thread = new WorkThread();
 connect(server, &QTcpServer::newConnection, this, &myServer::newConnection);
 //connect(this,&myServer::stopThreads, this, &ThreadController::stopThreads);
}
myServer::~myServer()
{
    delete ui;
        // 关闭server对象
        server->close();
        // 删除server对象
        delete server;
        // 遍历threads列表，关闭和删除所有线程对象
        for (auto thread : threads) {
            thread->quit();
            thread->wait();
            delete thread;
        }
}


void myServer::on_openPushButton_clicked(bool checked)
{
    if (server->isListening()) {
        // server is already listening
        return;
    }
    if (!server->listen(QHostAddress::LocalHost, 1234)) {
        qWarning() << "Unable to start the server:" << server->errorString();
        qDebug()<<server->serverPort();
        return;
    }
    qDebug()<<"go";


}


void myServer::newConnection(){
    QTcpSocket *socket = server->nextPendingConnection();
    qDebug()<<&socket;
    WorkThread *thread = new WorkThread(socket, this);
    socket->moveToThread(thread);
    connect(thread, &WorkThread::finished, thread, &WorkThread::deleteLater);
    connect(socket, &QTcpSocket::readyRead, thread, &WorkThread::readyRead);
    thread->start();
    threads.append(thread);
}
void WorkThread::readyRead(){
    qDebug()<<"readyRead!";
    //socket->waitForReadyRead();
    while(socket->bytesAvailable() > 0){
          QByteArray todata = socket->readAll();
          QString reply= myServer::WWD(socket,QString(todata));
          QByteArray datato =reply.toUtf8();
          socket->write(datato);
          socket->waitForBytesWritten();
          qDebug()<<todata;
      }
}
void WorkThread::run()
{
  QThread::run();
}

bool myServer::ifLogin(QString i2,QString i3){
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT name,password FROM user_table WHERE name = :name and password = :password");
    query.bindValue(":name",i2);
    query.bindValue(":password",i3);
    query.exec();
    if(query.next()){
        qDebug() << i2;
        qDebug() << i3;
        return true;
    }
    else{
        qDebug()<<"sadasdads";
        return false;
    }

}

QString myServer:: WWD(QTcpSocket *socket,QString i){
    //QSqlQuery query(DatabaseManager::instance().database());
    qDebug()<<i;
    QStringList list = i.split("\n"); //使用"\n"作为分隔符，把str分割成一个QStringList
    QString i1 = list[0];
    // qDebug()<<i1;
    if(i1 == "login"){
        QString name = list[1];
        QString password = list[2];
    if(ifLogin(name,password)){
        qDebug()<<"成功";
        return "login\n1";

    }else{
        qDebug()<<"失败";
        return "login\n0";
    }
}
    else if(i1 == "sign"){
        return "sign\n1";
}
    else if(i1 == "signMessage"){
        QString name = list[1];
        QString password = list[2];
        QString secondPas = list[3];
        if(isCunzai(name)){
            insertUserName(name,password,secondPas);
            return "signMessage\n1";
        }
        else{
            return "signMessage\n0";
        }
}
}
void myServer::on_closePushButton_clicked(bool checked)
{
    for (WorkThread *thread : threads) {
            thread->quit();
            thread->wait();
        }
        WSACleanup();
}
bool myServer::isCunzai( const QString &name){
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM user_table WHERE name = :name");
    query.bindValue(":name", name);
    query.exec();
    if (query.next()) {
        //QMessageBox::warning(this,"ERROR","用户名重复");
        qDebug()<<"用户名重复";
        return false;
    } else {
        //QMessageBox::warning(this,"ERROR","用户名");
        qDebug()<<"用户名不重复";
        return true;
    }
}
int myServer::searchId(){

    QSqlQuery query(DatabaseManager::instance().database());
    query.exec("select max(id) from user_table");
    if(query.next()){
    int maxId = query.value(0).toInt();
    qDebug()<<maxId;
    return maxId;
    }
    else{
        return -1;
    }
}
void myServer::insertUserName( const QString &name,const QString &password,const QString &secondPassword) {
    int id =searchId()+1;
    if (id == 0){
        qDebug()<<"报错了";
       //QMessageBox::warning(this,"ERROR","查询失void my败，请重试！！");
    }
    qDebug()<<id;

    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare("INSERT INTO user_table (id, name,password,secondPassword) VALUES (:id, :name,:password,:secondPassword)");

    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":password",password);
    query.bindValue(":secondPassword",secondPassword);
   //qDebug() << query.executedQuery();
    if(query.exec())
    {//QMessageBox::information(this,"提示","注册成功！",QMessageBox::Yes);
   // accept();
    }
    else {qDebug() << query.lastError().text();
       // QMessageBox::warning(this,"ERROR","注册失败，请重试！！");
    }
}
