#include "masterview.h"
#include "ui_masterview.h"
#include <QButtonGroup>

MasterView::MasterView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MasterView)
{
    ui->setupUi(this);

    // 设置窗口标题
    setWindowTitle("图书借阅管理系统");

    // 设置窗口大小
    resize(1000, 700);

    // 设置导航系统
    setupNavigation();

    // 设置按钮组
    setupButtonGroups();
}

MasterView::~MasterView()
{
    delete ui;
}

void MasterView::setupNavigation()
{
    // 设置按钮为可选状态
    ui->btnBookManage->setCheckable(true);
    ui->btnReaderManage->setCheckable(true);
    ui->btnBorrowManage->setCheckable(true);
    ui->btnReportManage->setCheckable(true);

    // 默认选中图书管理
    ui->btnBookManage->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
}

void MasterView::setupButtonGroups()
{
    // 创建左侧导航按钮组
    QButtonGroup *navGroup = new QButtonGroup(this);
    navGroup->addButton(ui->btnBookManage, 0);
    navGroup->addButton(ui->btnReaderManage, 1);
    navGroup->addButton(ui->btnBorrowManage, 2);
    navGroup->addButton(ui->btnReportManage, 3);
    navGroup->setExclusive(true);

    // 连接导航按钮到StackedWidget
    connect(navGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            [this, navGroup](QAbstractButton *button) {
                int id = navGroup->id(button);
                ui->stackedWidget->setCurrentIndex(id);
            });
}

void MasterView::on_btnExitSystem_clicked()
{
    // 退出程序
    close();
}
