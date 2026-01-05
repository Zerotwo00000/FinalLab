#include "idatabase.h"

IDatabase::IDatabase(QObject *parent)
    : QObject{parent}
{
    ininDatabase();
}

void IDatabase::ininDatabase()
{
    database=QSqlDatabase::addDatabase("QSQLITE");
    QString aFile="E:/qt/课程设计/LibraryManagementSystem.db";
    database.setDatabaseName(aFile);
    if(!database.open()){ //打开数据库
        qDebug() <<"failed to open database";
    }else
        qDebug() <<"open database is ok";
}

bool IDatabase::initBookModel()
{
    bookTabModel = new QSqlTableModel(this,database);
    bookTabModel->setTable("books");
    bookTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    bookTabModel->setSort(bookTabModel->fieldIndex("name"),Qt::AscendingOrder);
    if(!(bookTabModel->select()))
        return false;

    theBookSelection = new QItemSelectionModel(bookTabModel);
    return true;
}
