#include "borrowedview.h"
#include "ui_borrowedview.h"
#include "idatabase.h"
#include <QCloseEvent>
#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

BorrowedView::BorrowedView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BorrowedView)
{
    ui->setupUi(this);

    // 设置窗口标志，使其成为一个独立的窗口
    setWindowFlags(Qt::Window);

    // 设置初始日期为当前日期
    QDate currentDate = QDate::currentDate();
    ui->dateEditBorrowDate->setDate(currentDate);
    ui->dateEditDueDate->setDate(currentDate.addDays(30));  // 默认借阅30天

    // 加载可借阅书籍到下拉框
    loadAvailableBooks();

    // 连接组合框选择变化的信号
    connect(ui->comboBoxBorrow, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BorrowedView::onBookSelected);

    // 连接按钮信号
    connect(ui->btnBorrowedOK, &QPushButton::clicked, this, &BorrowedView::on_btnBorrowedOK_clicked);
    connect(ui->btnBorrowedCancel, &QPushButton::clicked, this, &BorrowedView::on_btnBorrowedCancel_clicked);
}

BorrowedView::~BorrowedView()
{
    delete ui;
}

// 加载可借阅的书籍
void BorrowedView::loadAvailableBooks()
{
    ui->comboBoxBorrow->clear();

    QSqlQuery query;
    query.prepare(
        "SELECT isbn, title, author, available_copies "
        "FROM books "
        "WHERE available_copies > 0 "
        "ORDER BY isbn"
        );

    if (!query.exec()) {
        qDebug() << "查询可借阅书籍失败:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString isbn = query.value(0).toString();
        QString title = query.value(1).toString();
        QString author = query.value(2).toString();
        int availableCopies = query.value(3).toInt();

        // 显示格式：ISBN - 书名 (作者) [可借: 数量]
        QString displayText = QString("%1 - 《%2》 - %3 [可借: %4本]")
                                  .arg(isbn, title, author, QString::number(availableCopies));

        ui->comboBoxBorrow->addItem(displayText, isbn);
    }

    qDebug() << "加载了" << ui->comboBoxBorrow->count() << "本可借阅书籍";
}

// 书籍被选中时显示详细信息
void BorrowedView::onBookSelected(int index)
{
    if (index < 0) return;

    QString isbn = ui->comboBoxBorrow->itemData(index).toString();
    if (isbn.isEmpty()) return;

    QSqlQuery query;
    query.prepare(
        "SELECT isbn, title, author, publisher, publish_year, category, "
        "total_copies, available_copies, location "
        "FROM books "
        "WHERE isbn = ?"
        );
    query.addBindValue(isbn);

    if (!query.exec() || !query.next()) {
        qDebug() << "查询书籍详细信息失败";
        return;
    }

    qDebug() << "\n=== 选择的书籍详细信息 ===";
    qDebug() << "ISBN:" << query.value(0).toString();
    qDebug() << "书名:" << query.value(1).toString();
    qDebug() << "作者:" << query.value(2).toString();
    qDebug() << "出版社:" << query.value(3).toString();
    qDebug() << "出版年份:" << query.value(4).toString();
    qDebug() << "分类:" << query.value(5).toString();
    qDebug() << "总数量:" << query.value(6).toString();
    qDebug() << "可借数量:" << query.value(7).toString();
    qDebug() << "位置:" << query.value(8).toString();
    qDebug() << "=======================\n";
}

// 检查读者状态
bool BorrowedView::checkReaderStatus(const QString& readerNo)
{
    QSqlQuery query;
    query.prepare(
        "SELECT status, current_borrowed, max_borrow_limit "
        "FROM readers "
        "WHERE reader_no = ?"
        );
    query.addBindValue(readerNo);

    if (!query.exec() || !query.next()) {
        qDebug() << "读者编号不存在";
        return false;
    }

    QString status = query.value(0).toString();
    int currentBorrowed = query.value(1).toInt();
    int maxBorrowLimit = query.value(2).toInt();

    qDebug() << "读者信息:";
    qDebug() << "  状态:" << status;
    qDebug() << "  当前借阅数:" << currentBorrowed;
    qDebug() << "  最大借阅数:" << maxBorrowLimit;

    if (status != "正常") {
        qDebug() << "读者状态异常，无法借书";
        return false;
    }

    if (currentBorrowed >= maxBorrowLimit) {
        qDebug() << "读者已达到最大借阅限制";
        return false;
    }

    return true;
}

// 检查是否今天已借阅过同一本书
bool BorrowedView::checkTodayBorrowed(const QString& readerNo, const QString& isbn, const QDate& borrowDate)
{
    QSqlQuery query;
    query.prepare(
        "SELECT COUNT(*) FROM borrow_records "
        "WHERE reader_no = ? AND isbn = ? AND date(borrow_date) = date(?) AND is_returned = 0"
        );
    query.addBindValue(readerNo);
    query.addBindValue(isbn);
    query.addBindValue(borrowDate.toString("yyyy-MM-dd"));

    if (!query.exec() || !query.next()) {
        return false;
    }

    int count = query.value(0).toInt();
    if (count > 0) {
        qDebug() << "今天已借阅过此书，请勿重复借阅";
        return true;
    }

    return false;
}

// 确定按钮点击
void BorrowedView::on_btnBorrowedOK_clicked()
{
    // 获取表单数据
    QString readerNo = ui->lineEditBorrowedReaderNo->text().trimmed();
    int bookIndex = ui->comboBoxBorrow->currentIndex();

    if (bookIndex < 0) {
        qDebug() << "请选择一本书籍";
        return;
    }

    QString isbn = ui->comboBoxBorrow->itemData(bookIndex).toString();
    QDate borrowDate = ui->dateEditBorrowDate->date();
    QDate dueDate = ui->dateEditDueDate->date();

    // 简单的表单验证
    if (readerNo.isEmpty() || isbn.isEmpty()) {
        qDebug() << "读者编号和书籍不能为空";
        return;
    }

    if (borrowDate > dueDate) {
        qDebug() << "借阅日期不能晚于应还日期";
        return;
    }

    qDebug() << "\n=== 开始借阅处理 ===";
    qDebug() << "读者编号:" << readerNo;
    qDebug() << "ISBN:" << isbn;
    qDebug() << "借阅日期:" << borrowDate.toString("yyyy-MM-dd");
    qDebug() << "应还日期:" << dueDate.toString("yyyy-MM-dd");

    // 检查读者状态
    if (!checkReaderStatus(readerNo)) {
        return;
    }

    // 检查是否今天已借阅过同一本书
    if (checkTodayBorrowed(readerNo, isbn, borrowDate)) {
        return;
    }

    // 检查图书是否可借
    QSqlQuery bookQuery;
    bookQuery.prepare("SELECT available_copies FROM books WHERE isbn = ?");
    bookQuery.addBindValue(isbn);

    if (!bookQuery.exec() || !bookQuery.next()) {
        qDebug() << "查询图书信息失败";
        return;
    }

    int availableCopies = bookQuery.value(0).toInt();
    if (availableCopies <= 0) {
        qDebug() << "图书已全部借出";
        return;
    }

    // 开始事务
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        qDebug() << "开始事务失败";
        return;
    }

    try {
        // 插入借阅记录
        QSqlQuery insertQuery;
        insertQuery.prepare(
            "INSERT INTO borrow_records (reader_no, isbn, borrow_date, due_date) "
            "VALUES (?, ?, ?, ?)"
            );
        insertQuery.addBindValue(readerNo);
        insertQuery.addBindValue(isbn);
        insertQuery.addBindValue(borrowDate.toString("yyyy-MM-dd"));
        insertQuery.addBindValue(dueDate.toString("yyyy-MM-dd"));

        if (!insertQuery.exec()) {
            QString error = insertQuery.lastError().text();
            db.rollback();
            qDebug() << "借阅失败:" << error;
            return;
        }

        if (!db.commit()) {
            db.rollback();
            qDebug() << "提交事务失败";
            return;
        }

        qDebug() << "借阅成功";

        // 刷新模型
        emit dataUpdated();

        // 清空表单
        ui->lineEditBorrowedReaderNo->clear();

        // 重新加载可借阅书籍
        loadAvailableBooks();

        close();

    } catch (...) {
        db.rollback();
        qDebug() << "借阅过程中发生异常";
    }
}

// 取消按钮点击
void BorrowedView::on_btnBorrowedCancel_clicked()
{
    close();
}

void BorrowedView::closeEvent(QCloseEvent *event)
{
    emit dialogClosed();
    QWidget::closeEvent(event);
}
