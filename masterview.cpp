#include "masterview.h"
#include "ui_masterview.h"
#include "bookeditview.h"  // 包含图书编辑界面
#include "readereditview.h"  // 包含读者编辑界面
#include "borrowedview.h"  // 包含借阅界面
#include <QButtonGroup>
#include <QDate>
#include "idatabase.h"
#include <QMessageBox>

MasterView::MasterView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MasterView)
    , bookEditDialog(nullptr)  // 初始化为nullptr
    , readerEditDialog(nullptr)  // 初始化为nullptr
    , borrowedDialog(nullptr)  // 初始化为nullptr
{
    ui->setupUi(this);

    // 设置窗口标题
    setWindowTitle("图书借阅管理系统");

    // 设置窗口大小
    resize(1000, 700);

    // 设置导航功能
    setupNavigation();

    // 连接信号槽
    setupConnections();

    IDatabase::getInstance();

    ui->BooktableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->BooktableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->BooktableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->BooktableView->setAlternatingRowColors(true);

    ui->ReadertableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ReadertableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->ReadertableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->ReadertableView->setAlternatingRowColors(true);

    ui->BorrowtableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->BorrowtableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->BorrowtableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->BorrowtableView->setAlternatingRowColors(true);

    IDatabase &iDatabase = IDatabase::getInstance();
    if (iDatabase.initBookModel()){
        ui->BooktableView->setModel(iDatabase.bookTabModel);
        ui->BooktableView->setSelectionModel(iDatabase.theBookSelection);
    }

    if (iDatabase.initReaderModel()){
        ui->ReadertableView->setModel(iDatabase.readerTabModel);
        ui->ReadertableView->setSelectionModel(iDatabase.theReaderSelection);
    }

    if (iDatabase.initBorrowModel()){
        ui->BorrowtableView->setModel(iDatabase.borrowTabModel);
        ui->BorrowtableView->setSelectionModel(iDatabase.theBorrowSelection);
    }



}

MasterView::~MasterView()
{
    delete ui;
    if (bookEditDialog) {
        delete bookEditDialog;
    }
    if (readerEditDialog) {
        delete readerEditDialog;
    }
    if (borrowedDialog) {
        delete borrowedDialog;
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

// 图书管理按钮点击
void MasterView::on_btnBookManage_clicked()
{
    // 切换页面
    ui->stackedWidget->setCurrentIndex(0);

    // 更新按钮选中状态
    ui->btnBookManage->setChecked(true);
    ui->btnReaderManage->setChecked(false);
    ui->btnBorrowManage->setChecked(false);
    ui->btnReportManage->setChecked(false);
}

// 读者管理按钮点击
void MasterView::on_btnReaderManage_clicked()
{
    // 切换页面
    ui->stackedWidget->setCurrentIndex(1);

    // 更新按钮选中状态
    ui->btnBookManage->setChecked(false);
    ui->btnReaderManage->setChecked(true);
    ui->btnBorrowManage->setChecked(false);
    ui->btnReportManage->setChecked(false);
}

// 借阅管理按钮点击
void MasterView::on_btnBorrowManage_clicked()
{
    // 切换页面
    ui->stackedWidget->setCurrentIndex(2);

    // 更新按钮选中状态
    ui->btnBookManage->setChecked(false);
    ui->btnReaderManage->setChecked(false);
    ui->btnBorrowManage->setChecked(true);
    ui->btnReportManage->setChecked(false);
}

// 统计报表按钮点击
void MasterView::on_btnReportManage_clicked()
{
    // 切换页面
    ui->stackedWidget->setCurrentIndex(3);

    // 更新按钮选中状态
    ui->btnBookManage->setChecked(false);
    ui->btnReaderManage->setChecked(false);
    ui->btnBorrowManage->setChecked(false);
    ui->btnReportManage->setChecked(true);
}

// 添加图书按钮点击
void MasterView::on_btnBookAdd_clicked()
{
    if (!bookEditDialog) {
        bookEditDialog = new BookEditView(this);
    }

    // 设置对话框为添加模式
    bookEditDialog->setWindowTitle("添加图书");

    // 显示对话框
    bookEditDialog->show();
    bookEditDialog->raise();
    bookEditDialog->activateWindow();

}

// 修改图书按钮点击
void MasterView::on_btnBookModify_clicked()
{
    if (!bookEditDialog) {
        bookEditDialog = new BookEditView(this);
    }

    // 设置对话框为修改模式
    bookEditDialog->setWindowTitle("修改图书");

    // 显示对话框
    bookEditDialog->show();
    bookEditDialog->raise();
    bookEditDialog->activateWindow();
}

// 查询图书按钮点击
void MasterView::on_btnBookQuery_clicked()
{
    // 获取搜索文本
    QString searchBookText = ui->BookSearchlineEdit->text().trimmed();

    if (searchBookText.isEmpty()) {
        // 如果搜索框为空，清空过滤器，显示所有图书
        IDatabase::getInstance().bookTabModel->setFilter("");
        IDatabase::getInstance().bookTabModel->select();
        return;
    }

    // 根据图书表字段构建多条件模糊查询
    // 可以查询：书名、作者、出版社、ISBN、分类
    QString Bookfilter = QString("title LIKE '%%1%' OR "
                             "author LIKE '%%1%' OR "
                             "publisher LIKE '%%1%' OR "
                             "isbn LIKE '%%1%' OR "
                             "category LIKE '%%1%'")
                         .arg(searchBookText);

    // 执行查询
    IDatabase::getInstance().queryBook(Bookfilter);
}

// 删除图书按钮点击
void MasterView::on_btnBookDelete_clicked()
{
    // 获取当前选中的行索引
    QModelIndex currentBookIndex = IDatabase::getInstance().theBookSelection->currentIndex();

    // 获取要删除的图书信息（用于提示用户）
    int Bookrow = currentBookIndex.row();
    QModelIndex titleIndex = IDatabase::getInstance().bookTabModel->index(Bookrow, 1);  // 第1列是书名
    QString bookTitle = IDatabase::getInstance().bookTabModel->data(titleIndex).toString();
    bool deleteSuccessB = IDatabase::getInstance().deleteCurrentBook();
}

// 添加读者按钮点击
void MasterView::on_btnReaderAdd_clicked()
{
    if (!readerEditDialog) {
        readerEditDialog = new ReaderEditView(this);
    }

    // 设置对话框为添加模式
    readerEditDialog->setWindowTitle("添加读者");

    // 显示对话框
    readerEditDialog->show();
    readerEditDialog->raise();
    readerEditDialog->activateWindow();
}

// 修改读者按钮点击
void MasterView::on_btnReaderModify_clicked()
{
    if (!readerEditDialog) {
        readerEditDialog = new ReaderEditView(this);
    }

    // 设置对话框为修改模式
    readerEditDialog->setWindowTitle("修改读者");

    // 显示对话框
    readerEditDialog->show();
    readerEditDialog->raise();
    readerEditDialog->activateWindow();
}

// 查询读者按钮点击
void MasterView::on_btnReaderQuery_clicked()
{
    // 获取搜索文本
    QString searchReaderText = ui->ReaderSearchlineEdit->text().trimmed();

    if (searchReaderText.isEmpty()) {
        // 如果搜索框为空，清空过滤器，显示所有图书
        IDatabase::getInstance().readerTabModel->setFilter("");
        IDatabase::getInstance().readerTabModel->select();
        return;
    }

    // 根据读者表字段构建多条件模糊查询
    // 可以查询：读者编号、姓名、电话、邮箱、身份证
    QString Readerfilter = QString("name LIKE '%%1%' OR "
                             "phone LIKE '%%1%' OR "
                             "email LIKE '%%1%' OR "
                             "reader_no LIKE '%%1%' OR "
                             "id_card LIKE '%%1%'")
                         .arg(searchReaderText);

    // 执行查询
    IDatabase::getInstance().queryReader(Readerfilter);
}

// 删除读者按钮点击
void MasterView::on_btnReaderDelete_clicked()
{
    // 获取当前选中的行索引
    QModelIndex currentReaderIndex = IDatabase::getInstance().theReaderSelection->currentIndex();

    // 获取要删除的读者信息（用于提示用户）
    int Readerrow = currentReaderIndex.row();
    QModelIndex nameIndex = IDatabase::getInstance().readerTabModel->index(Readerrow, 1);  // 第1列是姓名
    QString readerName = IDatabase::getInstance().readerTabModel->data(nameIndex).toString();
    bool deleteSuccessR = IDatabase::getInstance().deleteCurrentReader();
}

// 借阅按钮点击
void MasterView::on_btnBorrowed_clicked()
{
    if (!borrowedDialog) {
        borrowedDialog = new BorrowedView(this);
    }

    // 设置对话框标题
    borrowedDialog->setWindowTitle("借阅信息填写");

    // 显示对话框
    borrowedDialog->show();
    borrowedDialog->raise();
    borrowedDialog->activateWindow();
}

// 查询借阅记录按钮点击
void MasterView::on_btnBorrowQuery_clicked()
{
    // 获取搜索文本
    QString searchText = ui->BorrowSearchlineEdit->text();

    // 这里可以添加查询逻辑
    // 暂时不显示任何提示
}

// 删除借阅记录按钮点击
void MasterView::on_btnBorrowDelete_clicked()
{
    // 这里可以添加删除逻辑
    // 暂时不显示任何提示
}

// 归还图书按钮点击
void MasterView::on_btnBorrowReturn_clicked()
{
    // 这里可以添加归还逻辑
    // 暂时不显示任何提示
}

// 退出系统按钮点击
void MasterView::on_btnExitSystem_clicked()
{
    close();
}
