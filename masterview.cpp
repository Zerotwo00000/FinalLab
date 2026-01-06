#include "masterview.h"
#include "ui_masterview.h"
#include "bookeditview.h"
#include "readereditview.h"
#include "borrowedview.h"
#include "idatabase.h"
#include <QDebug>

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

    // 读者表格设置
    ui->ReadertableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ReadertableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->ReadertableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->ReadertableView->setAlternatingRowColors(true);

    // 借阅表格设置
    ui->BorrowtableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->BorrowtableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->BorrowtableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->BorrowtableView->setAlternatingRowColors(true);
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

    // 连接退出系统按钮
    connect(ui->btnExitSystem, &QPushButton::clicked, this, &MasterView::on_btnExitSystem_clicked);
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

// 借阅按钮点击
void MasterView::on_btnBorrowed_clicked()
{
    // 检查是否已经有对话框打开
    if (borrowedDialog != nullptr && borrowedDialog->isVisible()) {
        borrowedDialog->activateWindow();
        borrowedDialog->raise();
        return;
    }

    if (!borrowedDialog) {
        borrowedDialog = new BorrowedView(this);
    }

    borrowedDialog->setWindowTitle("借阅信息填写");
    borrowedDialog->show();
    borrowedDialog->raise();
    borrowedDialog->activateWindow();
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

// 归还图书按钮点击
void MasterView::on_btnBorrowReturn_clicked()
{
    // 暂时不实现
}

// 退出系统按钮点击
void MasterView::on_btnExitSystem_clicked()
{
    close();
}
