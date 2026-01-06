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
    bookTabModel->setSort(bookTabModel->fieldIndex("isbn"),Qt::AscendingOrder);
    bookTabModel->setHeaderData(bookTabModel->fieldIndex("isbn"), Qt::Horizontal, "ISBN");
    bookTabModel->setHeaderData(bookTabModel->fieldIndex("title"), Qt::Horizontal, "书名");
    bookTabModel->setHeaderData(bookTabModel->fieldIndex("author"), Qt::Horizontal, "作者");
    bookTabModel->setHeaderData(bookTabModel->fieldIndex("publisher"), Qt::Horizontal, "出版社");
    bookTabModel->setHeaderData(bookTabModel->fieldIndex("publish_year"), Qt::Horizontal, "出版年份");
    bookTabModel->setHeaderData(bookTabModel->fieldIndex("category"), Qt::Horizontal, "分类");
    bookTabModel->setHeaderData(bookTabModel->fieldIndex("total_copies"), Qt::Horizontal, "总数量");
    bookTabModel->setHeaderData(bookTabModel->fieldIndex("available_copies"), Qt::Horizontal, "可借数量");
    bookTabModel->setHeaderData(bookTabModel->fieldIndex("location"), Qt::Horizontal, "位置");
    bookTabModel->setHeaderData(bookTabModel->fieldIndex("add_date"), Qt::Horizontal, "添加时间");
    if(!(bookTabModel->select()))
        return false;

    theBookSelection = new QItemSelectionModel(bookTabModel);
    return true;
}

bool IDatabase::queryBook(QString filter)
{
    bookTabModel->setFilter(filter);
    return bookTabModel->select();
}

// 删除当前选中的图书
bool IDatabase::deleteCurrentBook()
{

    // 检查是否有选中的行
    if (!theBookSelection || !theBookSelection->hasSelection()) {
        qDebug() << "没有选中的图书";
        return false;
    }
    // 获取当前选中的行
    QModelIndex currentBookIndex = theBookSelection->currentIndex();

    // 获取当前行的ISBN（主键）
    int currentBookRow = currentBookIndex.row();
    QModelIndex isbnIndex = bookTabModel->index(currentBookRow, 0);  // 第0列是isbn
    QString isbn = bookTabModel->data(isbnIndex).toString();

    // 1. 从数据库中删除记录
    QSqlQuery query;
    query.prepare("DELETE FROM books WHERE isbn = ?");
    query.addBindValue(isbn);

    if (!query.exec()) {
        qDebug() << "数据库删除失败:" << query.lastError().text();
        return false;
    }

    // 2. 从模型中删除行
    if (!bookTabModel->removeRow(currentBookRow)) {
        qDebug() << "从模型删除行失败";
        return false;
    }

    // 3. 提交更改
    if (!bookTabModel->submitAll()) {
        qDebug() << "提交更改失败:" << bookTabModel->lastError().text();
        return false;
    }

    // 4. 刷新模型
    if (!bookTabModel->select()) {
        qDebug() << "刷新模型失败";
        return false;
    }

    qDebug() << "图书删除成功，ISBN:" << isbn;
    return true;
}

bool IDatabase::submitBookEdit()
{
    return bookTabModel->submitAll();
}

void IDatabase::revertBookEdit()
{
    bookTabModel->revertAll();
}

bool IDatabase::initReaderModel()
{
    readerTabModel = new QSqlTableModel(this,database);
    readerTabModel->setTable("readers");
    readerTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    readerTabModel->setSort(readerTabModel->fieldIndex("reader_no"),Qt::AscendingOrder);
    readerTabModel->setHeaderData(readerTabModel->fieldIndex("reader_no"), Qt::Horizontal, "读者编号");
    readerTabModel->setHeaderData(readerTabModel->fieldIndex("name"), Qt::Horizontal, "姓名");
    readerTabModel->setHeaderData(readerTabModel->fieldIndex("phone"), Qt::Horizontal, "电话");
    readerTabModel->setHeaderData(readerTabModel->fieldIndex("email"), Qt::Horizontal, "邮箱");
    readerTabModel->setHeaderData(readerTabModel->fieldIndex("id_card"), Qt::Horizontal, "身份证号");
    readerTabModel->setHeaderData(readerTabModel->fieldIndex("max_borrow_limit"), Qt::Horizontal, "最大借书数");
    readerTabModel->setHeaderData(readerTabModel->fieldIndex("current_borrowed"), Qt::Horizontal, "当前借书数");
    readerTabModel->setHeaderData(readerTabModel->fieldIndex("status"), Qt::Horizontal, "状态");
    readerTabModel->setHeaderData(readerTabModel->fieldIndex("register_date"), Qt::Horizontal, "注册日期");
    if(!(readerTabModel->select()))
        return false;

    theReaderSelection = new QItemSelectionModel(readerTabModel);
    return true;
}

bool IDatabase::queryReader(QString filter)
{
    readerTabModel->setFilter(filter);
    return readerTabModel->select();
}


bool IDatabase::deleteCurrentReader()
{
    // 检查是否有选中的行
    if (!theReaderSelection || !theReaderSelection->hasSelection()) {
        qDebug() << "没有选中的读者";
        return false;
    }

    // 获取当前选中的行
    QModelIndex currentReaderIndex = theReaderSelection->currentIndex();
    if (!currentReaderIndex.isValid()) {
        qDebug() << "选中的索引无效";
        return false;
    }

    // 获取当前行的reader_no（主键）
    int currentReaderRow = currentReaderIndex.row();
    QModelIndex readernoIndex = readerTabModel->index(currentReaderRow, 0);  // 第0列是reader_no
    QString readerno = readerTabModel->data(readernoIndex).toString();

    if (readerno.isEmpty()) {
        qDebug() << "获取读者编号失败";
        return false;
    }

    // 1. 从数据库中删除记录
    QSqlQuery Readerquery;
    Readerquery.prepare("DELETE FROM readers WHERE reader_no = ?");
    Readerquery.addBindValue(readerno);

    if (!Readerquery.exec()) {
        qDebug() << "数据库删除失败:" << Readerquery.lastError().text();
        return false;
    }

    // 2. 从模型中删除行
    if (!readerTabModel->removeRow(currentReaderRow)) {
        qDebug() << "从模型删除行失败";
        return false;
    }

    // 3. 提交更改
    if (!readerTabModel->submitAll()) {
        qDebug() << "提交更改失败:" << readerTabModel->lastError().text();
        return false;
    }

    // 4. 刷新模型
    if (!readerTabModel->select()) {
        qDebug() << "刷新模型失败";
        return false;
    }

    qDebug() << "读者删除成功，读者编号:" << readerno;
    return true;
}

bool IDatabase::submitReaderEdit()
{
    return readerTabModel->submitAll();
}

void IDatabase::revertReaderEdit()
{
    readerTabModel->revertAll();
}

bool IDatabase::initBorrowModel()
{
    borrowTabModel = new QSqlTableModel(this,database);
    borrowTabModel->setTable("borrow_records");
    borrowTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    borrowTabModel->setSort(borrowTabModel->fieldIndex("reader_no"),Qt::AscendingOrder);
    borrowTabModel->setHeaderData(borrowTabModel->fieldIndex("reader_no"), Qt::Horizontal, "读者编号");
    borrowTabModel->setHeaderData(borrowTabModel->fieldIndex("isbn"), Qt::Horizontal, "ISBN");
    borrowTabModel->setHeaderData(borrowTabModel->fieldIndex("borrow_date"), Qt::Horizontal, "借阅日期");
    borrowTabModel->setHeaderData(borrowTabModel->fieldIndex("due_date"), Qt::Horizontal, "应还日期");
    borrowTabModel->setHeaderData(borrowTabModel->fieldIndex("return_date"), Qt::Horizontal, "归还日期");
    borrowTabModel->setHeaderData(borrowTabModel->fieldIndex("status"), Qt::Horizontal, "状态");
    borrowTabModel->setHeaderData(borrowTabModel->fieldIndex("is_returned"), Qt::Horizontal, "是否已归还");
    borrowTabModel->setHeaderData(borrowTabModel->fieldIndex("is_overdue"), Qt::Horizontal, "是否逾期");
    borrowTabModel->setHeaderData(borrowTabModel->fieldIndex("created_at"), Qt::Horizontal, "创建时间");
    borrowTabModel->setHeaderData(borrowTabModel->fieldIndex("updated_at"), Qt::Horizontal, "更新时间");
    if(!(borrowTabModel->select()))
        return false;

    theBorrowSelection = new QItemSelectionModel(borrowTabModel);
    return true;
}
