#include "readereditview.h"
#include "ui_readereditview.h"
#include <QCloseEvent>

ReaderEditView::ReaderEditView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReaderEditView)
{
    ui->setupUi(this);

    // 设置窗口标志，使其成为一个独立的窗口
    setWindowFlags(Qt::Window);

    // 连接按钮信号
    connect(ui->BtnReaderOk, &QPushButton::clicked, this, &ReaderEditView::on_BtnReaderOK_clicked);
    connect(ui->BtnReaderCancel, &QPushButton::clicked, this, &ReaderEditView::on_BtnReaderCancel_clicked);
}

ReaderEditView::~ReaderEditView()
{
    delete ui;
}

void ReaderEditView::closeEvent(QCloseEvent *event)
{
    // 关闭窗口时发出destroyed信号
    QWidget::closeEvent(event);
}

// 确定按钮点击
void ReaderEditView::on_BtnReaderOK_clicked()
{
    // 获取表单数据
    QString readerNo = ui->lineEditReaderNo->text();
    QString name = ui->lineEditName->text();

    // 简单的表单验证
    if (readerNo.isEmpty() || name.isEmpty()) {
        // 可以设置输入框样式提示错误，但不弹窗
        return;
    }

    // 保存成功，不显示提示
    close();
}

// 取消按钮点击
void ReaderEditView::on_BtnReaderCancel_clicked()
{
    close();
}
