#include "idatabase.h"

IDatabase::IDatabase(QObject *parent)
    : QObject{parent}
{
    ininDatabase();
}

void IDatabase::ininDatabase()
{
    database=QSqlDatabase::addDatabase("QSQLITE");
    QString aFile="E:/qt/lab/LibraryManagementSystem/LibraryManagementSystem.db";
    database.setDatabaseName(aFile);
    if(!database.open()){ //打开数据库
        qDebug() <<"failed to open database";
    }else
        qDebug() <<"open database is ok";
}
