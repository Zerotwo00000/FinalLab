#include "masterview.h"
#include "ui_masterview.h"
#include "bookeditview.h"  // 包含图书编辑界面
#include <QButtonGroup>

MasterView::MasterView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MasterView)
    , bookEditDialog(nullptr)  // 初始化为nullptr
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
}

MasterView::~MasterView()
{
    delete ui;
    if (bookEditDialog) {
        delete bookEditDialog;
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
    QString searchText = ui->BookSearchlineEdit->text();

    // 这里可以添加查询逻辑
    // 暂时不显示任何提示
}

// 删除图书按钮点击
void MasterView::on_btnBookDelete_clicked()
{
    // 这里可以添加删除逻辑
    // 暂时不显示任何提示
}

// 退出系统按钮点击
void MasterView::on_btnExitSystem_clicked()
{
    close();
}
