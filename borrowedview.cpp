#include "borrowedview.h"
#include "ui_borrowedview.h"
#include "idatabase.h"
#include <QCloseEvent>
#include <QDate>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QComboBox>

BorrowedView::BorrowedView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BorrowedView)
    , selectedIsbn("")
{
    ui->setupUi(this);

    // 设置窗口标志
    setWindowFlags(Qt::Window);

    // 设置关闭时自动删除
    setAttribute(Qt::WA_DeleteOnClose);

    // 设置初始日期为当前日期
    QDate currentDate = QDate::currentDate();
    ui->dateEditBorrowDate->setDate(currentDate);
    ui->dateEditDueDate->setDate(currentDate.addDays(30));  // 默认借阅30天

    // 初始化下拉框
    initComboBox();

    // 连接按钮信号
    connect(ui->btnBorrowedOK, &QPushButton::clicked, this, &BorrowedView::on_btnBorrowedOK_clicked);
    connect(ui->btnBorrowedCancel, &QPushButton::clicked, this, &BorrowedView::on_btnBorrowedCancel_clicked);

    // 连接下拉框信号
    connect(ui->comboBoxBorrow, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BorrowedView::on_comboBoxBorrow_currentIndexChanged);
}

BorrowedView::~BorrowedView()
{
    delete ui;
}

void BorrowedView::initComboBox()
{
    ui->comboBoxBorrow->clear();

    // 查询可借阅的书籍（available_copies > 0）
    QSqlQuery query;
    QString sql = "SELECT isbn, title, available_copies FROM books WHERE available_copies > 0 ORDER BY title";

    if (!query.exec(sql)) {
        qDebug() << "查询可借阅书籍失败:" << query.lastError().text();
        return;
    }

    int count = 0;
    while (query.next()) {
        QString isbn = query.value(0).toString();
        QString title = query.value(1).toString();
        int available = query.value(2).toInt();

        // 将信息合并为显示字符串
        QString displayText = QString("%1 - %2 (可借: %3)").arg(isbn, title).arg(available);

        // 将完整的ISBN作为item的data存储
        ui->comboBoxBorrow->addItem(displayText, isbn);
        count++;
    }

    if (count == 0) {
        ui->comboBoxBorrow->addItem("没有可借阅的书籍", "");
    }

    qDebug() << "加载了" << count << "本可借阅的书籍";
}

void BorrowedView::on_comboBoxBorrow_currentIndexChanged(int index)
{
    if (index < 0) return;

    // 获取选择的ISBN
    selectedIsbn = ui->comboBoxBorrow->itemData(index).toString();

    if (!selectedIsbn.isEmpty()) {
        // 显示书籍信息
        showBookInfo(selectedIsbn);
    }
}

void BorrowedView::showBookInfo(const QString& isbn)
{
    QSqlQuery query;
    query.prepare("SELECT title, author, publisher, total_copies, available_copies FROM books WHERE isbn = ?");
    query.addBindValue(isbn);

    if (query.exec() && query.next()) {
        QString title = query.value(0).toString();
        QString author = query.value(1).toString();
        QString publisher = query.value(2).toString();
        int total = query.value(3).toInt();
        int available = query.value(4).toInt();

        QString message = QString("您选择的书籍信息如下：\n\n"
                                  "ISBN: %1\n"
                                  "书名: %2\n"
                                  "作者: %3\n"
                                  "出版社: %4\n"
                                  "总数量: %5\n"
                                  "可借数量: %6")
                              .arg(isbn, title, author, publisher)
                              .arg(total).arg(available);

        QMessageBox::information(this, "书籍信息", message);
    } else {
        QMessageBox::warning(this, "错误", "获取书籍信息失败！");
    }
}

bool BorrowedView::validateBorrowCondition(const QString& readerNo, const QString& isbn)
{
    // 检查读者是否存在
    QSqlQuery readerQuery;
    readerQuery.prepare("SELECT reader_no, name, current_borrowed, max_borrow_limit, status FROM readers WHERE reader_no = ?");
    readerQuery.addBindValue(readerNo);

    if (!readerQuery.exec() || !readerQuery.next()) {
        QMessageBox::warning(this, "错误", "读者不存在！");
        return false;
    }

    QString readerName = readerQuery.value("name").toString();
    int currentBorrowed = readerQuery.value("current_borrowed").toInt();
    int maxBorrowLimit = readerQuery.value("max_borrow_limit").toInt();
    QString status = readerQuery.value("status").toString();

    // 检查读者状态
    if (status != "正常") {
        QMessageBox::warning(this, "警告", QString("读者【%1】状态异常，无法借书！").arg(readerName));
        return false;
    }

    // 检查读者是否达到最大借阅数
    if (currentBorrowed >= maxBorrowLimit) {
        QMessageBox::warning(this, "警告",
                             QString("读者【%1】已达到最大借阅数！\n当前借阅：%2，最大可借：%3")
                                 .arg(readerName).arg(currentBorrowed).arg(maxBorrowLimit));
        return false;
    }

    // 检查书籍是否可借
    QSqlQuery bookQuery;
    bookQuery.prepare("SELECT title, available_copies FROM books WHERE isbn = ?");
    bookQuery.addBindValue(isbn);

    if (!bookQuery.exec() || !bookQuery.next()) {
        QMessageBox::warning(this, "错误", "书籍不存在！");
        return false;
    }

    QString bookTitle = bookQuery.value("title").toString();
    int availableCopies = bookQuery.value("available_copies").toInt();

    if (availableCopies <= 0) {
        QMessageBox::warning(this, "警告", QString("书籍【%1】已全部借出！").arg(bookTitle));
        return false;
    }

    return true;
}

void BorrowedView::on_btnBorrowedOK_clicked()
{
    // 获取表单数据
    QString readerNo = ui->lineEditBorrowedReaderNo->text().trimmed();

    if (readerNo.isEmpty()) {
        QMessageBox::warning(this, "警告", "读者编号不能为空！");
        ui->lineEditBorrowedReaderNo->setFocus();
        return;
    }

    int bookIndex = ui->comboBoxBorrow->currentIndex();

    if (bookIndex < 0 || ui->comboBoxBorrow->itemData(bookIndex).toString().isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要借阅的书籍！");
        ui->comboBoxBorrow->setFocus();
        return;
    }

    QString isbn = ui->comboBoxBorrow->itemData(bookIndex).toString();
    QDate borrowDate = ui->dateEditBorrowDate->date();
    QDate dueDate = ui->dateEditDueDate->date();

    // 检查借阅日期是否合理
    if (borrowDate > dueDate) {
        QMessageBox::warning(this, "警告", "借阅日期不能晚于应还日期！");
        return;
    }

    // 验证借阅条件
    if (!validateBorrowCondition(readerNo, isbn)) {
        return;
    }

    // 开始事务
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        QMessageBox::warning(this, "错误", "开始事务失败！");
        return;
    }

    try {
        // 插入借阅记录
        QSqlQuery insertQuery;
        insertQuery.prepare(
            "INSERT INTO borrow_records (reader_no, isbn, borrow_date, due_date, "
            "is_returned, is_overdue, created_at, updated_at) "
            "VALUES (?, ?, ?, ?, 0, 0, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)"
            );
        insertQuery.addBindValue(readerNo);
        insertQuery.addBindValue(isbn);
        insertQuery.addBindValue(borrowDate.toString("yyyy-MM-dd"));
        insertQuery.addBindValue(dueDate.toString("yyyy-MM-dd"));

        if (!insertQuery.exec()) {
            // 检查是否是触发器抛出的错误
            QString error = insertQuery.lastError().text();
            if (error.contains("读者已达到最大借阅限制") ||
                error.contains("图书已全部借出") ||
                error.contains("读者状态异常，无法借书")) {
                QMessageBox::warning(this, "借阅失败", error);
            } else {
                QMessageBox::warning(this, "错误", QString("插入借阅记录失败：%1").arg(error));
            }
            db.rollback();
            return;
        }

        // 提交事务
        if (!db.commit()) {
            QMessageBox::warning(this, "错误", "提交事务失败！");
            db.rollback();
            return;
        }

        QMessageBox::information(this, "成功", "借阅成功！");
        emit dataUpdated();
        close();

    } catch (...) {
        db.rollback();
        QMessageBox::warning(this, "错误", "借阅过程中发生异常！");
    }
}

void BorrowedView::on_btnBorrowedCancel_clicked()
{
    close();
}

void BorrowedView::closeEvent(QCloseEvent *event)
{
    emit dialogClosed();
    QWidget::closeEvent(event);
}
