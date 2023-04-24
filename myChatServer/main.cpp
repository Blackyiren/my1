#include "myserver.h"
#include"dbManagerServer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
//    QSqlQuery query(DatabaseManager::instance().database());
//    query.prepare("select id from user_table");

//    query.exec();

//     if(query.next()){
//         int x=query.value(0).toInt() ;
//         qDebug()<<x;
//         qDebug()<<"验证成功";

//     }
//     else {qDebug()<<"验证失败";

//     }
    QApplication a(argc, argv);
    myServer w;
    w.show();
    return a.exec();
}
