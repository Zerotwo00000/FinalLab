#include "masterview.h"
#include "ui_masterview.h"
#include "bookeditview.h"
#include "readereditview.h"
#include "borrowedview.h"
#include "idatabase.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDate>
#include <QDateTime>
#include <QStandardPaths>

MasterView::MasterView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MasterView)
{
    ui->setupUi(this);

    // 设置窗口标题
    setWindowTitle("图书借阅管理系统");

    // 设置窗口大小
    resize(1000, 700);

    // 设置表格视图属性
    setupTableViews();

    // 设置导航功能
    setupNavigation();

    // 连接信号槽
    setupConnections();

    // 设置数据库模型
    setupDatabaseModels();
}

MasterView::~MasterView()
{
    delete ui;
}

void MasterView::setupTableViews()
{
    // 图书表格设置
    ui->BooktableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->BooktableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->BooktableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->BooktableView->setAlternatingRowColors(true);
    ui->BooktableView->horizontalHeader()->setStretchLastSection(true);

    // 读者表格设置
    ui->ReadertableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ReadertableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->ReadertableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->ReadertableView->setAlternatingRowColors(true);
    ui->ReadertableView->horizontalHeader()->setStretchLastSection(true);

    // 借阅表格设置
    ui->BorrowtableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->BorrowtableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->BorrowtableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->BorrowtableView->setAlternatingRowColors(true);
    ui->BorrowtableView->horizontalHeader()->setStretchLastSection(true);
}

void MasterView::setupDatabaseModels()
{
    IDatabase &iDatabase = IDatabase::getInstance();

    // 初始化图书模型
    if (iDatabase.initBookModel()){
        ui->BooktableView->setModel(iDatabase.bookTabModel);
        ui->BooktableView->setSelectionModel(iDatabase.theBookSelection);
    }

    // 初始化读者模型
    if (iDatabase.initReaderModel()){
        ui->ReadertableView->setModel(iDatabase.readerTabModel);
        ui->ReadertableView->setSelectionModel(iDatabase.theReaderSelection);
    }

    // 初始化借阅模型
    if (iDatabase.initBorrowModel()){
        ui->BorrowtableView->setModel(iDatabase.borrowTabModel);
        ui->BorrowtableView->setSelectionModel(iDatabase.theBorrowSelection);
    }
}

void MasterView::setupNavigation()
{
    // 设置左侧导航按钮为可选状态
    ui->btnBookManage->setCheckable(true);
    ui->btnReaderManage->setCheckable(true);
    ui->btnBorrowManage->setCheckable(true);
    ui->btnReportManage->setCheckable(true);

    // 默认选中图书管理
    ui->btnBookManage->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
}

void MasterView::setupConnections()
{
    // 连接导航按钮
    connect(ui->btnBookManage, &QPushButton::clicked, this, &MasterView::on_btnBookManage_clicked);
    connect(ui->btnReaderManage, &QPushButton::clicked, this, &MasterView::on_btnReaderManage_clicked);
    connect(ui->btnBorrowManage, &QPushButton::clicked, this, &MasterView::on_btnBorrowManage_clicked);
    connect(ui->btnReportManage, &QPushButton::clicked, this, &MasterView::on_btnReportManage_clicked);

    // 连接图书管理按钮
    connect(ui->btnBookAdd, &QPushButton::clicked, this, &MasterView::on_btnBookAdd_clicked);
    connect(ui->btnBookModify, &QPushButton::clicked, this, &MasterView::on_btnBookModify_clicked);
    connect(ui->btnBookQuery, &QPushButton::clicked, this, &MasterView::on_btnBookQuery_clicked);
    connect(ui->btnBookDelete, &QPushButton::clicked, this, &MasterView::on_btnBookDelete_clicked);

    // 连接读者管理按钮
    connect(ui->btnReaderAdd, &QPushButton::clicked, this, &MasterView::on_btnReaderAdd_clicked);
    connect(ui->btnReaderModify, &QPushButton::clicked, this, &MasterView::on_btnReaderModify_clicked);
    connect(ui->btnReaderQuery, &QPushButton::clicked, this, &MasterView::on_btnReaderQuery_clicked);
    connect(ui->btnReaderDelete, &QPushButton::clicked, this, &MasterView::on_btnReaderDelete_clicked);

    // 连接借阅管理按钮
    connect(ui->btnBorrowed, &QPushButton::clicked, this, &MasterView::on_btnBorrowed_clicked);
    connect(ui->btnBorrowQuery, &QPushButton::clicked, this, &MasterView::on_btnBorrowQuery_clicked);
    connect(ui->btnBorrowDelete, &QPushButton::clicked, this, &MasterView::on_btnBorrowDelete_clicked);
    connect(ui->btnBorrowReturn, &QPushButton::clicked, this, &MasterView::on_btnBorrowReturn_clicked);

    // 连接报表管理按钮
    connect(ui->btnReportQuery, &QPushButton::clicked, this, &MasterView::on_btnReportQuery_clicked);
    connect(ui->btnExport, &QPushButton::clicked, this, &MasterView::exportDataClicked);

    // 连接退出系统按钮
    connect(ui->btnExitSystem, &QPushButton::clicked, this, &MasterView::on_btnExitSystem_clicked);

    qDebug() << "信号槽连接完成，检查报表按钮状态：";
    qDebug() << "查询统计按钮：" << (ui->btnReportQuery ? "存在" : "不存在");
    qDebug() << "导出数据按钮：" << (ui->btnExport ? "存在" : "不存在");
}

// 导航按钮点击
void MasterView::on_btnBookManage_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->btnBookManage->setChecked(true);
    ui->btnReaderManage->setChecked(false);
    ui->btnBorrowManage->setChecked(false);
    ui->btnReportManage->setChecked(false);
}

void MasterView::on_btnReaderManage_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->btnBookManage->setChecked(false);
    ui->btnReaderManage->setChecked(true);
    ui->btnBorrowManage->setChecked(false);
    ui->btnReportManage->setChecked(false);
}

void MasterView::on_btnBorrowManage_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->btnBookManage->setChecked(false);
    ui->btnReaderManage->setChecked(false);
    ui->btnBorrowManage->setChecked(true);
    ui->btnReportManage->setChecked(false);
}

void MasterView::on_btnReportManage_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->btnBookManage->setChecked(false);
    ui->btnReaderManage->setChecked(false);
    ui->btnBorrowManage->setChecked(false);
    ui->btnReportManage->setChecked(true);

    // 切换到报表页面时自动生成统计报表
    generateReport();
}

// 添加图书按钮点击
void MasterView::on_btnBookAdd_clicked()
{
    qDebug() << "=== 添加图书按钮点击 ===";

    // 检查是否已经有对话框打开
    if (bookEditDialog != nullptr && bookEditDialog->isVisible()) {
        qDebug() << "对话框已存在，激活窗口";
        bookEditDialog->activateWindow();
        bookEditDialog->raise();
        return;
    }

    // 如果指针不为空但窗口不可见，删除旧指针
    if (bookEditDialog != nullptr) {
        qDebug() << "删除旧的对话框指针";
        bookEditDialog = nullptr;
    }

    // 创建新的对话框
    qDebug() << "创建新的添加图书对话框";
    bookEditDialog = new BookEditView(this, -1, BookEditView::Add);

    // 设置窗口关闭时自动删除
    bookEditDialog->setAttribute(Qt::WA_DeleteOnClose);

    // 连接数据更新信号
    connect(bookEditDialog, &BookEditView::dataUpdated, this, [this]() {
        qDebug() << "图书数据更新，刷新模型";
        IDatabase::getInstance().bookTabModel->select();
    });

    // 连接对话框关闭信号
    connect(bookEditDialog, &BookEditView::destroyed, this, [this]() {
        qDebug() << "对话框被销毁";
        bookEditDialog = nullptr;
    });

    // 显示对话框
    bookEditDialog->show();
    bookEditDialog->raise();
    bookEditDialog->activateWindow();
    qDebug() << "=== 添加图书按钮处理完成 ===";
}

// 修改图书按钮点击
void MasterView::on_btnBookModify_clicked()
{
    qDebug() << "=== 修改图书按钮点击 ===";

    // 检查是否已经有对话框打开
    if (bookEditDialog != nullptr && bookEditDialog->isVisible()) {
        qDebug() << "对话框已存在，激活窗口";
        bookEditDialog->activateWindow();
        bookEditDialog->raise();
        return;
    }

    // 检查是否有选中的行
    if (!IDatabase::getInstance().theBookSelection ||
        !IDatabase::getInstance().theBookSelection->hasSelection()) {
        qDebug() << "没有选中的图书";
        return;
    }

    // 获取当前选中的行索引
    QModelIndex currentIndex = IDatabase::getInstance().theBookSelection->currentIndex();
    if (!currentIndex.isValid()) {
        qDebug() << "选中的索引无效";
        return;
    }

    int currentRow = currentIndex.row();
    qDebug() << "修改图书，行:" << currentRow;

    // 如果指针不为空但窗口不可见，删除旧指针
    if (bookEditDialog != nullptr) {
        qDebug() << "删除旧的对话框指针";
        bookEditDialog = nullptr;
    }

    // 创建新的对话框
    qDebug() << "创建新的修改图书对话框";
    bookEditDialog = new BookEditView(this, currentRow, BookEditView::Edit);

    // 设置窗口关闭时自动删除
    bookEditDialog->setAttribute(Qt::WA_DeleteOnClose);

    // 连接数据更新信号
    connect(bookEditDialog, &BookEditView::dataUpdated, this, [this]() {
        qDebug() << "图书数据更新，刷新模型";
        IDatabase::getInstance().bookTabModel->select();
    });

    // 连接对话框关闭信号
    connect(bookEditDialog, &BookEditView::destroyed, this, [this]() {
        qDebug() << "对话框被销毁";
        bookEditDialog = nullptr;
    });

    // 显示对话框
    bookEditDialog->show();
    bookEditDialog->raise();
    bookEditDialog->activateWindow();
    qDebug() << "=== 修改图书按钮处理完成 ===";
}

// 查询图书按钮点击
void MasterView::on_btnBookQuery_clicked()
{
    QString searchBookText = ui->BookSearchlineEdit->text().trimmed();

    if (searchBookText.isEmpty()) {
        IDatabase::getInstance().bookTabModel->setFilter("");
        IDatabase::getInstance().bookTabModel->select();
        return;
    }

    QString Bookfilter = QString("title LIKE '%%1%' OR "
                                 "author LIKE '%%1%' OR "
                                 "publisher LIKE '%%1%' OR "
                                 "isbn LIKE '%%1%' OR "
                                 "category LIKE '%%1%'")
                             .arg(searchBookText);

    IDatabase::getInstance().queryBook(Bookfilter);
}

// 删除图书按钮点击
void MasterView::on_btnBookDelete_clicked()
{
    if (!IDatabase::getInstance().theBookSelection ||
        !IDatabase::getInstance().theBookSelection->hasSelection()) {
        return;
    }

    IDatabase::getInstance().deleteCurrentBook();
}

// 添加读者按钮点击
void MasterView::on_btnReaderAdd_clicked()
{
    qDebug() << "=== 添加读者按钮点击 ===";

    // 检查是否已经有对话框打开
    if (readerEditDialog != nullptr && readerEditDialog->isVisible()) {
        qDebug() << "读者对话框已存在，激活窗口";
        readerEditDialog->activateWindow();
        readerEditDialog->raise();
        return;
    }

    // 如果指针不为空但窗口不可见，删除旧指针
    if (readerEditDialog != nullptr) {
        qDebug() << "删除旧的读者对话框指针";
        readerEditDialog = nullptr;
    }

    // 创建新的对话框
    qDebug() << "创建新的添加读者对话框";
    readerEditDialog = new ReaderEditView(this, -1, ReaderEditView::Add);

    // 设置窗口关闭时自动删除
    readerEditDialog->setAttribute(Qt::WA_DeleteOnClose);

    // 连接数据更新信号
    connect(readerEditDialog, &ReaderEditView::dataUpdated, this, [this]() {
        qDebug() << "读者数据更新，刷新模型";
        IDatabase::getInstance().readerTabModel->select();
    });

    // 连接对话框关闭信号
    connect(readerEditDialog, &ReaderEditView::dialogClosed, this, [this]() {
        qDebug() << "读者对话框关闭信号收到";
        readerEditDialog = nullptr;
    });

    // 连接对话框销毁信号
    connect(readerEditDialog, &ReaderEditView::destroyed, this, [this]() {
        qDebug() << "读者对话框被销毁";
        readerEditDialog = nullptr;
    });

    // 显示对话框
    readerEditDialog->show();
    readerEditDialog->raise();
    readerEditDialog->activateWindow();
    qDebug() << "=== 添加读者按钮处理完成 ===";
}

// 修改读者按钮点击
void MasterView::on_btnReaderModify_clicked()
{
    qDebug() << "=== 修改读者按钮点击 ===";

    // 检查是否已经有对话框打开
    if (readerEditDialog != nullptr && readerEditDialog->isVisible()) {
        qDebug() << "读者对话框已存在，激活窗口";
        readerEditDialog->activateWindow();
        readerEditDialog->raise();
        return;
    }

    // 检查是否有选中的行
    if (!IDatabase::getInstance().theReaderSelection ||
        !IDatabase::getInstance().theReaderSelection->hasSelection()) {
        qDebug() << "没有选中的读者";
        return;
    }

    // 获取当前选中的行索引
    QModelIndex currentIndex = IDatabase::getInstance().theReaderSelection->currentIndex();
    if (!currentIndex.isValid()) {
        qDebug() << "选中的索引无效";
        return;
    }

    int currentRow = currentIndex.row();
    qDebug() << "修改读者，行:" << currentRow;

    // 获取读者编号用于调试
    QString readerNo = IDatabase::getInstance().readerTabModel->data(
                                                                  IDatabase::getInstance().readerTabModel->index(currentRow, 0)
                                                                  ).toString();
    qDebug() << "读者编号:" << readerNo;

    // 如果指针不为空但窗口不可见，删除旧指针
    if (readerEditDialog != nullptr) {
        qDebug() << "删除旧的读者对话框指针";
        readerEditDialog = nullptr;
    }

    // 创建新的对话框
    qDebug() << "创建新的修改读者对话框";
    readerEditDialog = new ReaderEditView(this, currentRow, ReaderEditView::Edit);

    // 设置窗口关闭时自动删除
    readerEditDialog->setAttribute(Qt::WA_DeleteOnClose);

    // 连接数据更新信号
    connect(readerEditDialog, &ReaderEditView::dataUpdated, this, [this]() {
        qDebug() << "读者数据更新，刷新模型";
        IDatabase::getInstance().readerTabModel->select();
    });

    // 连接对话框关闭信号
    connect(readerEditDialog, &ReaderEditView::dialogClosed, this, [this]() {
        qDebug() << "读者对话框关闭信号收到";
        readerEditDialog = nullptr;
    });

    // 连接对话框销毁信号
    connect(readerEditDialog, &ReaderEditView::destroyed, this, [this]() {
        qDebug() << "读者对话框被销毁";
        readerEditDialog = nullptr;
    });

    // 显示对话框
    readerEditDialog->show();
    readerEditDialog->raise();
    readerEditDialog->activateWindow();
    qDebug() << "=== 修改读者按钮处理完成 ===";
}

// 查询读者按钮点击
void MasterView::on_btnReaderQuery_clicked()
{
    QString searchReaderText = ui->ReaderSearchlineEdit->text().trimmed();

    if (searchReaderText.isEmpty()) {
        IDatabase::getInstance().readerTabModel->setFilter("");
        IDatabase::getInstance().readerTabModel->select();
        return;
    }

    QString Readerfilter = QString("name LIKE '%%1%' OR "
                                   "phone LIKE '%%1%' OR "
                                   "email LIKE '%%1%' OR "
                                   "reader_no LIKE '%%1%' OR "
                                   "id_card LIKE '%%1%'")
                               .arg(searchReaderText);

    IDatabase::getInstance().queryReader(Readerfilter);
}

// 删除读者按钮点击
void MasterView::on_btnReaderDelete_clicked()
{
    if (!IDatabase::getInstance().theReaderSelection ||
        !IDatabase::getInstance().theReaderSelection->hasSelection()) {
        return;
    }

    IDatabase::getInstance().deleteCurrentReader();
}

// 查询借阅记录按钮点击
void MasterView::on_btnBorrowQuery_clicked()
{
    QString searchBorrowText = ui->BorrowSearchlineEdit->text().trimmed();

    if (searchBorrowText.isEmpty()) {
        IDatabase::getInstance().borrowTabModel->setFilter("");
        IDatabase::getInstance().borrowTabModel->select();
        return;
    }

    QString filter = QString("reader_no LIKE '%%1%' OR isbn LIKE '%%1%'")
                         .arg(searchBorrowText);

    IDatabase::getInstance().queryBorrow(filter);
}

// 删除借阅记录按钮点击
void MasterView::on_btnBorrowDelete_clicked()
{
    if (!IDatabase::getInstance().theBorrowSelection ||
        !IDatabase::getInstance().theBorrowSelection->hasSelection()) {
        return;
    }

    IDatabase::getInstance().deleteCurrentBorrow();
}

// 借阅按钮点击
void MasterView::on_btnBorrowed_clicked()
{
    if (borrowedDialog != nullptr && borrowedDialog->isVisible()) {
        borrowedDialog->activateWindow();
        borrowedDialog->raise();
        return;
    }

    if (borrowedDialog) {
        delete borrowedDialog;
    }

    borrowedDialog = new BorrowedView(this);

    connect(borrowedDialog, &BorrowedView::dataUpdated, this, [this]() {
        qDebug() << "借阅成功，刷新数据...";
        // 刷新所有模型
        IDatabase::getInstance().borrowTabModel->select();
        IDatabase::getInstance().bookTabModel->select();
        IDatabase::getInstance().readerTabModel->select();
    });

    connect(borrowedDialog, &BorrowedView::dialogClosed, this, [this]() {
        qDebug() << "借阅对话框关闭";
        borrowedDialog = nullptr;
    });

    borrowedDialog->setWindowTitle("借阅信息填写");
    borrowedDialog->show();
    borrowedDialog->raise();
    borrowedDialog->activateWindow();
}

// 归还图书按钮点击
void MasterView::on_btnBorrowReturn_clicked()
{
    qDebug() << "\n=== 开始归还图书 ===";

    if (!IDatabase::getInstance().theBorrowSelection ||
        !IDatabase::getInstance().theBorrowSelection->hasSelection()) {
        qDebug() << "请先选择要归还的借阅记录";
        return;
    }

    QModelIndex currentIndex = IDatabase::getInstance().theBorrowSelection->currentIndex();
    if (!currentIndex.isValid()) {
        qDebug() << "选中的索引无效";
        return;
    }

    int currentRow = currentIndex.row();

    // 获取借阅记录信息
    QString readerNo = IDatabase::getInstance().borrowTabModel->data(
                                                                  IDatabase::getInstance().borrowTabModel->index(currentRow, 0)).toString();
    QString isbn = IDatabase::getInstance().borrowTabModel->data(
                                                              IDatabase::getInstance().borrowTabModel->index(currentRow, 1)).toString();
    QString borrowDate = IDatabase::getInstance().borrowTabModel->data(
                                                                    IDatabase::getInstance().borrowTabModel->index(currentRow, 2)).toString();
    QString dueDateStr = IDatabase::getInstance().borrowTabModel->data(
                                                                    IDatabase::getInstance().borrowTabModel->index(currentRow, 3)).toString();

    qDebug() << "读者编号:" << readerNo;
    qDebug() << "ISBN:" << isbn;
    qDebug() << "借阅日期:" << borrowDate;
    qDebug() << "应还日期:" << dueDateStr;

    // 检查是否已归还
    int isReturned = IDatabase::getInstance().borrowTabModel->data(
                                                                IDatabase::getInstance().borrowTabModel->index(currentRow, 5)).toInt();

    if (isReturned == 1) {
        qDebug() << "该书籍已归还，无需再次归还";
        return;
    }

    QDate dueDate = QDate::fromString(dueDateStr, "yyyy-MM-dd");
    QDate currentDate = QDate::currentDate();

    // 计算是否逾期
    bool isOverdue = currentDate > dueDate;
    int overdueDays = 0;

    if (isOverdue) {
        overdueDays = dueDate.daysTo(currentDate);
        qDebug() << "注意: 已逾期" << overdueDays << "天";
    } else {
        qDebug() << "未逾期，按时归还";
    }

    // 开始事务
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        qDebug() << "开始事务失败";
        return;
    }

    try {
        // 更新借阅记录，设置归还日期
        QSqlQuery updateQuery;
        updateQuery.prepare(
            "UPDATE borrow_records SET "
            "return_date = DATE('now'), "
            "is_returned = 1, "
            "updated_at = CURRENT_TIMESTAMP "
            "WHERE reader_no = ? AND isbn = ? AND borrow_date = ? AND is_returned = 0"
            );
        updateQuery.addBindValue(readerNo);
        updateQuery.addBindValue(isbn);
        updateQuery.addBindValue(borrowDate);

        if (!updateQuery.exec()) {
            db.rollback();
            qDebug() << "更新借阅记录失败:" << updateQuery.lastError().text();
            return;
        }

        int rowsAffected = updateQuery.numRowsAffected();
        if (rowsAffected == 0) {
            db.rollback();
            qDebug() << "更新失败，可能是记录已被归还";
            return;
        }

        if (!db.commit()) {
            db.rollback();
            qDebug() << "提交事务失败";
            return;
        }

        qDebug() << "归还成功!";

        if (isOverdue) {
            qDebug() << "本次借阅逾期" << overdueDays << "天，请注意查看逾期记录";
        }

        // 刷新模型
        IDatabase::getInstance().borrowTabModel->select();
        IDatabase::getInstance().bookTabModel->select();
        IDatabase::getInstance().readerTabModel->select();

    } catch (...) {
        db.rollback();
        qDebug() << "归还过程中发生异常";
    }
}

// 报表查询按钮点击
void MasterView::on_btnReportQuery_clicked()
{
    qDebug() << "查询统计按钮被点击";
    generateReport();
}

// 导出数据按钮点击
void MasterView::exportDataClicked()
{
    qDebug() << "导出数据按钮被点击";
    exportToExcel();
}

// 获取借阅最多的图书
QString MasterView::getMostBorrowedBook()
{
    QSqlQuery query;
    query.prepare("SELECT isbn, COUNT(*) as borrow_count FROM borrow_records GROUP BY isbn ORDER BY borrow_count DESC LIMIT 1");

    if (query.exec() && query.next()) {
        QString isbn = query.value(0).toString();
        int borrowCount = query.value(1).toInt();

        // 查询图书详细信息
        QSqlQuery bookQuery;
        bookQuery.prepare("SELECT title, author FROM books WHERE isbn = ?");
        bookQuery.addBindValue(isbn);

        if (bookQuery.exec() && bookQuery.next()) {
            QString title = bookQuery.value(0).toString();
            QString author = bookQuery.value(1).toString();
            return QString("%1 (ISBN: %2) - 借阅次数: %3次").arg(title, isbn, QString::number(borrowCount));
        }
    }

    return "暂无借阅记录";
}

// 获取借阅最少的图书
QString MasterView::getLeastBorrowedBook()
{
    QSqlQuery query;
    query.prepare("SELECT isbn, COUNT(*) as borrow_count FROM borrow_records GROUP BY isbn ORDER BY borrow_count ASC LIMIT 1");

    if (query.exec() && query.next()) {
        QString isbn = query.value(0).toString();
        int borrowCount = query.value(1).toInt();

        // 查询图书详细信息
        QSqlQuery bookQuery;
        bookQuery.prepare("SELECT title, author FROM books WHERE isbn = ?");
        bookQuery.addBindValue(isbn);

        if (bookQuery.exec() && bookQuery.next()) {
            QString title = bookQuery.value(0).toString();
            QString author = bookQuery.value(1).toString();
            return QString("%1 (ISBN: %2) - 借阅次数: %3次").arg(title, isbn, QString::number(borrowCount));
        }
    }

    return "暂无借阅记录";
}

// 获取借书最多的读者
QString MasterView::getMostActiveReader()
{
    QSqlQuery query;
    query.prepare("SELECT reader_no, COUNT(*) as borrow_count FROM borrow_records GROUP BY reader_no ORDER BY borrow_count DESC LIMIT 1");

    if (query.exec() && query.next()) {
        QString readerNo = query.value(0).toString();
        int borrowCount = query.value(1).toInt();

        // 查询读者详细信息
        QSqlQuery readerQuery;
        readerQuery.prepare("SELECT name, phone FROM readers WHERE reader_no = ?");
        readerQuery.addBindValue(readerNo);

        if (readerQuery.exec() && readerQuery.next()) {
            QString name = readerQuery.value(0).toString();
            QString phone = readerQuery.value(1).toString();
            return QString("%1 (编号: %2) - 借书次数: %3次").arg(name, readerNo, QString::number(borrowCount));
        }
    }

    return "暂无借阅记录";
}

// 获取借书最少的读者
QString MasterView::getLeastActiveReader()
{
    QSqlQuery query;
    query.prepare("SELECT reader_no, COUNT(*) as borrow_count FROM borrow_records GROUP BY reader_no ORDER BY borrow_count ASC LIMIT 1");

    if (query.exec() && query.next()) {
        QString readerNo = query.value(0).toString();
        int borrowCount = query.value(1).toInt();

        // 查询读者详细信息
        QSqlQuery readerQuery;
        readerQuery.prepare("SELECT name, phone FROM readers WHERE reader_no = ?");
        readerQuery.addBindValue(readerNo);

        if (readerQuery.exec() && readerQuery.next()) {
            QString name = readerQuery.value(0).toString();
            QString phone = readerQuery.value(1).toString();
            return QString("%1 (编号: %2) - 借书次数: %3次").arg(name, readerNo, QString::number(borrowCount));
        }
    }

    return "暂无借阅记录";
}

// 生成统计报表
void MasterView::generateReport()
{
    qDebug() << "开始生成统计报表...";

    // 获取统计信息
    QString mostBorrowedBook = getMostBorrowedBook();
    QString leastBorrowedBook = getLeastBorrowedBook();
    QString mostActiveReader = getMostActiveReader();
    QString leastActiveReader = getLeastActiveReader();

    // 更新UI显示
    ui->lineEditMostBorrowed->setText(mostBorrowedBook);
    ui->lineEditLoseBorrowed->setText(leastBorrowedBook);
    ui->lineEditMBorrowedReader->setText(mostActiveReader);
    ui->lineEditLBorrowedReader->setText(leastActiveReader);

    qDebug() << "统计报表生成完成";
    qDebug() << "借阅次数最多的图书：" << mostBorrowedBook;
    qDebug() << "借阅次数最少的图书：" << leastBorrowedBook;
    qDebug() << "借书次数最多的读者：" << mostActiveReader;
    qDebug() << "借书次数最少的读者：" << leastActiveReader;
}

// 导出到Excel
void MasterView::exportToExcel()
{
    qDebug() << "开始导出到Excel";

    // 获取统计信息
    QString mostBorrowedBook = getMostBorrowedBook();
    QString leastBorrowedBook = getLeastBorrowedBook();
    QString mostActiveReader = getMostActiveReader();
    QString leastActiveReader = getLeastActiveReader();

    // 弹出文件保存对话框
    QString fileName = QFileDialog::getSaveFileName(this, "导出报表",
                                                    "图书借阅统计报表_" + QDate::currentDate().toString("yyyy-MM-dd") + ".csv",
                                                    "CSV Files (*.csv);;All Files (*)");

    if (fileName.isEmpty()) {
        qDebug() << "用户取消了导出";
        return;
    }

    qDebug() << "保存文件到：" << fileName;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "无法创建文件:" << fileName;
        QMessageBox::warning(this, "导出失败", "无法创建文件！");
        return;
    }

    QTextStream out(&file);

    // 在Qt 6中，使用setGenerateByteOrderMark来生成UTF-8 BOM
    out.setGenerateByteOrderMark(true);

    // 写入标题
    out << "图书借阅统计报表\n";
    out << "生成时间：" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n\n";

    // 写入统计项目
    out << "统计项目,统计结果\n";
    out << "借阅次数最多的图书," << mostBorrowedBook << "\n";
    out << "借阅次数最少的图书," << leastBorrowedBook << "\n";
    out << "借书次数最多的读者," << mostActiveReader << "\n";
    out << "借书次数最少的读者," << leastActiveReader << "\n";

    file.close();

    qDebug() << "报表已导出到:" << fileName;
    QMessageBox::information(this, "导出成功", QString("报表已成功导出到:\n%1").arg(fileName));
}

// 退出系统按钮点击
void MasterView::on_btnExitSystem_clicked()
{
    close();
}
