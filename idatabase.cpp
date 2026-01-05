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
    bookTabModel->setSort(bookTabModel->fieldIndex("book_id"),Qt::AscendingOrder);
    if(!(bookTabModel->select()))
        return false;

    theBookSelection = new QItemSelectionModel(bookTabModel);
    return true;
}

bool IDatabase::initReaderModel()
{
    readerTabModel = new QSqlTableModel(this,database);
    readerTabModel->setTable("readers");
    readerTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    readerTabModel->setSort(readerTabModel->fieldIndex("reader_id"),Qt::AscendingOrder);
    if(!(readerTabModel->select()))
        return false;

    theReaderSelection = new QItemSelectionModel(readerTabModel);
    return true;
}

bool IDatabase::initBorrowModel()
{
    borrowTabModel = new QSqlTableModel(this,database);
    borrowTabModel->setTable("borrow_records");
    borrowTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    borrowTabModel->setSort(borrowTabModel->fieldIndex("borrow_id"),Qt::AscendingOrder);
    if(!(borrowTabModel->select()))
        return false;

    theBorrowSelection = new QItemSelectionModel(borrowTabModel);
    return true;
}
