#ifndef DBMANAGER_H
#define DBMANAGER_H

#include<QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include<QMessageBox>
#include <QSqlQuery>

class DatabaseManager {
public:
    static DatabaseManager& instance() {
        static DatabaseManager instance;
        return instance;
    }

    QSqlQuery executeQuery(const QString& query) {
            QSqlQuery q(db);
            q.exec(query);
            return q;
        }

    QSqlDatabase& database() {
        return db;
    }

private:
    QSqlDatabase db;

    DatabaseManager() {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("mychatServer.db");
        db.open();
    }

    ~DatabaseManager() {
        db.close();
    }

    DatabaseManager(const DatabaseManager&) = delete;
    void operator=(const DatabaseManager&) = delete;
};





#endif // DBMANAGER_H
