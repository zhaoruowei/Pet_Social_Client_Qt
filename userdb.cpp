#include "userdb.h"

UserDB::UserDB(QObject *parent)
    : QObject{parent}
{
    // init
    // create or open db
    createDB();
    createTable();
    queryTable();
}

void UserDB::createDB()
{
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        this->db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        this->db = QSqlDatabase::addDatabase("QSQLITE");
    }

#ifdef Q_OS_WIN
   this->db.setDatabaseName("pet_social.db");
#endif

#ifdef Q_OS_ANDROID
    QFile file("assets:/database/pet_social.db");
    file.setPermissions(QFile::ReadOwner|QFile::WriteOwner);
    this->db.setDatabaseName("pet_social.db");
#endif
    if (!this->db.open())
    {
        qDebug() << "db open error" << this->db.lastError();
    }
}

void UserDB::createTable()
{
    QSqlQuery query(this->db);
    QString str = QString("CREATE TABLE USERTOKEN ("
                          "UID INT PRIMARY KEY NOT NULL,"
                          "TOKEN TEXT NOT NULL);");
    query.exec(str);
}

void UserDB::queryTable()
{
    this->queryobj = new QSqlQuery(this->db);
    QString str = QString("SELECT * FROM USERTOKEN;");
    this->queryobj->exec(str);
}

// update token for refresh token
void UserDB::updateTable(int uid, QString token)
{
    QString str = QString("SELECT * FROM USERTOKEN "
                          "WHERE "
                          "UID = %1;").arg(uid);
    QSqlQuery query(str);
    if (query.first() != false)
    {
        str = QString("UPDATE USERTOKEN "
                      "SET "
                      "UID = %1, TOKEN = '%2' "
                      "WHERE "
                      "UID = %1;").arg(uid).arg(token);
        query.exec(str);
        // update query model
        queryTable();
    }
    else
    {
        qDebug() << "record not found";
    }
}

// delete token for log out
void UserDB::deleteTable(int uid)
{
    QString str = QString("SELECT * FROM USERTOKEN "
                          "WHERE "
                          "UID = %1;").arg(uid);
    QSqlQuery query(str);
    if (query.first() != false)
    {
        str = QString("DELETE FROM USERTOKEN "
                      "WHERE "
                      "UID = %1;").arg(uid);
        query.exec(str);
        // update query model
        queryTable();
    }
    else
    {
        qDebug() << "record not found";
    }
}

// insert token for log in
void UserDB::insertTable(int uid, QString token)
{
    QSqlQuery query(this->db);
    QString str = QString("INSERT INTO USERTOKEN "
                          "VALUES (%1, '%2');").arg(uid).arg(token);
    if (query.exec(str) == false)
    {
        qDebug() << "insert error" << str;
    }
    else
    {
        queryTable();
    }
}
