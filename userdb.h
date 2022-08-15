#ifndef USERDB_H
#define USERDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QFile>


class UserDB : public QObject
{
    Q_OBJECT
public:
    explicit UserDB(QObject *parent = nullptr);
    void createDB();
    void createTable();
    void queryTable();
    // update token for refresh token
    void updateTable(int uid, QString token);
    // delete token for log out
    void deleteTable(int uid);
    // insert token for log in
    void insertTable(int uid, QString token);

public:
    // maintain query model
    QSqlQuery *queryobj;

private:
    // maintain db
    QSqlDatabase db;
};

#endif // USERDB_H
